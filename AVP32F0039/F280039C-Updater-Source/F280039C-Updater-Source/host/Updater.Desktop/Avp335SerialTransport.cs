using System.Diagnostics;
using Updater.Core;

namespace Updater.Desktop;

public sealed class Avp335SerialTransport : IAvp335Transport
{
    private readonly SerialConnection connection;
    public Avp335SerialTransport(string port) => connection = new(port, 115200);
    public Task<Avp335Packet> ExchangeAsync(Avp335Packet request, TimeSpan timeout, CancellationToken cancellation) => Task.Run(() =>
    {
        cancellation.ThrowIfCancellationRequested();
        connection.Write(request.Encode());
        var watch = Stopwatch.StartNew();
        var parser = new Avp335Parser();
        byte[] buffer = new byte[256];
        var lastByte = watch.Elapsed;
        while (watch.Elapsed < timeout)
        {
            cancellation.ThrowIfCancellationRequested();
            uint count = connection.Read(buffer);
            if (watch.Elapsed - lastByte > TimeSpan.FromMilliseconds(200)) parser.Reset();
            if (count > 0) lastByte = watch.Elapsed;
            for (int i = 0; i < count; i++)
                foreach (var reply in parser.Feed(buffer[i]))
                    if (reply.IsAck && reply.Command == request.Command && reply.Sequence == request.Sequence) return reply;
        }
        throw new TimeoutException("335 SCI 应答超时：检查 SCI-A GPIO28/RX、GPIO29/TX、共地、晶振版本和 115200 波特率。");
    }, cancellation);
    public ValueTask DisposeAsync() => connection.DisposeAsync();
}
