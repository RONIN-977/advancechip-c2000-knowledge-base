using System.Buffers.Binary;
using System.Runtime.InteropServices;
using System.Text.Json;
using Updater.Core;

if (args.Length != 4) throw new ArgumentException("dll app.out hex2000.exe results.json");
var passed = new List<object>();
void Assert(bool ok, string message) { if (!ok) throw new Exception(message); }
async Task Test(string name, Func<Task> body)
{
    await body();
    passed.Add(new { name, status = "PASS" });
    Console.WriteLine("PASS " + name);
}
using var model = new Model(args[0]);
var image = await TiOutConverter.LoadAsync(args[1], args[2], target: DeviceProfile.F280039C);
Console.WriteLine($"Real KB APP image: {image.Words.Length} WORDs, start=0x{image.StartWordAddress:X6}, CRC32=0x{Crc32.Compute(image.Bytes):X8}");
async Task Enter()
{
    model.Reset();
    Assert(model.Stat(0) == 0, "Initial APP mode");
    await using var wire = new Wire(model);
    await new UpdaterClient(wire).EnterBootAsync(CancellationToken.None);
    Assert(model.Stat(0) == 1 && model.Stat(5) == 1, "ENTER_BOOT ACK and reset into BOOT");
}
void CheckImage(FirmwareImage target)
{
    Assert(model.Stat(4) == 1 && model.Stat(3) == 1, "Image must be committed once");
    Assert(model.Stat(6) == target.Words.Length, "Written image length mismatch");
    Assert(model.Stat(7) == Crc32.Compute(target.Bytes), "Whole image CRC mismatch");
    for (int i = 0; i < target.Words.Length; ++i)
        Assert(model.ReadWord(target.StartWordAddress + (uint)i) == target.Words[i], $"Flash WORD mismatch at {i}");
}
await Test("Actual latest APP OUT parses and matches 0039 partition", () => {
    Assert(image.DeviceId == 0x280039c1 && image.BoardId == 0x55505301 && image.Cpu == 1, "Identity");
    Assert(image.StartWordAddress == 0x90000 && image.EntryWordAddress == 0x90000 && image.Words.Length % 4 == 0, "Entry and ECC alignment");
    return Task.CompletedTask;
});
await Test("Real APP receive/dispatch acknowledges ENTER_BOOT then resets into BOOT", Enter);
await Test("Real OUT full CAN upload with exact WORD readback CRC commit and reset into APP", async () => {
    await Enter();
    await using var wire = new Wire(model);
    var client = new UpdaterClient(wire);
    int percent = 0;
    await client.UpdateAsync(image, new ProgressNow(p => percent = p.Percent), CancellationToken.None);
    CheckImage(image);
    Assert(percent == 100 && model.Stat(1) == 1 && model.Stat(2) == (image.Words.Length + 59) / 60, "Write counts");
    Assert(model.Stat(8) > 0 && model.Stat(9) > 0, "Actual C CAN fragment RX/TX exercised");
    await client.ResetAsync(CancellationToken.None);
    Assert(model.Stat(0) == 0 && model.Stat(5) == 2, "RESET returns to APP in model");
});
await Test("Lost BEGIN DATA FINISH ACK retries do not repeat erase program or commit", async () => {
    await Enter();
    await using var wire = new Wire(model);
    wire.Drop.UnionWith([Command.Begin, Command.Data, Command.Finish]);
    await new UpdaterClient(wire).UpdateAsync(image, null, CancellationToken.None);
    CheckImage(image);
    Assert(wire.Dropped == 3 && model.Stat(1) == 1 && model.Stat(2) == (image.Words.Length + 59) / 60 && model.Stat(3) == 1, "Retry repeated Flash operation");
});
await Test("Maximum 248KiB APP partition with short final packet and full readback", async () => {
    await Enter();
    ushort[] words = Enumerable.Range(0, checked((int)(Profile.AppEnd - Profile.AppStart))).Select(i => (ushort)(i ^ 0xa55a)).ToArray();
    image.Words.CopyTo(words, 0);
    var full = image with { Words = words };
    await using var wire = new Wire(model);
    await new UpdaterClient(wire).UpdateAsync(full, null, CancellationToken.None);
    CheckImage(full);
    Assert(full.Words.Length == 126976 && model.Stat(2) == (full.Words.Length + 59) / 60, "Capacity");
});
await Test("Cancellation leaves invalid image and a fresh session recovers", async () => {
    await Enter();
    await using var wire = new Wire(model);
    var client = new UpdaterClient(wire);
    using var cancel = new CancellationTokenSource();
    try {
        await client.UpdateAsync(image, new ProgressNow(p => { if (p.Percent > 0) cancel.Cancel(); }), cancel.Token);
        throw new Exception("Cancellation ignored");
    } catch (OperationCanceledException) when (cancel.IsCancellationRequested) { }
    Assert(model.Stat(4) == 0 && model.Stat(3) == 0, "Interrupted image became valid");
    await client.UpdateAsync(image, null, CancellationToken.None);
    CheckImage(image);
});
for (ushort fault = 1; fault <= 4; ++fault)
{
    ushort selected = fault;
    await Test($"Flash model fault {fault} rejects upgrade and never commits", async () => {
        await Enter(); model.Fault(selected);
        await using var wire = new Wire(model);
        bool rejected = false;
        try { await new UpdaterClient(wire).UpdateAsync(image, null, CancellationToken.None); }
        catch (IOException) { rejected = true; }
        Assert(rejected && model.Stat(3) == 0 && model.Stat(4) == 0, "Fault accepted or committed");
    });
}
await Test("Corrupt CRC rejected by actual C before erase", async () => {
    await Enter();
    byte[] packet = new Packet((byte)Command.Begin, 123, 1, image.Manifest()).Encode();
    packet[^1] ^= 1;
    Assert(model.Raw(packet).Length == 0 && model.Stat(1) == 0 && model.Stat(3) == 0, "Corrupt packet accepted");
});
await Test("Wrong image start entry and length rejected before Flash erase", async () => {
    await Enter();
    foreach ((int field, uint value) in new[] { (12, 0x8f000U), (20, 0x90002U), (16, 0U), (16, 0x1f004U) })
    {
        byte[] manifest = image.Manifest();
        BinaryPrimitives.WriteUInt32LittleEndian(manifest.AsSpan(field), value);
        var response = Packet.Decode(model.Raw(new Packet((byte)Command.Begin, 123, 1, manifest).Encode()));
        Assert(response.Payload[0] == (byte)Status.BadImage && model.Stat(1) == 0, "Bad manifest accepted");
    }
});
await Test("Wrong device ID rejected before Flash erase", async () => {
    await Enter();
    byte[] manifest = image.Manifest();
    BinaryPrimitives.WriteUInt32LittleEndian(manifest, 0x28004901);
    var response = Packet.Decode(model.Raw(new Packet((byte)Command.Begin, 123, 1, manifest).Encode()));
    Assert(response.Payload[0] == (byte)Status.BadImage && model.Stat(1) == 0, "Wrong device accepted");
});
await Test("Foreign session and out of order DATA rejected without programming", async () => {
    await Enter();
    _ = Packet.Decode(model.Raw(new Packet((byte)Command.Begin, 123, 1, image.Manifest()).Encode()));
    byte[] payload = new byte[12];
    BinaryPrimitives.WriteUInt32LittleEndian(payload, image.StartWordAddress);
    foreach (var packet in new[] { new Packet(3, 124, 2, payload), new Packet(3, 123, 3, payload) })
    {
        var response = Packet.Decode(model.Raw(packet.Encode()));
        Assert(response.Payload[0] != 0 && model.Stat(2) == 0, "Foreign/out of order packet accepted");
    }
});
for (ushort fault = 6; fault <= 7; ++fault)
{
    ushort selected = fault;
    await Test($"CAN fragment fault {fault} no erase and next complete request recovers", async () => {
        await Enter(); model.Fault(selected);
        var begin = new Packet((byte)Command.Begin, 123, 1, image.Manifest()).Encode();
        Assert(model.Raw(begin).Length == 0 && model.Stat(1) == 0, "Incomplete/reordered frame accepted");
        model.Fault(0);
        var response = Packet.Decode(model.Raw(begin));
        Assert(response.Payload[0] == 0 && model.Stat(1) == 1, "CAN reassembly did not recover");
    });
}
await Test("APP does not reset when CAN ENTER_BOOT ACK cannot finish", () => {
    model.Reset(); model.Fault(5);
    var response = model.Raw(new Packet((byte)Command.EnterBoot, 0, 0, []).Encode());
    Assert(response.Length == 0 && model.Stat(0) == 0 && model.Stat(5) == 0, "APP reset before ACK completion");
    return Task.CompletedTask;
});
var result = new {
    status = "PASS", tests = passed.Count, cases = passed,
    firmware = Path.GetFullPath(args[1]), imageWords = image.Words.Length,
    imageStart = $"0x{image.StartWordAddress:X6}", imageCrc32 = $"0x{Crc32.Compute(image.Bytes):X8}",
    execution = "Current Updater.Core C# source + actual KB C protocol/app entry/CAN send; Flash/register/reset adapters",
    hardwareTest = "NOT RUN", cpuInstructionSimulation = false
};
File.WriteAllText(args[3], JsonSerializer.Serialize(result, new JsonSerializerOptions { WriteIndented = true }));
Console.WriteLine($"All {passed.Count} current KB CAN simulation cases passed.");

