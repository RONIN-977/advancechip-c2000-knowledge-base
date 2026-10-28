using System.Diagnostics;
using Updater.Core;

namespace Updater.Desktop;

public sealed class Avp335SerialTransport : IAvp335Transport
{
    private readonly SerialConnection connection;
    private Avp335Parser? handshakeParser;
    private Stopwatch? handshakeWatch;
    private TimeSpan handshakeLastByte;
    public Avp335SerialTransport(string port) => connection = new(port, 115200);
    public async Task<Avp335Packet> WaitForBootAsync(Avp335Packet request, CancellationToken cancellation)
    {
        connection.SetReadWait(5);
        handshakeParser = new();
        handshakeWatch = Stopwatch.StartNew();
        handshakeLastByte = TimeSpan.Zero;
        try { return await Avp335PowerOnHandshake.WaitAsync(request, ExchangeAsync, cancellation); }
        finally
        {
            handshakeParser = null; handshakeWatch = null;
            connection.SetReadWait(50);
        }
    }
    public Task<Avp335Packet> ExchangeAsync(Avp335Packet request, TimeSpan timeout, CancellationToken cancellation) => Task.Run(() =>
    {
        cancellation.ThrowIfCancellationRequested();
        connection.Write(request.Encode());
        var watch = Stopwatch.StartNew();
        var parser = handshakeParser ?? new Avp335Parser();
        byte[] buffer = new byte[256];
        var lastByte = watch.Elapsed;
        while (watch.Elapsed < timeout)
        {
            cancellation.ThrowIfCancellationRequested();
            uint count = connection.Read(buffer);
            var elapsed = handshakeWatch?.Elapsed ?? watch.Elapsed;
            var previous = handshakeWatch is null ? lastByte : handshakeLastByte;
            if (elapsed - previous > TimeSpan.FromMilliseconds(200)) parser.Reset();
            if (count > 0)
            {
                lastByte = elapsed;
                if (handshakeWatch is not null) handshakeLastByte = elapsed;
            }
            for (int i = 0; i < count; i++)
                foreach (var reply in parser.Feed(buffer[i]))
                    if (reply.IsAck && reply.Command == request.Command && reply.Sequence == request.Sequence) return reply;
        }
        throw new TimeoutException("335 SCI 应答超时：检查 SCI-A GPIO28/RX、GPIO29/TX、共地、晶振版本和 115200 波特率。");
    }, cancellation);
    public ValueTask DisposeAsync() => connection.DisposeAsync();
}
