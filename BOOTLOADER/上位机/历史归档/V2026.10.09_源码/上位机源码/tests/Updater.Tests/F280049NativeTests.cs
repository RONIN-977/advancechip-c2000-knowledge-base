using System.Runtime.InteropServices;
using Updater.Core;

internal static class F280049NativeTests
{
    public static async Task<int> Run(string dll, string firmware, string converter)
    {
        int passed = 0;
        void Assert(bool value) { if (!value) throw new Exception("Native C protocol assertion failed"); }
        using var model = new Model(dll);
        foreach (bool can in new[] { false, true })
        {
            foreach (bool maximum in new[] { false, true })
            {
                model.Reset();
                var image = await TiOutConverter.LoadAsync(firmware, converter, target: DeviceProfile.F280049);
                if (maximum) image = image with { Words = Enumerable.Range(0, 65520).Select(i => (ushort)(i ^ 0xa55a)).ToArray() };
                await using var transport = new Wire(model, can);
                transport.Drop.UnionWith(new[] { Command.Begin, Command.Data, Command.Finish });
                var client = new UpdaterClient(transport);
                await client.UpdateAsync(image, null, CancellationToken.None);
                Assert(model.Stat(0) == 1 && model.Stat(1) == (image.Words.Length + 59) / 60 && model.Stat(2) == 1 && model.Stat(3) == 1 && model.Stat(4) == (uint)BootState.Valid);
                await client.ResetAsync(CancellationToken.None);
                passed++; Console.WriteLine($"PASS native 280049 {(can ? "CAN" : "SCI")} {(maximum ? "128 KB boundary" : "actual TI OUT")}, lost BEGIN/DATA/FINISH ACK, no repeated Flash operation");
            }
        }
        for (ushort fault = 1; fault <= 4; fault++)
        {
            model.Reset(); model.SetFault(fault);
            await using var transport = new Wire(model, false);
            try { await new UpdaterClient(transport).UpdateAsync(FirmwareImage.Demo(DeviceProfile.F280049), null, CancellationToken.None); throw new Exception("Fault ignored"); }
            catch (IOException) { }
            Assert(model.Stat(2) == 0 && model.Stat(3) == 0 && model.Stat(4) == (uint)BootState.Failed);
            passed++; Console.WriteLine($"PASS native 280049 injected Flash fault {fault} leaves invalid application");
        }
        model.Reset();
        await using (var transport = new Wire(model, true))
        {
            using var cancel = new CancellationTokenSource();
            var client = new UpdaterClient(transport);
            var image = FirmwareImage.Demo(DeviceProfile.F280049);
            try { await client.UpdateAsync(image, new ProgressNow(p => { if (p.Percent > 0) cancel.Cancel(); }), cancel.Token); throw new Exception("Cancellation ignored"); }
            catch (OperationCanceledException) { }
            Assert(model.Stat(3) == 0);
            await client.UpdateAsync(image, null, CancellationToken.None);
            Assert(model.Stat(3) == 1);
            passed++; Console.WriteLine("PASS native 280049 cancellation, ABORT, fresh session recovery");
        }
        model.Reset();
        await using (var transport = new Wire(model, false))
        {
            try { await new UpdaterClient(transport).UpdateAsync(FirmwareImage.Demo(), null, CancellationToken.None); throw new Exception("Wrong model accepted"); }
            catch (InvalidDataException) { }
            Assert(model.Stat(0) == 0);
            var request = new Packet(2, 1, 1, FirmwareImage.Demo().Manifest());
            Assert(model.Exchange(false, request).Payload[0] == (byte)Status.BadImage && model.Stat(0) == 0);
            var corrupt = request.Encode(); corrupt[^1] ^= 1;
            Assert(model.Raw(false, corrupt).Length == 0 && model.Stat(0) == 0);
            passed++; Console.WriteLine("PASS native 280049 mismatched model/manifest and corrupt CRC rejected before erase");
        }
        return passed;
    }
    private sealed class ProgressNow(Action<UpdateProgress> action) : IProgress<UpdateProgress>
    { public void Report(UpdateProgress p) => action(p); }
    private sealed class Wire(Model model, bool can) : ITransport
    {
        public readonly HashSet<Command> Drop = [];
        public Task<Packet> ExchangeAsync(Packet request, TimeSpan timeout, CancellationToken cancellation)
        {
            cancellation.ThrowIfCancellationRequested();
            var reply = model.Exchange(can, request);
            if (Drop.Remove((Command)request.Command)) throw new TimeoutException("Drop ACK after actual C execution");
            return Task.FromResult(reply);
        }
        public ValueTask DisposeAsync() => ValueTask.CompletedTask;
    }
    private sealed class Model : IDisposable
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate void ResetFn();
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate void FaultFn(ushort value);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate uint StatFn(ushort index);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate ushort ExchangeFn(ushort transport, [In] ushort[] input, ushort size, [Out] ushort[] output);
        private readonly nint library;
        private readonly ResetFn reset;
        private readonly FaultFn fault;
        private readonly StatFn stat;
        private readonly ExchangeFn exchange;
        public Model(string path)
        {
            library = NativeLibrary.Load(path);
            reset = Marshal.GetDelegateForFunctionPointer<ResetFn>(NativeLibrary.GetExport(library, "native_reset"));
            fault = Marshal.GetDelegateForFunctionPointer<FaultFn>(NativeLibrary.GetExport(library, "native_fault"));
            stat = Marshal.GetDelegateForFunctionPointer<StatFn>(NativeLibrary.GetExport(library, "native_stat"));
            exchange = Marshal.GetDelegateForFunctionPointer<ExchangeFn>(NativeLibrary.GetExport(library, "native_exchange"));
        }
        public void Reset() => reset();
        public void SetFault(ushort value) => fault(value);
        public uint Stat(ushort index) => stat(index);
        public byte[] Raw(bool can, byte[] input)
        {
            ushort[] output = new ushort[Packet.MaxSize];
            ushort size = exchange(can ? (ushort)2 : (ushort)1, input.Select(b => (ushort)b).ToArray(), checked((ushort)input.Length), output);
            return output.Take(size).Select(b => checked((byte)b)).ToArray();
        }
        public Packet Exchange(bool can, Packet request) => Packet.Decode(Raw(can, request.Encode()));
        public void Dispose() => NativeLibrary.Free(library);
    }
}