sealed class ProgressNow(Action<UpdateProgress> action) : IProgress<UpdateProgress>
{ public void Report(UpdateProgress p) => action(p); }
sealed class Wire(Model model) : ITransport
{
    public readonly HashSet<Command> Drop = [];
    public int Dropped;
    public Task<Packet> ExchangeAsync(Packet request, TimeSpan timeout, CancellationToken cancellation)
    {
        cancellation.ThrowIfCancellationRequested();
        byte[] wire = model.Raw(request.Encode());
        if (wire.Length == 0) throw new TimeoutException("C model returned no complete CAN reply");
        if (Drop.Remove((Command)request.Command)) { Dropped++; throw new TimeoutException("Injected lost ACK after real C execution"); }
        return Task.FromResult(Packet.Decode(wire));
    }
    public ValueTask DisposeAsync() => ValueTask.CompletedTask;
}
sealed class Model : IDisposable
{
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate void ResetFn();
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate void FaultFn(ushort fault);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate uint StatFn(ushort index);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate ushort WordFn(uint address);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate ushort ExchangeFn(ushort transport, [In] ushort[] input, ushort size, [Out] ushort[] output);
    private readonly nint library;
    private readonly ResetFn reset;
    private readonly FaultFn fault;
    private readonly StatFn stat;
    private readonly WordFn word;
    private readonly ExchangeFn exchange;
    public Model(string path)
    {
        library = NativeLibrary.Load(Path.GetFullPath(path));
        T Get<T>(string name) where T : Delegate => Marshal.GetDelegateForFunctionPointer<T>(NativeLibrary.GetExport(library, name));
        reset = Get<ResetFn>("native_reset"); fault = Get<FaultFn>("native_fault");
        stat = Get<StatFn>("native_stat"); word = Get<WordFn>("native_read_word"); exchange = Get<ExchangeFn>("native_exchange");
    }
    public void Reset() => reset();
    public void Fault(ushort value) => fault(value);
    public uint Stat(ushort index) => stat(index);
    public ushort ReadWord(uint address) => word(address);
    public byte[] Raw(byte[] input)
    {
        ushort[] output = new ushort[Packet.MaxSize];
        ushort size = exchange(2, input.Select(b => (ushort)b).ToArray(), checked((ushort)input.Length), output);
        return output.Take(size).Select(b => checked((byte)b)).ToArray();
    }
    public void Dispose() => NativeLibrary.Free(library);
}
