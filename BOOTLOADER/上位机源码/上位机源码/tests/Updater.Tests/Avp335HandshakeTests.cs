using System.Buffers.Binary;
using System.Diagnostics;
using System.Text.Json;
using Updater.Core;
using Updater.Desktop;

internal static class Avp335HandshakeTests
{
    private static void Assert(bool value, string message)
    { if (!value) throw new Exception(message); }

    internal static async Task Run()
    {
        var results = new List<object>();
        async Task Test(string name, Func<Task<object>> body)
        {
            var details = await body();
            results.Add(new { Name = name, Result = "passed", Details = details });
            Console.WriteLine("PASS " + name + " " + JsonSerializer.Serialize(details));
        }
        foreach (int powerOn in new[] { 0, 23, 117, 485, 1200, 8750 })
            await Test("500ms BOOT window / power-on " + powerOn + "ms", async () =>
            {
                var wire = SerialConnection.Wire = new TestSerialWire { BootAt = powerOn };
                await using var transport = new Avp335SerialTransport("COM1");
                var reply = await transport.WaitForBootAsync(new(0x01, 1, []), default);
                Assert(reply.IsBoot && wire.CapturedAt >= powerOn && wire.CapturedAt < powerOn + 500, "Missed BOOT window");
                Assert(wire.Requests[0].Time < 30, "First request was delayed");
                Assert(wire.ReadWaits.SequenceEqual(new uint[] { 5, 50 }), "Read wait not restored");
                return new { PowerOnMs = powerOn, CaptureMs = wire.CapturedAt, Sends = wire.Requests.Count, Intervals = wire.Intervals() };
            });
        await Test("APP replies wait for BOOT without APP jump request", async () =>
        {
            var wire = SerialConnection.Wire = new TestSerialWire { BootAt = 350, AppReplies = true };
            await using var transport = new Avp335SerialTransport("COM1");
            var reply = await transport.WaitForBootAsync(new(0x01, 1, []), default);
            Assert(reply.IsBoot && wire.CapturedAt >= 350 && wire.Requests.All(r => r.Packet.Command == 1), "APP jump requested");
            return new { wire.CapturedAt, Sends = wire.Requests.Count };
        });
        await Test("wrong sequence / CRC / ACK / status / length ignored", async () =>
        {
            var wire = SerialConnection.Wire = new TestSerialWire { BadFrames = true };
            await using var transport = new Avp335SerialTransport("COM1");
            var reply = await transport.WaitForBootAsync(new(0x01, 1, []), default);
            Assert(reply.IsBoot && wire.Requests.Count == 6, "Invalid response accepted");
            return new { Sends = wire.Requests.Count };
        });
        await Test("split response survives a resend and separate reads", async () =>
        {
            var wire = SerialConnection.Wire = new TestSerialWire { SplitResponse = true };
            await using var transport = new Avp335SerialTransport("COM1");
            var reply = await transport.WaitForBootAsync(new(0x01, 1, []), default);
            Assert(reply.IsBoot && wire.Requests.Count >= 2 && wire.Elapsed >= 75, "Split parser state lost");
            return new { ElapsedMs = wire.Elapsed, Sends = wire.Requests.Count };
        });
        await Test("cancel while waiting stops polling", async () =>
        {
            var wire = SerialConnection.Wire = new TestSerialWire { Silent = true };
            await using var transport = new Avp335SerialTransport("COM1");
            using var cancel = new CancellationTokenSource(180);
            try { await transport.WaitForBootAsync(new(0x01, 1, []), cancel.Token); throw new Exception("Cancel ignored"); }
            catch (OperationCanceledException) { }
            int sends = wire.Requests.Count;
            await Task.Delay(100);
            Assert(wire.Requests.Count == sends && wire.Elapsed < 450 && wire.ReadWaits.Last() == 50, "Polling continued");
            return new { ElapsedMs = wire.Elapsed, Sends = sends };
        });
        await Test("no response waits the full 9 seconds / approximately 50ms cadence", async () =>
        {
            var wire = SerialConnection.Wire = new TestSerialWire { Silent = true };
            await using var transport = new Avp335SerialTransport("COM1");
            try { await transport.WaitForBootAsync(new(0x01, 1, []), default); throw new Exception("Silence accepted"); }
            catch (TimeoutException ex) { Assert(ex.Message.Contains("9 秒"), "Missing recovery instruction"); }
            double elapsed = wire.Elapsed;
            var intervals = wire.Requests.Zip(wire.Requests.Skip(1), (a, b) => b.Time - a.Time).Order().ToArray();
            Assert(elapsed >= 8990 && elapsed < 9300 && intervals[intervals.Length / 2] is >= 40 and <= 65, "Deadline or cadence incorrect");
            Assert(intervals.Min() >= 20 && wire.Requests.All(r => r.Packet.Command == 1 && r.Packet.Sequence == 1), "Burst or different request");
            return new { ElapsedMs = elapsed, Sends = wire.Requests.Count, Intervals = wire.Intervals() };
        });
        await Test("old V200 BOOT rejected before erase without address query", async () =>
        {
            await using var simulator = new Avp335SimulatedTransport { BootVersion = 200 };
            try { await new Avp335UpdaterClient(simulator).UpdateAsync(Avp335FirmwareImage.Demo(), null, default, true); throw new Exception("V200 accepted"); }
            catch (InvalidDataException) { }
            Assert(simulator.EraseCount == 0 && simulator.ProgramCount == 0, "Incompatible BOOT erased");
            return new { EraseCount = simulator.EraseCount };
        });
        await Test("successful handshake stops fast polling / lost ACK / MD5 / automatic start", async () =>
        {
            var wire = SerialConnection.Wire = new TestSerialWire { BootAt = 117 };
            await using var transport = new Avp335SerialTransport("COM1");
            var client = new Avp335UpdaterClient(transport);
            var image = Avp335FirmwareImage.Demo();
            await client.UpdateAsync(image, null, default, true);
            await client.ResetAsync(default);
            Assert(wire.Simulator.CommitCount == 1 && !wire.Simulator.InBoot && wire.Simulator.Flash.SequenceEqual(image.Bytes), "Upgrade/start failed");
            Assert(wire.Requests.All(r => r.Packet.Command != 0x76), "Address query sent");
            var commands = wire.Requests.SkipWhile(r => r.Packet.Command == 1).Select(r => r.Packet.Command).ToArray();
            Assert(commands.TakeWhile(c => c != 0x74).All(c => c != 1) && commands[^1] == 0x75, "Fast polling continued after handshake");
            var first = wire.Requests.First(r => r.Packet.Command == 0x72);
            Assert(first.Time - wire.HandshakeDeliveredAt >= 4, "MCU ACK drain delay lost");
            Assert(wire.Requests.Count(r => r.Packet.Command == 0x73) == wire.Simulator.ProgramCount + 1, "Dropped ACK was not retried");
            return new { wire.Simulator.EraseCount, wire.Simulator.ProgramCount, wire.Simulator.CommitCount, AddressQuery = "absent", AppStarted = true };
        });
        Console.WriteLine("All " + results.Count + " handshake tests passed.");
    }
}

