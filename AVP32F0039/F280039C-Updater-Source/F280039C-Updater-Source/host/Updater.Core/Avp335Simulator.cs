using System.Buffers.Binary;
using System.Security.Cryptography;

namespace Updater.Core;

public sealed class Avp335SimulatedTransport(bool dropAck = false) : IAvp335Transport
{
    public uint BootVersion { get; set; } = Avp335Profile.BootVersion;
    public bool InBoot { get; set; } = true;
    public bool Valid { get; private set; }
    public bool FailErase { get; set; }
    public bool FailProgram { get; set; }
    public bool FailVerify { get; set; }
    public bool FailDigest { get; set; }
    public bool WrongIndex { get; set; }
    public int EraseCount { get; private set; }
    public int ProgramCount { get; private set; }
    public int CommitCount { get; private set; }
    public byte[] Flash { get; private set; } = [];
    private int received;
    private byte[]? lastRequest;
    private Avp335Packet? lastReply;
    private bool dropped;
    public Task<Avp335Packet> ExchangeAsync(Avp335Packet request, TimeSpan timeout, CancellationToken cancellation)
    {
        cancellation.ThrowIfCancellationRequested();
        byte[] wire = request.Encode();
        if (lastRequest is not null && wire.SequenceEqual(lastRequest)) return Task.FromResult(lastReply!);
        byte[] payload = [0];
        switch (request.Command)
        {
            case 0x01:
                payload = new byte[26];
                BinaryPrimitives.WriteUInt32LittleEndian(payload.AsSpan(18), BootVersion);
                BinaryPrimitives.WriteUInt32LittleEndian(payload.AsSpan(22), Avp335Profile.BootVersion);
                break;
            case 0x72: InBoot = true; break;
            case 0x75: if (!Valid) payload[0] = 5; else InBoot = false; break;
            case 0x02: InBoot = false; break;
            case 0x70:
                payload = [0, 128, 0]; Valid = false;
                int size = checked((int)BinaryPrimitives.ReadUInt32LittleEndian(request.Payload));
                if (FailErase || !InBoot || size < 4 || size > Avp335Profile.MaxBytes || (size & 1) != 0) { payload[0] = 1; break; }
                Flash = Enumerable.Repeat((byte)0xff, size).ToArray(); received = 0; EraseCount++; break;
            case 0x73:
                payload = new byte[5];
                uint index = BinaryPrimitives.ReadUInt32LittleEndian(request.Payload);
                int length = BinaryPrimitives.ReadUInt16LittleEndian(request.Payload.AsSpan(4));
                BinaryPrimitives.WriteUInt32LittleEndian(payload.AsSpan(1), WrongIndex ? index + 1 : index);
                if (FailProgram || FailVerify || index * 128 != received || length != Math.Min(128, Flash.Length - received) || request.Payload.Length != length + 6)
                { payload[0] = 1; break; }
                Array.Copy(request.Payload, 6, Flash, received, length); received += length; ProgramCount++; break;
            case 0x74:
                if (FailDigest || received != Flash.Length || !MD5.HashData(Flash).SequenceEqual(request.Payload)) { payload[0] = 3; break; }
                Valid = true; CommitCount++; break;
            default: payload[0] = 5; break;
        }
        var reply = new Avp335Packet(request.Command, request.Sequence, payload, InBoot ? (byte)0x89 : (byte)0x81);
        // Exercise the same frame codec as the serial receiver.
        lastReply = Avp335Packet.Decode(reply.Encode()); lastRequest = wire;
        if (dropAck && !dropped && request.Command == 0x73)
        { dropped = true; throw new TimeoutException("335 仿真：丢弃一次写入 ACK"); }
        return Task.FromResult(lastReply);
    }
    public ValueTask DisposeAsync() => ValueTask.CompletedTask;
}
