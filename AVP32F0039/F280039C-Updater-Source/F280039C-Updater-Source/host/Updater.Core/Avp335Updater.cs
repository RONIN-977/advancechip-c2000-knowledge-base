using System.Buffers.Binary;

namespace Updater.Core;

public sealed record Avp335DeviceInfo(bool InBoot, uint BootVersion, uint AppVersion);

public sealed class Avp335UpdaterClient(IAvp335Transport transport)
{
    private ushort sequence;
    public async Task<Avp335DeviceInfo> ProbeAsync(CancellationToken cancellation)
    {
        var reply = await Send(0x01, [], cancellation);
        if (reply.Payload.Length != 26) throw new InvalidDataException("335 版本应答长度错误。");
        CheckStatus(reply);
        return new(reply.IsBoot, BinaryPrimitives.ReadUInt32LittleEndian(reply.Payload.AsSpan(18)), BinaryPrimitives.ReadUInt32LittleEndian(reply.Payload.AsSpan(22)));
    }
    public async Task EnterBootAsync(CancellationToken cancellation)
    {
        CheckStatus(await Send(0x72, [], cancellation));
        // APP drains its acknowledgement before disabling interrupts and jumping.
        for (int attempt = 0; attempt < 15; attempt++)
        {
            await Task.Delay(200, cancellation);
            try { if ((await ProbeAsync(cancellation)).InBoot) return; }
            catch (TimeoutException) when (attempt < 14) { }
        }
        throw new TimeoutException("335 未进入 BOOT；请确认 SCI-A 接线及配套 APP，或复位后重试。");
    }
    public async Task ResetAsync(CancellationToken cancellation)
    {
        var info = await ProbeAsync(cancellation);
        CheckStatus(await Send(info.InBoot ? (ushort)0x75 : (ushort)0x02, [], cancellation));
    }
    public async Task UpdateAsync(Avp335FirmwareImage image, IProgress<UpdateProgress>? progress, CancellationToken cancellation)
    {
        image.Validate(); cancellation.ThrowIfCancellationRequested();
        var info = await ProbeAsync(cancellation);
        if (!info.InBoot) { await EnterBootAsync(cancellation); info = await ProbeAsync(cancellation); }
        if (!info.InBoot || info.BootVersion != Avp335Profile.BootVersion)
            throw new InvalidDataException($"需要 335 V201 / 448 KB BOOT，当前版本 {info.BootVersion}；未开始擦写。");
        CheckStatus(await Send(0x72, [], cancellation));
        byte[] size = new byte[4]; BinaryPrimitives.WriteUInt32LittleEndian(size, (uint)image.Bytes.Length);
        progress?.Report(new(0, "335 BOOT V201 已匹配，正在擦除 APP 区"));
        var begin = await Send(0x70, size, cancellation); CheckStatus(begin);
        if (!begin.IsBoot || begin.Payload.Length != 3 || BinaryPrimitives.ReadUInt16LittleEndian(begin.Payload.AsSpan(1)) != Avp335Profile.PacketBytes)
            throw new InvalidDataException("335 分包容量应答不匹配。");
        for (int offset = 0, index = 0; offset < image.Bytes.Length; index++)
        {
            cancellation.ThrowIfCancellationRequested();
            int count = Math.Min(Avp335Profile.PacketBytes, image.Bytes.Length - offset);
            byte[] payload = new byte[6 + count];
            BinaryPrimitives.WriteUInt32LittleEndian(payload, (uint)index);
            BinaryPrimitives.WriteUInt16LittleEndian(payload.AsSpan(4), (ushort)count);
            Array.Copy(image.Bytes, offset, payload, 6, count);
            var reply = await Send(0x73, payload, cancellation); CheckStatus(reply);
            if (!reply.IsBoot || reply.Payload.Length != 5 || BinaryPrimitives.ReadUInt32LittleEndian(reply.Payload.AsSpan(1)) != index)
                throw new InvalidDataException("335 写入应答的包索引不匹配。");
            offset += count;
            progress?.Report(new(offset * 99 / image.Bytes.Length, $"335 已写入并回读 {offset:N0} / {image.Bytes.Length:N0} 字节"));
        }
        var finish = await Send(0x74, image.Digest, cancellation); CheckStatus(finish);
        if (!finish.IsBoot || finish.Payload.Length != 1) throw new InvalidDataException("335 结束应答的 BOOT 状态或长度不匹配。");
        progress?.Report(new(100, "335 整包 MD5 校验通过，有效标志已提交"));
    }
    private async Task<Avp335Packet> Send(ushort command, byte[] payload, CancellationToken cancellation)
    {
        var request = new Avp335Packet(command, ++sequence, payload);
        for (int attempt = 0; ; attempt++)
        {
            try
            {
                var reply = await transport.ExchangeAsync(request, TimeSpan.FromSeconds(command == 0x70 ? 120 : command == 0x74 ? 15 : 3), cancellation);
                if (!reply.IsAck || reply.Command != command || reply.Sequence != request.Sequence || reply.Payload.Length == 0)
                    throw new InvalidDataException("335 应答不属于当前请求。");
                // Allow the MCU main loop to finish TX and re-arm RX before the next request.
                await Task.Delay(5, CancellationToken.None);
                return reply;
            }
            catch (TimeoutException) when (attempt < 2) { cancellation.ThrowIfCancellationRequested(); }
        }
    }
    private static void CheckStatus(Avp335Packet reply)
    {
        if (reply.Payload[0] != 0) throw new IOException($"335 命令 0x{reply.Command:X2} 失败，错误码 {reply.Payload[0]}。");
    }
}
