using System.Buffers.Binary;
using Updater.Core;

internal static class F2800157Tests
{
    public static async Task<int> Run(string firmwareDirectory, string converter)
    {
        int passed = 0;
        void Assert(bool value) { if (!value) throw new Exception("280157 assertion failed"); }
        void Reject(Action action)
        {
            try { action(); } catch (InvalidDataException) { return; }
            throw new Exception("280157 invalid image accepted");
        }
        void Test(string name, Action action) { action(); passed++; Console.WriteLine("PASS 280157 " + name); }
        async Task AsyncTest(string name, Func<Task> action) { await action(); passed++; Console.WriteLine("PASS 280157 " + name); }
        var target = DeviceProfile.F2800157;
        var image = FirmwareImage.Demo(target) with { Words = new ushort[64] };
        Test("192 KB minus 32 bytes partition and manifest identity", () =>
        {
            image.Validate(target);
            Assert(target.AppEnd - target.AppStart == 98288);
            Assert(BinaryPrimitives.ReadUInt32LittleEndian(image.Manifest()) == 0x28015701);
            (image with { Words = new ushort[98288] }).Validate(target);
            Reject(() => (image with { Words = new ushort[98292] }).Validate(target));
        });
        Test("selected model and legacy model cannot be mixed", () =>
        {
            Reject(() => image.Validate(DeviceProfile.F280039C));
            Reject(() => FirmwareImage.Demo().Validate(target));
            Reject(() => (image with { DeviceId = 0 }).Validate());
        });
        Test("bootloader, RAM, entry and alignment rejected", () =>
        {
            foreach (uint address in new uint[] { 0x080000, 0x087c00, 0x8000, 0x09fff0 })
                Reject(() => (image with { StartWordAddress = address }).Validate());
            Reject(() => (image with { EntryWordAddress = 0x088002 }).Validate());
            Reject(() => (image with { Words = new ushort[63] }).Validate());
        });
        foreach (bool can in new[] { false, true })
        {
            await AsyncTest($"{(can ? "CAN" : "SCI")} full image, lost ACK, reset", async () =>
            {
                await using var wire = new SimulatedTransport(can, true, 0, target);
                var client = new UpdaterClient(wire);
                await client.UpdateAsync(image, null, CancellationToken.None);
                Assert(wire.Device.State == BootState.Valid && wire.Device.EraseCount == 1 && wire.Device.ProgramCount == 2 && wire.Device.CommitCount == 1);
                await client.ResetAsync(CancellationToken.None);
            });
            await AsyncTest($"{(can ? "CAN" : "SCI")} wrong device refused before erase", async () =>
            {
                await using var wire = new SimulatedTransport(can, false, 0);
                try { await new UpdaterClient(wire).UpdateAsync(image, null, CancellationToken.None); throw new Exception("Wrong model accepted"); }
                catch (InvalidDataException) { }
                Assert(wire.Device.EraseCount == 0);
            });
        }
        Test("wrong board, CPU and overlarge BEGIN cannot erase", () =>
        {
            foreach (var (field, value) in new[] { (0, Profile.DeviceId), (1, 0u), (2, 2u), (4, 98292u) })
            {
                var device = new BootSimulator(target);
                var manifest = image.Manifest();
                BinaryPrimitives.WriteUInt32LittleEndian(manifest.AsSpan(field * 4), value);
                Assert(device.Handle(new Packet(2, 1, 1, manifest)).Payload[0] == (byte)Status.BadImage);
                Assert(device.EraseCount == 0);
            }
        });
        foreach (bool verify in new[] { false, true })
            await AsyncTest(verify ? "CRC failure cannot commit" : "program failure cannot commit", async () =>
            {
                await using var wire = new SimulatedTransport(false, false, 0, target);
                wire.Device.FailVerify = verify; wire.Device.FailWrite = !verify;
                try { await new UpdaterClient(wire).UpdateAsync(image, null, CancellationToken.None); throw new Exception("Failure was ignored"); }
                catch (IOException) { }
                Assert(wire.Device.CommitCount == 0 && wire.Device.State == BootState.Failed);
            });
        string fw = Path.GetFullPath(firmwareDirectory);
        await AsyncTest("actual linked OUT equals TI export; model and RAM load address", async () =>
        {
            var loaded = await TiOutConverter.LoadAsync(Path.Combine(fw, "AVP32F00157_Application.out"), converter, target: target);
            var text = TiBootTable.Load(Path.Combine(fw, "AVP32F00157_Application.txt"), target: target);
            Assert(loaded.DeviceId == target.DeviceId && loaded.EntryWordAddress == target.AppStart && loaded.Bytes.SequenceEqual(text.Bytes));
            Assert(loaded.Words.Length > 4 && loaded.StartWordAddress + loaded.Words.Length <= target.AppEnd);
            await using var wire = new SimulatedTransport(true, true, 0, target);
            await new UpdaterClient(wire).UpdateAsync(loaded, null, CancellationToken.None);
            Assert(wire.Device.State == BootState.Valid);
        });
        await AsyncTest("actual bootloader OUT rejected", async () =>
        {
            try { await TiOutConverter.LoadAsync(Path.Combine(fw, "AVP32F00157_Bootloader.out"), converter, target: target); throw new Exception("Boot accepted"); }
            catch (InvalidDataException) { }
        });
        Test("JSON retains model and enforces selected target", () =>
        {
            string temp = Path.Combine(Path.GetTempPath(), Guid.NewGuid() + ".json");
            try { image.Save(temp); Assert(FirmwareImage.Load(temp, target).DeviceId == target.DeviceId); Reject(() => FirmwareImage.Load(temp, DeviceProfile.F280039C)); }
            finally { File.Delete(temp); }
        });
        return passed;
    }
}
