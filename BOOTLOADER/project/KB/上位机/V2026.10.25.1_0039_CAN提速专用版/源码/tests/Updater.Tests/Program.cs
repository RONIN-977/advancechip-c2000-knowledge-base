using System.Buffers.Binary;
using System.Text;
using Updater.Core;

if (args.Length > 0 && args[0] == "--custom-address") { await CustomAddressTests.Run(); return; }
if (args.Length > 0 && args[0] == "--335-handshake") { await Avp335HandshakeTests.Run(); return; }

if (args.Length >= 4 && args[0] == "--f2800157")
{
    string firmwareDirectory = args.Length >= 5 ? Path.GetFullPath(args[4])
        : Path.GetFullPath(Path.Combine(args[1], "../../../AVP32F00157/output"));
    int count = await F2800157Tests.Run(firmwareDirectory, args[2]);
    count += await F2800157NativeTests.Run(args[3], Path.Combine(firmwareDirectory, "AVP32F00157_Application.out"), args[2]);
    Console.WriteLine($"All {count} F2800157 tests passed.");
    return;
}

if (args.Length >= 3 && args[0] == "--f280035")
{
    int count = await F280035Tests.Run(args[1], args[2]);
    if (args.Length > 3)
        count += await F280035NativeTests.Run(args[3], Path.GetFullPath(Path.Combine(args[1], "../../../../../../AVP32F035/output/AVP32F035_Application.out")), args[2]);
    Console.WriteLine($"All {count} F280035 tests passed.");
    return;
}

if (args.Length >= 3 && args[0] == "--f280049")
{
    int count = await F280049Tests.Run(args[1], args[2]);
    if (args.Length > 3)
        count += await F280049NativeTests.Run(args[3], Path.GetFullPath(Path.Combine(args[1], "../../../../F280049_Bootloader/build/xtal20/application.out")), args[2]);
    Console.WriteLine($"All {count} F280049 tests passed.");
    return;
}

