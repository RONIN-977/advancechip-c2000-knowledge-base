using System.Buffers.Binary;

namespace Updater.Core;

// In-memory model. It does not open COM ports, load adapter DLLs, or access real hardware.
public sealed class BootSimulator
{
    public BootState State { get; private set; }
    public int EraseCount { get; private set; }
    public int ProgramCount { get; private set; }
    public int CommitCount { get; private set; }
    public bool FailVerify { get; set; }
    public bool FailWrite { get; set; }
    private uint session, sequence, next = Profile.AppStart, end, crc;
    private byte[] flash = [];
    private byte[]? cachedRequest;
    private Packet? cachedReply;
    public Packet Handle(Packet request)
    {
        byte[] encoded = request.Encode();
        if (cachedRequest is not null && encoded.AsSpan().SequenceEqual(cachedRequest)) return cachedReply!;
        Status status;
        bool accepted = false;
        switch ((Command)request.Command)
        {
            case Command.Info:
                if (request.Payload.Length != 0 || request.Session != 0 || request.Sequence != 0) return Reply(request, Status.BadLength);
                return Reply(request, Status.Ok, info: true);
            case Command.Reset:
                return Reply(request, request.Payload.Length == 0 && State == BootState.Valid ? Status.Ok : Status.BadState);
            case Command.EnterBoot:
                return Reply(request, request.Payload.Length == 0 && request.Session == 0 && request.Sequence == 0 ? Status.Ok : Status.BadLength);
            case Command.Begin:
                if (request.Payload.Length != 32) return Reply(request, Status.BadLength);
                if (State == BootState.Receiving) return Reply(request, Status.Busy);
                if (request.Session == 0 || request.Session == session || request.Sequence != 1) return Reply(request, Status.BadSequence);
                uint Field(int index) => BinaryPrimitives.ReadUInt32LittleEndian(request.Payload.AsSpan(index * 4));
                uint count = Field(4);
                if (Field(0) != Profile.DeviceId || Field(1) != Profile.BoardId || Field(2) != Profile.Cpu ||
                    Field(3) != Profile.AppStart || count == 0 || count % 4 != 0 || count > Profile.AppEnd - Profile.AppStart || Field(5) != Profile.AppStart)
                    return Reply(request, Status.BadImage);
                session = request.Session; sequence = request.Sequence;
                next = Profile.AppStart; end = next + count; crc = Field(6);
                flash = Enumerable.Repeat((byte)0xff, checked((int)count * 2)).ToArray();
                EraseCount++; State = BootState.Receiving; status = Status.Ok; accepted = true;
                break;
            case Command.Data:
            case Command.Finish:
            case Command.Abort:
                if (State != BootState.Receiving || request.Session != session) return Reply(request, Status.BadState);
                if (request.Command == (byte)Command.Abort ? request.Sequence <= sequence : request.Sequence != sequence + 1) return Reply(request, Status.BadSequence);
                if ((Command)request.Command == Command.Data)
                {
                    int countWords = (request.Payload.Length - 4) / 2;
                    if (request.Payload.Length < 12 || (request.Payload.Length - 4) % 8 != 0) return Reply(request, Status.BadLength);
                    if (BinaryPrimitives.ReadUInt32LittleEndian(request.Payload) != next || (uint)countWords > end - next)
                        return Reply(request, Status.BadImage);
                    if (FailWrite) { State = BootState.Failed; status = Status.FlashError; }
                    else
                    {
                        Array.Copy(request.Payload, 4, flash, (next - Profile.AppStart) * 2, countWords * 2);
                        next += (uint)countWords; ProgramCount++; status = Status.Ok;
                    }
                }
                else
                {
                    if (request.Payload.Length != 0) return Reply(request, Status.BadLength);
                    if ((Command)request.Command == Command.Abort) { State = BootState.Idle; status = Status.Ok; }
                    else
                    {
                        if (next != end) return Reply(request, Status.BadState);
                        if (FailVerify || Crc32.Compute(flash) != crc) { State = BootState.Failed; status = Status.VerifyError; }
                        else { State = BootState.Valid; CommitCount++; status = Status.Ok; }
                    }
                }
                sequence = request.Sequence; accepted = true;
                break;
            default: return Reply(request, Status.BadCommand);
        }
        var reply = Reply(request, status);
        if (accepted) { cachedRequest = encoded; cachedReply = reply; }
        return reply;
    }
    private Packet Reply(Packet request, Status status, bool info = false)
    {
        byte[] payload = new byte[info ? 24 : 6];
        payload[0] = (byte)status; payload[1] = (byte)State;
        BinaryPrimitives.WriteUInt32LittleEndian(payload.AsSpan(2), next);
        if (info)
        {
            BinaryPrimitives.WriteUInt32LittleEndian(payload.AsSpan(6), Profile.DeviceId);
            BinaryPrimitives.WriteUInt32LittleEndian(payload.AsSpan(10), Profile.BoardId);
            BinaryPrimitives.WriteUInt16LittleEndian(payload.AsSpan(14), Profile.Cpu);
            BinaryPrimitives.WriteUInt32LittleEndian(payload.AsSpan(16), Profile.AppStart);
            BinaryPrimitives.WriteUInt32LittleEndian(payload.AsSpan(20), Profile.AppEnd);
        }
        return new Packet((byte)(request.Command | 0x80), request.Session, request.Sequence, payload);
    }
}

public sealed class SimulatedTransport(bool can, bool dropOneAck = false, int latencyMs = 8) : ITransport
{
    public BootSimulator Device { get; } = new();
    private byte transfer;
    private bool dropped;
    public async Task<Packet> ExchangeAsync(Packet request, TimeSpan timeout, CancellationToken cancellation)
    {
        await Task.Delay(latencyMs, cancellation);
        var inbound = PassWire(request);
        var reply = Device.Handle(inbound);
        if (dropOneAck && !dropped && request.Command == (byte)Command.Data)
        {
            dropped = true;
            throw new TimeoutException("模拟已写入但 ACK 丢失。");
        }
        return PassWire(reply);
    }
    private Packet PassWire(Packet packet)
    {
        var bytes = packet.Encode();
        Packet? result = null;
        if (can)
        {
            var decoder = new CanReassembler();
            foreach (var frame in CanFraming.Fragment(bytes, transfer++)) result = decoder.Feed(frame) ?? result;
        }
        else
        {
            var decoder = new SciParser();
            foreach (byte value in bytes) foreach (var decoded in decoder.Feed(value)) result = decoded;
        }
        return result ?? throw new InvalidDataException("模拟通信帧无法组装。");
    }
    public ValueTask DisposeAsync() => ValueTask.CompletedTask;
}