internal sealed class TestSerialWire
{
    private readonly Stopwatch watch = Stopwatch.StartNew();
    private readonly List<(double Time, byte[] Bytes)> pending = [];
    private bool splitQueued, upgrading;
    internal int BootAt { get; init; }
    internal bool Silent { get; init; }
    internal bool AppReplies { get; init; }
    internal bool BadFrames { get; init; }
    internal bool SplitResponse { get; init; }
    internal double CapturedAt { get; private set; } = -1;
    internal double HandshakeDeliveredAt { get; private set; }
    internal double Elapsed => watch.Elapsed.TotalMilliseconds;
    internal List<(double Time, Avp335Packet Packet)> Requests { get; } = [];
    internal List<uint> ReadWaits { get; } = [];
    internal Avp335SimulatedTransport Simulator { get; } = new(true);
    internal object Intervals()
    {
        var values = Requests.Zip(Requests.Skip(1), (a, b) => b.Time - a.Time).Order().ToArray();
        return values.Length == 0 ? new { Count = 0, Min = 0.0, Median = 0.0, Max = 0.0 } :
            new { Count = values.Length, Min = values[0], Median = values[values.Length / 2], Max = values[^1] };
    }
    internal void Write(byte[] bytes)
    {
        var request = Avp335Packet.Decode(bytes);
        double now = Elapsed;
        Requests.Add((now, request));
        if (Silent) return;
        if (request.Command != 1) upgrading = true;
        if (upgrading)
        {
            try { pending.Add((Elapsed, Simulator.ExchangeAsync(request, TimeSpan.FromSeconds(3), default).GetAwaiter().GetResult().Encode())); }
            catch (TimeoutException) { /* Drop the wire ACK; production retry waits for its ordinary timeout. */ }
            return;
        }
        bool inBoot = CapturedAt >= 0 || now >= BootAt && now < BootAt + 500;
        if (!inBoot && !AppReplies) return;
        byte[] payload = new byte[26];
        BinaryPrimitives.WriteUInt32LittleEndian(payload.AsSpan(18), Avp335Profile.BootVersion);
        var reply = new Avp335Packet(1, request.Sequence, payload, inBoot ? (byte)0x89 : (byte)0x81);
        byte[] frame = reply.Encode();
        if (BadFrames && Requests.Count <= 5)
        {
            switch (Requests.Count)
            {
                case 1: frame = (reply with { Sequence = (ushort)(request.Sequence + 1) }).Encode(); break;
                case 2: frame[^1] ^= 1; break;
                case 3: frame = (reply with { Flags = 9 }).Encode(); break;
                case 4: payload[0] = 1; frame = reply.Encode(); break;
                case 5: frame = (reply with { Payload = new byte[25] }).Encode(); break;
            }
            pending.Add((Elapsed, frame)); return;
        }
        if (inBoot && CapturedAt < 0) CapturedAt = now;
        if (SplitResponse)
        {
            if (splitQueued) return;
            splitQueued = true;
            pending.Add((now + 40, frame[..18])); pending.Add((now + 75, frame[18..]));
        }
        else pending.Add((Elapsed, frame));
    }
    internal uint Read(byte[] buffer, uint wait)
    {
        var timeout = Stopwatch.StartNew();
        do
        {
            int index = pending.FindIndex(p => p.Time <= Elapsed);
            if (index >= 0)
            {
                byte[] bytes = pending[index].Bytes; pending.RemoveAt(index);
                bytes.CopyTo(buffer, 0);
                if (!upgrading && CapturedAt >= 0) HandshakeDeliveredAt = Elapsed;
                return (uint)bytes.Length;
            }
            Thread.Sleep(1);
        } while (timeout.Elapsed.TotalMilliseconds < wait);
        return 0;
    }
}

namespace Updater.Desktop
{
    // Compile the production serial transport against a timed memory UART for software tests.
    internal sealed class SerialConnection : IAsyncDisposable
    {
        internal static TestSerialWire Wire { get; set; } = new();
        private uint wait = 50;
        public SerialConnection(string port, int baud) { }
        public void Write(byte[] data) => Wire.Write(data);
        public uint Read(byte[] buffer) => Wire.Read(buffer, wait);
        internal void SetReadWait(uint milliseconds) { wait = milliseconds; Wire.ReadWaits.Add(milliseconds); }
        public ValueTask DisposeAsync() => ValueTask.CompletedTask;
    }
}
