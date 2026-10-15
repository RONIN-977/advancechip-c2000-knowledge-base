using System.Buffers.Binary;
using System.Security.Cryptography;

namespace Updater.Core;

public interface ITransport : IAsyncDisposable
{
    Task<Packet> ExchangeAsync(Packet request, TimeSpan timeout, CancellationToken cancellation);
}

public sealed record UpdateProgress(int Percent, string Message);

public sealed class UpdaterClient(ITransport transport)
{
    public async Task UpdateAsync(FirmwareImage image, IProgress<UpdateProgress>? progress, CancellationToken cancellation)
    {
        image.Validate();
        var info = await Send(new Packet((byte)Command.Info, 0, 0, []), cancellation);
        if (info.Payload.Length != 24 || BinaryPrimitives.ReadUInt32LittleEndian(info.Payload.AsSpan(6)) != image.DeviceId ||
            BinaryPrimitives.ReadUInt32LittleEndian(info.Payload.AsSpan(10)) != image.BoardId ||
            BinaryPrimitives.ReadUInt16LittleEndian(info.Payload.AsSpan(14)) != image.Cpu ||
            BinaryPrimitives.ReadUInt32LittleEndian(info.Payload.AsSpan(16)) != Profile.AppStart ||
            BinaryPrimitives.ReadUInt32LittleEndian(info.Payload.AsSpan(20)) != Profile.AppEnd)
            throw new InvalidDataException("设备身份或 Flash 分区不匹配，未开始擦写。");
        uint session;
        do { session = BinaryPrimitives.ReadUInt32LittleEndian(RandomNumberGenerator.GetBytes(4)); } while (session == 0);
        uint sequence = 1;
        progress?.Report(new(0, "校验设备信息，开始升级会话"));
        try
        {
            await Send(new Packet((byte)Command.Begin, session, sequence++, image.Manifest()), cancellation);
            byte[] bytes = image.Bytes;
            for (int offset = 0; offset < image.Words.Length;)
            {
                cancellation.ThrowIfCancellationRequested();
                int words = Math.Min(60, image.Words.Length - offset);
                byte[] data = new byte[4 + words * 2];
                BinaryPrimitives.WriteUInt32LittleEndian(data, image.StartWordAddress + (uint)offset);
                Array.Copy(bytes, offset * 2, data, 4, words * 2);
                var reply = await Send(new Packet((byte)Command.Data, session, sequence++, data), cancellation);
                offset += words;
                if (reply.Payload.Length != 6 || BinaryPrimitives.ReadUInt32LittleEndian(reply.Payload.AsSpan(2)) != image.StartWordAddress + (uint)offset)
                    throw new InvalidDataException("设备确认地址不匹配。");
                progress?.Report(new(offset * 99 / image.Words.Length, $"已写入 {offset:N0} / {image.Words.Length:N0} 个 16 位字"));
            }
            var committed = await Send(new Packet((byte)Command.Finish, session, sequence, []), cancellation);
            if (committed.Payload[1] != (byte)BootState.Valid || BinaryPrimitives.ReadUInt32LittleEndian(committed.Payload.AsSpan(2)) != image.StartWordAddress + image.Words.Length)
                throw new InvalidDataException("设备未确认有效镜像状态。");
            progress?.Report(new(100, "整包校验通过，镜像已提交，可复位启动应用"));
        }
        catch
        {
            // Abort is allowed to skip a sequence number: a canceled exchange may
            // have reached the target even when its acknowledgement was not read.
            try { await transport.ExchangeAsync(new Packet((byte)Command.Abort, session, sequence + 1, []), TimeSpan.FromSeconds(1), CancellationToken.None); }
            catch (Exception) { /* Boot session also expires after 30 seconds of inactivity. */ }
            throw;
        }
    }
    public Task<Packet> ResetAsync(CancellationToken cancellation) => Send(new Packet((byte)Command.Reset, 0, 0, []), cancellation);
    public async Task EnterBootAsync(CancellationToken cancellation)
    {
        await Send(new Packet((byte)Command.EnterBoot, 0, 0, []), cancellation);
        await Task.Delay(800, cancellation);
        await Send(new Packet((byte)Command.Info, 0, 0, []), cancellation);
    }
    private async Task<Packet> Send(Packet request, CancellationToken cancellation)
    {
        // Retransmit exactly the same request. The target caches the last full request and response.
        for (int attempt = 0; ; attempt++)
        {
            try
            {
                var reply = await transport.ExchangeAsync(request, TimeSpan.FromSeconds(request.Command == (byte)Command.Begin ? 120 : 3), cancellation);
                if (reply.Command != (request.Command | 0x80) || reply.Session != request.Session || reply.Sequence != request.Sequence || reply.Payload.Length < 6)
                    throw new InvalidDataException("应答不属于当前请求。");
                if (reply.Payload[0] != 0) throw new IOException($"设备拒绝命令 {(Command)request.Command}: {(Status)reply.Payload[0]}");
                return reply;
            }
            catch (TimeoutException) when (attempt < 2) { cancellation.ThrowIfCancellationRequested(); }
        }
    }
}