int passed = 0;
void Test(string name, Action body) { body(); passed++; Console.WriteLine($"PASS {name}"); }
async Task AsyncTest(string name, Func<Task> body) { await body(); passed++; Console.WriteLine($"PASS {name}"); }
void Assert(bool condition, string message = "Assertion failed") { if (!condition) throw new Exception(message); }
void Reject(Action action) { try { action(); } catch (InvalidDataException) { return; } throw new Exception("Invalid input was accepted"); }
var image = FirmwareImage.Demo();
Test("CRC32 ISO-HDLC check vector", () => Assert(Crc32.Compute(Encoding.ASCII.GetBytes("123456789")) == 0xcbf43926));
Test("frame roundtrip and every single-bit corruption", () => {
    var packet = new Packet(3, 0x12345678, 25, Enumerable.Range(0, 128).Select(x => (byte)x).ToArray());
    byte[] wire = packet.Encode();
    Assert(Packet.Decode(wire).Encode().SequenceEqual(wire));
    for (int i = 0; i < wire.Length; i++) for (int bit = 0; bit < 8; bit++) { byte[] bad = (byte[])wire.Clone(); bad[i] ^= (byte)(1 << bit); Reject(() => Packet.Decode(bad)); }
});
Test("SCI noise, corrupt frame, back-to-back recovery", () => {
    var packet = new Packet(1, 0, 0, []).Encode();
    byte[] corrupt = (byte[])packet.Clone(); corrupt[^1] ^= 1;
    var parser = new SciParser(); var result = new List<Packet>();
    foreach (byte b in new byte[] { 0x00, 0xa5, 0x10 }.Concat(corrupt).Concat(packet).Concat(packet)) result.AddRange(parser.Feed(b));
    Assert(result.Count == 2);
    foreach (byte b in packet.Take(8)) _ = parser.Feed(b).ToArray();
    parser.Reset(); result.Clear(); foreach (byte b in packet) result.AddRange(parser.Feed(b)); Assert(result.Count == 1);
});
Test("CAN all payload sizes and lost/reordered fragments", () => {
    for (int size = 0; size <= 128; size++) {
        var packet = new Packet(3, 2, 5, new byte[size]); var frames = CanFraming.Fragment(packet.Encode(), 17).ToArray();
        var decoder = new CanReassembler(); Packet? result = null;
        foreach (var frame in frames) { Assert(frame.Length <= 8); result = decoder.Feed(frame) ?? result; }
        Assert(result is not null && result.Encode().SequenceEqual(packet.Encode()));
        decoder.Reset(); result = null;
        foreach (var frame in frames.Where((_, i) => i != 1)) result = decoder.Feed(frame) ?? result;
        Assert(result is null);
    }
});
Test("image boundaries and CPU identity", () => {
    Reject(() => (image with { StartWordAddress = 0x80000 }).Validate());
    Reject(() => (image with { Cpu = 2 }).Validate());
    Reject(() => (image with { BoardId = 0 }).Validate());
    Reject(() => (image with { Words = new ushort[3] }).Validate());
    Reject(() => (image with { Words = new ushort[checked((int)(Profile.AppEnd - Profile.AppStart)) + 4] }).Validate());
});
Test("BEGIN and DATA retry never repeat flash operations", () => {
    var target = new BootSimulator(); var begin = new Packet(2, 9, 1, image.Manifest());
    Assert(target.Handle(begin).Payload[0] == 0); Assert(target.Handle(begin).Payload[0] == 0); Assert(target.EraseCount == 1);
    byte[] data = new byte[12]; BinaryPrimitives.WriteUInt32LittleEndian(data, Profile.AppStart); Array.Copy(image.Bytes, 0, data, 4, 8);
    var write = new Packet(3, 9, 2, data); var ack = target.Handle(write);
    Assert(ack.Payload[0] == 0 && target.Handle(write).Encode().SequenceEqual(ack.Encode()) && target.ProgramCount == 1);
    data = (byte[])data.Clone(); data[4] ^= 1;
    Assert(target.Handle(write with { Payload = data }).Payload[0] == (byte)Status.BadSequence);
    Assert(target.Handle(new Packet(4, 9, 3, [])).Payload[0] == (byte)Status.BadState);
});
Test("out-of-order and foreign-session writes rejected", () => {
    var target = new BootSimulator(); target.Handle(new Packet(2, 42, 1, image.Manifest()));
    byte[] data = new byte[12]; BinaryPrimitives.WriteUInt32LittleEndian(data, Profile.AppStart);
    Assert(target.Handle(new Packet(3, 43, 2, data)).Payload[0] == (byte)Status.BadState);
    Assert(target.Handle(new Packet(3, 42, 5, data)).Payload[0] == (byte)Status.BadSequence);
    BinaryPrimitives.WriteUInt32LittleEndian(data, Profile.AppStart + 4);
    Assert(target.Handle(new Packet(3, 42, 2, data)).Payload[0] == (byte)Status.BadImage && target.ProgramCount == 0);
});
Test("abort can skip an uncertain sequence; next session can restart", () => {
    var target = new BootSimulator(); target.Handle(new Packet(2, 42, 1, image.Manifest()));
    Assert(target.Handle(new Packet(5, 42, 3, [])).Payload[0] == 0 && target.State == BootState.Idle);
    Assert(target.Handle(new Packet(2, 43, 1, image.Manifest())).Payload[0] == 0);
});
await AsyncTest("enter boot handshake", async () => {
    await using var transport = new SimulatedTransport(false, false, 0);
    await new UpdaterClient(transport).EnterBootAsync(CancellationToken.None);
    Assert(transport.Device.EraseCount == 0);
});
foreach (bool can in new[] { false, true }) await AsyncTest($"full {(can ? "CAN" : "SCI")} upgrade with lost ACK", async () => {
    await using var transport = new SimulatedTransport(can, true, 0);
    await new UpdaterClient(transport).UpdateAsync(image, null, CancellationToken.None);
    Assert(transport.Device.State == BootState.Valid && transport.Device.EraseCount == 1 && transport.Device.CommitCount == 1);
    Assert(transport.Device.ProgramCount == (image.Words.Length + 59) / 60);
});
await AsyncTest("verification failure cannot commit", async () => {
    await using var transport = new SimulatedTransport(true, false, 0); transport.Device.FailVerify = true;
    try { await new UpdaterClient(transport).UpdateAsync(image, null, CancellationToken.None); throw new Exception("Expected rejection"); } catch (IOException) { }
    Assert(transport.Device.CommitCount == 0 && transport.Device.State == BootState.Failed);
});
await AsyncTest("program failure cannot commit", async () => {
    await using var transport = new SimulatedTransport(false, false, 0); transport.Device.FailWrite = true;
    try { await new UpdaterClient(transport).UpdateAsync(image, null, CancellationToken.None); throw new Exception("Expected rejection"); } catch (IOException) { }
    Assert(transport.Device.CommitCount == 0 && transport.Device.State == BootState.Failed);
});
await AsyncTest("cancel before begin does not erase", async () => {
    await using var transport = new SimulatedTransport(false, false, 0); using var cancel = new CancellationTokenSource(); cancel.Cancel();
    try { await new UpdaterClient(transport).UpdateAsync(image, null, cancel.Token); throw new Exception("Expected cancellation"); } catch (OperationCanceledException) { }
    Assert(transport.Device.EraseCount == 0);
});
string Table(params (uint address, ushort[] words)[] segments)
{
    var bytes = new List<byte>();
    void Word(ushort v) { bytes.Add((byte)v); bytes.Add((byte)(v >> 8)); }
    void Address(uint v) { Word((ushort)(v >> 16)); Word((ushort)v); }
    Word(0x08aa); for (int i = 0; i < 8; i++) Word(0); Address(Profile.AppStart);
    foreach (var (address, words) in segments) { Word((ushort)words.Length); Address(address); foreach (ushort w in words) Word(w); }
    Word(0); return "\u0002\n" + string.Join(" ", bytes.Select(x => x.ToString("X2"))) + "\n\u0003";
}
Test("TI sparse sections merged before ECC padding", () => {
    var parsed = TiBootTable.Parse(Table((Profile.AppStart, new ushort[] { 1, 2 }), (Profile.AppStart + 3, new ushort[] { 4, 5 })));
    Assert(parsed.Words.SequenceEqual(new ushort[] { 1, 2, 0xffff, 4, 5, 0xffff, 0xffff, 0xffff }));
    Reject(() => TiBootTable.Parse(Table((Profile.AppStart, new ushort[] { 1, 2 }), (Profile.AppStart + 1, new ushort[] { 3 }))));
    Reject(() => TiBootTable.Parse(Table((0x80000, new ushort[] { 1, 2 }))));
    Reject(() => TiBootTable.Parse(Table((Profile.AppEnd - 1, new ushort[] { 1, 2 }))));
    Reject(() => TiBootTable.Parse("AA 08 00"));
});
if (args.Length != 0)
{
    Test("real hex2000 output parsed at relocated entry", () => {
        var parsed = TiBootTable.Load(Path.Combine(args[0], "build", "firmware", "application.txt"));
        Assert(parsed.EntryWordAddress == 0x90000 && parsed.Words.Length > 100 && !parsed.SimulationOnly);
    });
    var golden = new Packet(2, 0x12345678, 1, image.Manifest()).Encode();
    Directory.CreateDirectory(Path.Combine(args[0], "build", "tests"));
    File.WriteAllText(Path.Combine(args[0], "build", "tests", "begin-packet.hex"), Convert.ToHexString(golden));
}
if (args.Length > 1)
{
    string converter = args[1];
    string source = Path.Combine(args[0], "build", "firmware", "application.out");
    await AsyncTest("installed TI converter discovered and usable", async () =>
    {
        string found = Updater.Desktop.TiToolLocator.Find() ?? throw new Exception("TI converter not discovered");
        var converted = await TiOutConverter.LoadAsync(source, found);
        await using var transport = new SimulatedTransport(true, true, 0);
        await new UpdaterClient(transport).UpdateAsync(converted, null, CancellationToken.None);
        Assert(transport.Device.State == BootState.Valid && transport.Device.CommitCount == 1);
    });
    await AsyncTest("OUT conversion equals existing TI export and preserves source", async () =>
    {
        byte[] before = File.ReadAllBytes(source);
        var converted = await TiOutConverter.LoadAsync(source, converter);
        var expected = TiBootTable.Load(Path.ChangeExtension(source, ".txt"));
        Assert(converted.EntryWordAddress == expected.EntryWordAddress);
        Assert(converted.Words.SequenceEqual(expected.Words));
        Assert(Crc32.Compute(converted.Bytes) == Crc32.Compute(expected.Bytes));
        Assert(before.SequenceEqual(File.ReadAllBytes(source)));
    });
    await AsyncTest("OUT path with spaces and Chinese characters", async () =>
    {
        string directory = Path.Combine(Path.GetTempPath(), "升级测试 " + Guid.NewGuid().ToString("N"));
        Directory.CreateDirectory(directory);
        try
        {
            string path = Path.Combine(directory, "LED 应用.out");
            File.Copy(source, path);
            var converted = await TiOutConverter.LoadAsync(path, converter);
            Assert(converted.EntryWordAddress == Profile.AppStart && converted.Words.Length > 100);
        }
        finally { Directory.Delete(directory, true); }
    });
    await AsyncTest("OUT bootloader rejected by application partition validation", async () =>
    {
        try
        {
            await TiOutConverter.LoadAsync(Path.Combine(args[0], "build", "firmware", "bootloader.out"), converter);
            throw new Exception("Bootloader must not be accepted as an application");
        }
        catch (InvalidDataException) { }
    });
    await AsyncTest("OUT invalid input and missing converter rejected", async () =>
    {
        string path = Path.Combine(Path.GetTempPath(), Guid.NewGuid().ToString("N") + ".out");
        try
        {
            File.WriteAllText(path, "This is not a TI executable.");
            try
            {
                await TiOutConverter.LoadAsync(path, converter);
                throw new Exception("Invalid OUT was accepted");
            }
            catch (InvalidDataException) { }
            try
            {
                await TiOutConverter.LoadAsync(source, path + ".missing.exe");
                throw new Exception("Missing converter was accepted");
            }
            catch (FileNotFoundException) { }
        }
        finally { File.Delete(path); }
    });
    await AsyncTest("OUT cancellation before launch", async () =>
    {
        using var cancelled = new CancellationTokenSource();
        cancelled.Cancel();
        try
        {
            await TiOutConverter.LoadAsync(source, converter, cancelled.Token);
            throw new Exception("Expected cancellation");
        }
        catch (OperationCanceledException) { }
    });
}
passed += await Avp335Tests.Run(args.Length > 2 && !string.IsNullOrWhiteSpace(args[2]) ? args[2] : null);
if (args.Length > 1) passed += await F280049Tests.Run(args[0], args[1]);
if (args.Length > 3)
{
    string fw = Path.GetFullPath(Path.Combine(args[0], "../../../../F280049_Bootloader/build/xtal20/application.out"));
    passed += await F280049NativeTests.Run(args[3], fw, args[1]);
}
Console.WriteLine($"All {passed} tests passed.");
