using System.Buffers.Binary;
using System.Diagnostics;
using System.Security.Cryptography;

namespace Updater.Core;

public interface ITransport : IAsyncDisposable
{
    Task<Packet> ExchangeAsync(Packet request, TimeSpan timeout, CancellationToken cancellation);
}

public sealed record UpdateProgress(int Percent, string Message);

public sealed class UpdaterClient(ITransport transport, CommunicationTrace? trace = null)
{
    private int requests, attempts, retries, timeouts, responses, dataWords;
    public async Task UpdateAsync(FirmwareImage image, IProgress<UpdateProgress>? progress, CancellationToken cancellation)
    {
        requests = attempts = retries = timeouts = responses = dataWords = 0;
        long started = Stopwatch.GetTimestamp();
        string outcome = "success";
        trace?.Record("stage_start", new { stage = "update", imageWords = image.Words.Length, image.StartWordAddress, image.DeviceId, image.BoardId });
        try { await UpdateCoreAsync(image, progress, cancellation); }
        catch (Exception ex)
        {
            outcome = ex is OperationCanceledException ? "cancelled" : "failed";
            trace?.Record("update_failure", new { outcome, error = ex.GetType().Name, ex.Message });
            throw;
        }
        finally { trace?.Record("update_summary", new { outcome, duration_ms = Stopwatch.GetElapsedTime(started).TotalMilliseconds, requests, attempts, retries, timeouts, responses, acknowledgedWords = dataWords }); }
    }
    private async Task UpdateCoreAsync(FirmwareImage image, IProgress<UpdateProgress>? progress, CancellationToken cancellation)
    {
        image.Validate();
        var info = await Send(new Packet((byte)Command.Info, 0, 0, []), cancellation);
        if (info.Payload.Length != 24 || BinaryPrimitives.ReadUInt32LittleEndian(info.Payload.AsSpan(6)) != image.DeviceId ||
            BinaryPrimitives.ReadUInt32LittleEndian(info.Payload.AsSpan(10)) != image.BoardId ||
            BinaryPrimitives.ReadUInt16LittleEndian(info.Payload.AsSpan(14)) != image.Cpu)
            throw new InvalidDataException("设备型号、板号或 CPU 不匹配，未开始擦写。");
        uint deviceStart = BinaryPrimitives.ReadUInt32LittleEndian(info.Payload.AsSpan(16));
        uint deviceEnd = BinaryPrimitives.ReadUInt32LittleEndian(info.Payload.AsSpan(20));
        if (deviceStart != image.Target.AppStart || deviceEnd != image.Target.AppEnd)
            throw new InvalidDataException($"APP 分区不匹配，未开始擦写。主机：[0x{image.Target.AppStart:X6}, 0x{image.Target.AppEnd:X6})；设备：[0x{deviceStart:X6}, 0x{deviceEnd:X6})。地址单位为 16 位字，结束地址不包含，请核对 BOOT 配置。");
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
                dataWords += words;
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
            trace?.Record("abort_start", new { session, sequence = sequence + 1, timeout_ms = 1000 });
            try
            {
                var reply = await transport.ExchangeAsync(new Packet((byte)Command.Abort, session, sequence + 1, []), TimeSpan.FromSeconds(1), CancellationToken.None);
                trace?.Record("abort_response", new { reply.Session, reply.Sequence, status = reply.Payload.Length > 0 ? (int?)reply.Payload[0] : null });
            }
            catch (Exception ex) { trace?.Record("abort_failure", new { error = ex.GetType().Name, ex.Message }); /* Boot session also expires after 30 seconds of inactivity. */ }
            throw;
        }
    }
    public Task<Packet> ResetAsync(CancellationToken cancellation) => Send(new Packet((byte)Command.Reset, 0, 0, []), cancellation);
    public async Task EnterBootAsync(CancellationToken cancellation)
    {
        trace?.Record("stage_start", new { stage = "enter_boot" });
        await Send(new Packet((byte)Command.EnterBoot, 0, 0, []), cancellation);
        trace?.Record("enter_boot_wait", new { delay_ms = 800 });
        await Task.Delay(800, cancellation);
        await Send(new Packet((byte)Command.Info, 0, 0, []), cancellation);
        trace?.Record("stage_end", new { stage = "enter_boot" });
    }
    private async Task<Packet> Send(Packet request, CancellationToken cancellation)
    {
        requests++;
        long commandStarted = Stopwatch.GetTimestamp();
        string command = ((Command)request.Command).ToString();
        int timeoutMs = request.Command == (byte)Command.Begin ? 120000 : 3000;
        uint? writeAddress = request.Command == (byte)Command.Data && request.Payload.Length >= 4 ? BinaryPrimitives.ReadUInt32LittleEndian(request.Payload) : null;
        trace?.Record("command_start", new { command, request.Session, request.Sequence, payload_bytes = request.Payload.Length, writeAddress, timeout_ms = timeoutMs });
        // Retransmit exactly the same request. The target caches the last full request and response.
        for (int attempt = 0; ; attempt++)
        {
            attempts++;
            long started = Stopwatch.GetTimestamp();
            trace?.Record("exchange_start", new { command, request.Session, request.Sequence, attempt = attempt + 1, timeout_ms = timeoutMs });
            try
            {
                var reply = await transport.ExchangeAsync(request, TimeSpan.FromSeconds(request.Command == (byte)Command.Begin ? 120 : 3), cancellation);
                responses++;
                trace?.Record("exchange_response", new {
                    command, request.Session, request.Sequence, attempt = attempt + 1,
                    elapsed_ms = Stopwatch.GetElapsedTime(started).TotalMilliseconds,
                    responseCommand = reply.Command, responseSession = reply.Session, responseSequence = reply.Sequence,
                    payload_bytes = reply.Payload.Length,
                    status = reply.Payload.Length > 0 ? ((Status)reply.Payload[0]).ToString() : null,
                    state = reply.Payload.Length > 1 ? ((BootState)reply.Payload[1]).ToString() : null,
                    confirmedAddress = reply.Payload.Length == 6 ? (uint?)BinaryPrimitives.ReadUInt32LittleEndian(reply.Payload.AsSpan(2)) : null
                });
                if (reply.Command != (request.Command | 0x80) || reply.Session != request.Session || reply.Sequence != request.Sequence || reply.Payload.Length < 6)
                    throw new InvalidDataException("应答不属于当前请求。");
                if (reply.Payload[0] != 0) throw new IOException($"设备拒绝命令 {(Command)request.Command}: {(Status)reply.Payload[0]}");
                trace?.Record("command_end", new { command, request.Session, request.Sequence, attempts = attempt + 1, elapsed_ms = Stopwatch.GetElapsedTime(commandStarted).TotalMilliseconds });
                return reply;
            }
            catch (TimeoutException) when (attempt < 2)
            {
                timeouts++;
                trace?.Record("exchange_timeout", new { command, request.Session, request.Sequence, attempt = attempt + 1, elapsed_ms = Stopwatch.GetElapsedTime(started).TotalMilliseconds });
                cancellation.ThrowIfCancellationRequested();
                retries++;
                trace?.Record("exchange_retry", new { command, request.Session, request.Sequence, next_attempt = attempt + 2 });
            }
            catch (Exception ex)
            {
                if (ex is TimeoutException) timeouts++;
                trace?.Record("command_failure", new { command, request.Session, request.Sequence, attempt = attempt + 1, elapsed_ms = Stopwatch.GetElapsedTime(started).TotalMilliseconds, error = ex.GetType().Name, ex.Message });
                throw;
            }
        }
    }
}
