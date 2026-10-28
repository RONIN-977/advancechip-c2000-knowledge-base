using System.Buffers.Binary;

namespace Updater.Core;

public static class Avp335Crc
{
    public static byte Header(ReadOnlySpan<byte> bytes)
    {
        byte crc = 0;
        foreach (byte value in bytes)
        {
            crc ^= value;
            for (int bit = 0; bit < 8; bit++) crc = (byte)((crc >> 1) ^ ((crc & 1) != 0 ? 0x8c : 0));
        }
        return crc;
    }
    public static ushort Frame(ReadOnlySpan<byte> bytes)
    {
        ushort crc = 0; // The supplied SCI code uses zero, not Modbus's 0xffff initial value.
        foreach (byte value in bytes)
        {
            crc ^= value;
            for (int bit = 0; bit < 8; bit++) crc = (ushort)((crc >> 1) ^ ((crc & 1) != 0 ? 0xa001 : 0));
        }
        return crc;
    }
}

public sealed record Avp335Packet(ushort Command, ushort Sequence, byte[] Payload, byte Flags = 1)
{
    public bool IsAck => (Flags & 0x80) != 0;
    public bool IsBoot => (Flags & 8) != 0;
    public byte[] Encode()
    {
        if (Payload.Length > 149) throw new InvalidDataException("335 SCI 帧超过 160 字节。");
        byte[] data = new byte[11 + Payload.Length];
        data[0] = 0xcc;
        BinaryPrimitives.WriteUInt16LittleEndian(data.AsSpan(1), (ushort)(data.Length << 3));
        data[3] = Avp335Crc.Header(data.AsSpan(0, 3)); data[4] = Flags;
        BinaryPrimitives.WriteUInt16LittleEndian(data.AsSpan(5), Command);
        BinaryPrimitives.WriteUInt16LittleEndian(data.AsSpan(7), Sequence);
        Payload.CopyTo(data, 9);
        BinaryPrimitives.WriteUInt16LittleEndian(data.AsSpan(data.Length - 2), Avp335Crc.Frame(data.AsSpan(0, data.Length - 2)));
        return data;
    }
    public static Avp335Packet Decode(ReadOnlySpan<byte> data)
    {
        if (data.Length < 11 || data.Length > 160 || data[0] != 0xcc ||
            (BinaryPrimitives.ReadUInt16LittleEndian(data[1..]) >> 3) != data.Length ||
            data[3] != Avp335Crc.Header(data[..3]) ||
            BinaryPrimitives.ReadUInt16LittleEndian(data[^2..]) != Avp335Crc.Frame(data[..^2]))
            throw new InvalidDataException("335 SCI 帧长度或 CRC 校验失败。");
        return new(BinaryPrimitives.ReadUInt16LittleEndian(data[5..]), BinaryPrimitives.ReadUInt16LittleEndian(data[7..]), data[9..^2].ToArray(), data[4]);
    }
}

public sealed class Avp335Parser
{
    private readonly List<byte> buffer = [];
    public void Reset() => buffer.Clear();
    public IEnumerable<Avp335Packet> Feed(byte value)
    {
        buffer.Add(value);
        while (buffer.Count > 0)
        {
            if (buffer[0] != 0xcc) { buffer.RemoveAt(0); continue; }
            if (buffer.Count < 4) yield break;
            int length = (buffer[1] | buffer[2] << 8) >> 3;
            if (length < 11 || length > 160 || buffer[3] != Avp335Crc.Header(buffer.Take(3).ToArray()))
            { buffer.RemoveAt(0); continue; }
            if (buffer.Count < length) yield break;
            Avp335Packet? packet = null;
            try { packet = Avp335Packet.Decode(buffer.Take(length).ToArray()); }
            catch (InvalidDataException) { }
            if (packet is null) { buffer.RemoveAt(0); continue; }
            buffer.RemoveRange(0, length); yield return packet;
        }
    }
}

public interface IAvp335Transport : IAsyncDisposable
{
    Task<Avp335Packet> ExchangeAsync(Avp335Packet request, TimeSpan timeout, CancellationToken cancellation);
    Task<Avp335Packet> WaitForBootAsync(Avp335Packet request, CancellationToken cancellation) =>
        Avp335PowerOnHandshake.WaitAsync(request, ExchangeAsync, cancellation);
}
