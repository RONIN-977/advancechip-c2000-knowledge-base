using System.Buffers.Binary;
using System.Security.Cryptography;
using System.Text;
using Updater.Core;

internal static class Avp335Tests
{
    private record Section(string Name, uint Load, byte[] Data, uint Run = 0, uint Flags = 0x20);
    private static void Assert(bool value) { if (!value) throw new Exception("335 assertion failed"); }
    private static void Reject(Action action)
    {
        try { action(); } catch (InvalidDataException) { return; }
        throw new Exception("335 invalid input accepted");
    }
    private static byte[] Coff(params Section[] sections)
    {
        var names = new List<byte>(new byte[4]);
        var offsets = new Dictionary<string, uint>();
        foreach (var s in sections.Where(s => s.Name.Length > 8))
        { offsets[s.Name] = (uint)names.Count; names.AddRange(Encoding.ASCII.GetBytes(s.Name + "\0")); }
        int raw = 50 + sections.Length * 48, symbols = raw + sections.Sum(s => s.Data.Length);
        byte[] file = new byte[symbols + names.Count];
        void W16(int p, ushort v) => BinaryPrimitives.WriteUInt16LittleEndian(file.AsSpan(p), v);
        void W32(int p, uint v) => BinaryPrimitives.WriteUInt32LittleEndian(file.AsSpan(p), v);
        W16(0, 0xc2); W16(2, (ushort)sections.Length); W16(16, 28); W16(18, 2); W16(20, 0x9d); W32(8, (uint)symbols);
        for (int i = 0; i < sections.Length; i++)
        {
            var s = sections[i]; int h = 50 + 48 * i;
            if (s.Name.Length > 8) W32(h + 4, offsets[s.Name]); else Encoding.ASCII.GetBytes(s.Name).CopyTo(file, h);
            W32(h + 8, s.Run == 0 ? s.Load : s.Run); W32(h + 12, s.Load); W32(h + 16, (uint)s.Data.Length / 2);
            W32(h + 20, (uint)raw); W32(h + 40, s.Flags); s.Data.CopyTo(file, raw); raw += s.Data.Length;
        }
        W32(symbols, (uint)names.Count); names.Skip(4).ToArray().CopyTo(file, symbols + 4); return file;
    }
    private static Section Entry => new("codestart", 0x300000, [0x76, 0, 2, 0x30]);
    private static Section Flag => new("BOOTFLAG", 0x337ffe, [0xaa, 0], Flags: 0x40);
    private sealed class ProgressAction(Action<UpdateProgress> action) : IProgress<UpdateProgress>
    { public void Report(UpdateProgress value) => action(value); }
    private sealed class BadReplyTransport(int fault) : IAvp335Transport
    {
        public Task<Avp335Packet> ExchangeAsync(Avp335Packet p, TimeSpan t, CancellationToken c) => Task.FromResult(
            new Avp335Packet(fault == 0 ? (ushort)2 : p.Command, fault == 1 ? (ushort)(p.Sequence + 1) : p.Sequence, new byte[26], fault == 2 ? (byte)1 : (byte)0x89));
        public ValueTask DisposeAsync() => ValueTask.CompletedTask;
    }
    public static async Task<int> Run(string? firmwareRoot)
    {
        int passed = 0;
        void Test(string name, Action body) { body(); passed++; Console.WriteLine("PASS 335 " + name); }
        async Task Async(string name, Func<Task> body) { await body(); passed++; Console.WriteLine("PASS 335 " + name); }
        Test("original CRC vectors and frame corruption", () =>
        {
            var bytes = Encoding.ASCII.GetBytes("123456789");
            Assert(Avp335Crc.Header(bytes) == 0xa1 && Avp335Crc.Frame(bytes) == 0xbb3d);
            byte[] wire = new Avp335Packet(0x73, 0x1234, new byte[134]).Encode();
            Assert(wire.Length == 145 && wire[0] == 0xcc && wire[4] == 1);
            Assert(Avp335Packet.Decode(wire).Encode().SequenceEqual(wire));
            for (int i = 0; i < wire.Length; i++) for (int b = 0; b < 8; b++)
            { var bad = (byte[])wire.Clone(); bad[i] ^= (byte)(1 << b); Reject(() => Avp335Packet.Decode(bad)); }
        });
        Test("SCI parser resynchronizes after noise and CRC errors", () =>
        {
            byte[] good = new Avp335Packet(0x01, 1, []).Encode(), bad = (byte[])good.Clone(); bad[^1] ^= 1;
            var parser = new Avp335Parser(); var result = new List<Avp335Packet>();
            foreach (byte b in new byte[] { 0, 0xcc, 0, 0 }.Concat(bad).Concat(good).Concat(good)) result.AddRange(parser.Feed(b));
            Assert(result.Count == 2); parser.Reset();
        });
        Test("COFF LOAD address, RAM function and FFFF gaps", () =>
        {
            var parsed = Avp335OutLoader.Parse(Coff(Entry, new("ramfuncs", 0x300010, [1, 2, 3, 4], 0x9000), Flag,
                new(".ebss", 0xc000, [0, 0], Flags: 0x80), new(".debug", 0, [0, 0], Flags: 0x10)));
            Assert(parsed.Bytes.Length == 36 && parsed.Bytes[4] == 255 && parsed.Bytes[32] == 1 && parsed.Bytes[35] == 4);
            Assert(parsed.Digest.SequenceEqual(MD5.HashData(parsed.Bytes)));
        });
        Test("COFF rejects overlap, missing entry, legacy flag and reserved ranges", () =>
        {
            Reject(() => Avp335OutLoader.Parse(Coff(Entry, Entry, Flag)));
            Reject(() => Avp335OutLoader.Parse(Coff(Flag)));
            Reject(() => Avp335OutLoader.Parse(Coff(Entry, Flag with { Load = 0x307ffe })));
            foreach (uint address in new uint[] { 0x2fffff, 0x338000, 0x33fff8, 0x380400, 0x9000, 0x337ffd })
                Reject(() => Avp335OutLoader.Parse(Coff(Entry, Flag, new(".text", address, [1, 2, 3, 4]))));
        });
        Test("COFF complete file and target checks", () =>
        {
            byte[] file = Coff(Entry, Flag);
            for (int length = 0; length < file.Length; length++) Reject(() => Avp335OutLoader.Parse(file[..length]));
            var wrong = (byte[])file.Clone(); wrong[20] ^= 1; Reject(() => Avp335OutLoader.Parse(wrong));
            wrong = (byte[])file.Clone(); BinaryPrimitives.WriteUInt32LittleEndian(wrong.AsSpan(70), 0); Reject(() => Avp335OutLoader.Parse(wrong));
            Reject(() => Avp335OutLoader.Parse(Encoding.ASCII.GetBytes("\x7fELF not COFF")));
        });
        Test("448 KB minus 4 bytes boundary and metadata excluded", () =>
        {
            var max = Avp335OutLoader.Parse(Coff(Entry, new(".text", 0x300002, new byte[Avp335Profile.MaxBytes - 4]), Flag));
            Assert(max.Bytes.Length == 458748 && max.Bytes.Length % 128 == 124);
            Reject(() => new Avp335FirmwareImage(new byte[458750]).Validate());
            Reject(() => new Avp335FirmwareImage(new byte[5]).Validate());
        });
        await Async("APP enters BOOT and upgrades with a short final packet and lost ACK", async () =>
        {
            await using var transport = new Avp335SimulatedTransport(true) { InBoot = false };
            var image = Avp335FirmwareImage.Demo();
            await new Avp335UpdaterClient(transport).UpdateAsync(image, null, default);
            Assert(transport.Valid && transport.CommitCount == 1 && transport.ProgramCount == 33 && transport.EraseCount == 1);
            Assert(transport.Flash.SequenceEqual(image.Bytes));
            await new Avp335UpdaterClient(transport).ResetAsync(default); Assert(!transport.InBoot);
        });
        await Async("maximum image including 124-byte final packet", async () =>
        {
            await using var transport = new Avp335SimulatedTransport();
            var image = new Avp335FirmwareImage(Enumerable.Range(0, Avp335Profile.MaxBytes).Select(x => (byte)x).ToArray());
            await new Avp335UpdaterClient(transport).UpdateAsync(image, null, default);
            Assert(transport.Valid && transport.ProgramCount == 3584 && transport.Flash.SequenceEqual(image.Bytes));
        });
        await Async("old 64 KB BOOT rejected before erase", async () =>
        {
            await using var transport = new Avp335SimulatedTransport { BootVersion = 200 };
            try { await new Avp335UpdaterClient(transport).UpdateAsync(Avp335FirmwareImage.Demo(), null, default); throw new Exception("old BOOT accepted"); }
            catch (InvalidDataException) { }
            Assert(transport.EraseCount == 0 && transport.ProgramCount == 0);
        });
        foreach (string fault in new[] { "erase", "program", "verify", "MD5", "index" })
            await Async(fault + " failure prevents commit", async () =>
            {
                await using var t = new Avp335SimulatedTransport { FailErase = fault == "erase", FailProgram = fault == "program", FailVerify = fault == "verify", FailDigest = fault == "MD5", WrongIndex = fault == "index" };
                try { await new Avp335UpdaterClient(t).UpdateAsync(Avp335FirmwareImage.Demo(), null, default); throw new Exception("fault accepted"); }
                catch (Exception ex) when (ex is IOException or InvalidDataException) { }
                Assert(t.CommitCount == 0 && !t.Valid && t.InBoot);
            });
        for (int fault = 0; fault < 3; fault++) await Async("ACK identity check " + fault, async () =>
        {
            await using var transport = new BadReplyTransport(fault);
            try { await new Avp335UpdaterClient(transport).ProbeAsync(default); throw new Exception("bad ACK accepted"); }
            catch (InvalidDataException) { }
        });
        await Async("cancel midway and upgrade again", async () =>
        {
            await using var transport = new Avp335SimulatedTransport(); using var cancel = new CancellationTokenSource();
            try { await new Avp335UpdaterClient(transport).UpdateAsync(Avp335FirmwareImage.Demo(), new ProgressAction(p => { if (p.Percent > 0) cancel.Cancel(); }), cancel.Token); throw new Exception("cancel ignored"); }
            catch (OperationCanceledException) { }
            Assert(transport.InBoot && !transport.Valid && transport.CommitCount == 0);
            await new Avp335UpdaterClient(transport).UpdateAsync(Avp335FirmwareImage.Demo(), null, default);
            Assert(transport.Valid && transport.EraseCount == 2);
        });
        if (firmwareRoot is not null) foreach (string crystal in new[] { "30MHz", "33.3MHz" })
        {
            Test("real " + crystal + " APP OUT and BOOT rejection", () =>
            {
                var app = Avp335OutLoader.Load(Path.Combine(firmwareRoot, "build", crystal, "APP", $"AVP32F335_APP_{crystal}.out"));
                Assert(app.Bytes.Length > 100 && !app.SimulationOnly);
                Reject(() => Avp335OutLoader.Load(Path.Combine(firmwareRoot, "build", crystal, "BOOT", $"AVP32F335_BOOT_{crystal}.out")));
            });
        }
        return passed;
    }
}
