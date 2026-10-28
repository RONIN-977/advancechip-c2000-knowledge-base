using System.Diagnostics;

namespace Updater.Core;

public static class Avp335PowerOnHandshake
{
    public static readonly TimeSpan Timeout = TimeSpan.FromSeconds(9);
    public static readonly TimeSpan Interval = TimeSpan.FromMilliseconds(50);

    public static async Task<Avp335Packet> WaitAsync(Avp335Packet request,
        Func<Avp335Packet, TimeSpan, CancellationToken, Task<Avp335Packet>> exchange,
        CancellationToken cancellation)
    {
        var watch = Stopwatch.StartNew();
        var nextSend = TimeSpan.Zero;
        while (watch.Elapsed < Timeout)
        {
            cancellation.ThrowIfCancellationRequested();
            var delay = nextSend - watch.Elapsed;
            var remaining = Timeout - watch.Elapsed;
            if (delay > TimeSpan.Zero && remaining > TimeSpan.Zero)
                await Task.Delay(delay < remaining ? delay : remaining, cancellation);
            remaining = Timeout - watch.Elapsed;
            if (remaining <= TimeSpan.Zero) break;
            nextSend += Interval;
            if (nextSend <= watch.Elapsed) nextSend = watch.Elapsed + Interval;
            var receiveTime = nextSend - watch.Elapsed;
            if (receiveTime <= TimeSpan.Zero) continue;
            try
            {
                var reply = await exchange(request, receiveTime < remaining ? receiveTime : remaining, cancellation);
                if (reply.IsAck && reply.IsBoot && reply.Command == request.Command &&
                    reply.Sequence == request.Sequence && reply.Payload.Length == 26 && reply.Payload[0] == 0)
                    return reply;
            }
            catch (TimeoutException) { cancellation.ThrowIfCancellationRequested(); }
        }
        cancellation.ThrowIfCancellationRequested();
        throw new TimeoutException("9 秒内未连接到 335 BOOT。请重新点击开始升级，并在等待期间给板子上电或复位。");
    }
}
