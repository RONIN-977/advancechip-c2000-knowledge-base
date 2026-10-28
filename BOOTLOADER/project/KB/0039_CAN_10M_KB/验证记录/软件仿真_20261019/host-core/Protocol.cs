using System.Buffers.Binary;

namespace Updater.Core;

public enum Command : byte { Info = 1, Begin = 2, Data = 3, Finish = 4, Abort = 5, Reset = 6, EnterBoot = 7 }
public enum Status : byte { Ok, BadCommand, BadLength, BadImage, BadState, BadSequence, FlashError, VerifyError, Busy }
public enum BootState : byte { Idle, Receiving, Valid, Failed }

public static class Crc32
{
    public static uint Compute(ReadOnlySpan<byte> data)
    {
        uint crc = 0xffffffff;
        foreach (byte value in data)
        {
            crc ^= value;
            for (int i = 0; i < 8; i++) crc = (crc >> 1) ^ ((crc & 1) != 0 ? 0xedb88320u : 0u);
        }
        return ~crc;
    }
}

public sealed record Packet(byte Command, uint Session, uint Sequence, byte[] Payload)
{
    public const int MaxPayload = 128;
    public const int HeaderSize = 14;
    public const int MaxSize = HeaderSize + MaxPayload + 4;
    public byte[] Encode()
    {
        if (Payload.Length > MaxPayload) throw new ArgumentOutOfRangeException(nameof(Payload));
        byte[] data = new byte[HeaderSize + Payload.Length + 4];
        data[0] = 0xa5; data[1] = 0x5a; data[2] = 1; data[3] = Command;
        BinaryPrimitives.WriteUInt32LittleEndian(data.AsSpan(4), Session);
        BinaryPrimitives.WriteUInt32LittleEndian(data.AsSpan(8), Sequence);
        BinaryPrimitives.WriteUInt16LittleEndian(data.AsSpan(12), (ushort)Payload.Length);
        Payload.CopyTo(data, HeaderSize);
        BinaryPrimitives.WriteUInt32LittleEndian(data.AsSpan(data.Length - 4), Crc32.Compute(data.AsSpan(2, data.Length - 6)));
        return data;
    }
    public static Packet Decode(ReadOnlySpan<byte> data)
    {
        if (data.Length < HeaderSize + 4 || data[0] != 0xa5 || data[1] != 0x5a || data[2] != 1)
            throw new InvalidDataException("帧头或协议版本错误。");
        int length = BinaryPrimitives.ReadUInt16LittleEndian(data[12..]);
        if (length > MaxPayload || data.Length != HeaderSize + length + 4)
            throw new InvalidDataException("帧长度错误。");
        if (Crc32.Compute(data[2..^4]) != BinaryPrimitives.ReadUInt32LittleEndian(data[^4..]))
            throw new InvalidDataException("帧 CRC32 错误。");
        return new Packet(data[3], BinaryPrimitives.ReadUInt32LittleEndian(data[4..]),
            BinaryPrimitives.ReadUInt32LittleEndian(data[8..]), data.Slice(HeaderSize, length).ToArray());
    }
}

// Feed arbitrary SCI receive chunks. Call Reset on the inter-byte deadline (e.g. 200 ms).
public sealed class SciParser
{
    private readonly List<byte> buffer = [];
    public void Reset() => buffer.Clear();
    public IEnumerable<Packet> Feed(byte value)
    {
        buffer.Add(value);
        while (buffer.Count >= 2)
        {
            if (buffer[0] != 0xa5 || buffer[1] != 0x5a) { buffer.RemoveAt(0); continue; }
            if (buffer.Count < Packet.HeaderSize) yield break;
            int length = buffer[12] | (buffer[13] << 8);
            if (buffer[2] != 1 || length > Packet.MaxPayload) { buffer.RemoveAt(0); continue; }
            int total = Packet.HeaderSize + length + 4;
            if (buffer.Count < total) yield break;
            Packet? packet = null;
            try { packet = Packet.Decode(buffer.Take(total).ToArray()); }
            catch (InvalidDataException) { }
            if (packet is null) { buffer.RemoveAt(0); continue; }
            buffer.RemoveRange(0, total);
            yield return packet;
        }
    }
}

// Custom classic-CAN framing, not CANopen/ISO-TP/TI ROM CAN boot.
// Every frame: transfer ID, index (bit7=start), six stream octets; final frame may be short.
public static class CanFraming
{
    public static IEnumerable<byte[]> Fragment(byte[] packet, byte transferId)
    {
        if (packet.Length < 18 || packet.Length > Packet.MaxSize) throw new ArgumentOutOfRangeException(nameof(packet));
        for (int offset = 0, index = 0; offset < packet.Length; offset += 6, index++)
        {
            int count = Math.Min(6, packet.Length - offset);
            byte[] frame = new byte[count + 2];
            frame[0] = transferId; frame[1] = (byte)(index | (index == 0 ? 0x80 : 0));
            Array.Copy(packet, offset, frame, 2, count);
            yield return frame;
        }
    }
}

public sealed class CanReassembler
{
    private readonly List<byte> buffer = [];
    private byte transfer;
    private int next;
    private bool active;
    public void Reset() { active = false; next = 0; buffer.Clear(); }
    public Packet? Feed(ReadOnlySpan<byte> frame)
    {
        if (frame.Length is < 3 or > 8) { Reset(); return null; }
        int index = frame[1] & 0x7f;
        if ((frame[1] & 0x80) != 0)
        {
            Reset();
            if (index != 0) return null;
            active = true; transfer = frame[0];
        }
        if (!active || frame[0] != transfer || index != next) { Reset(); return null; }
        next++;
        foreach (byte b in frame[2..]) buffer.Add(b);
        if (buffer.Count > Packet.MaxSize) { Reset(); return null; }
        if (buffer.Count < Packet.HeaderSize) return null;
        int length = buffer[12] | (buffer[13] << 8);
        if (length > Packet.MaxPayload) { Reset(); return null; }
        int total = Packet.HeaderSize + length + 4;
        if (buffer.Count < total) return null;
        byte[] data = buffer.ToArray();
        Reset();
        try { return Packet.Decode(data); } catch (InvalidDataException) { return null; }
    }
}
