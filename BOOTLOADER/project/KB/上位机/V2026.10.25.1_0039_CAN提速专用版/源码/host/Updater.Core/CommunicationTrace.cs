using System.Diagnostics;
using System.Text;
using System.Text.Json;
using System.Threading.Channels;

namespace Updater.Core;

/// <summary>Best-effort diagnostic journal. Record never performs disk IO.</summary>
public sealed class CommunicationTrace : IAsyncDisposable
{
    private readonly long started = Stopwatch.GetTimestamp();
    private readonly Channel<object> queue = Channel.CreateUnbounded<object>(new() { SingleReader = true });
    private readonly Task writer;
    public string? FilePath { get; }
    public string? Error { get; private set; }

    private CommunicationTrace(StreamWriter? output, string? path, string? error)
    {
        FilePath = path;
        Error = error;
        writer = output is null ? Task.CompletedTask : Task.Run(() => WriteAsync(output));
        if (output is null) queue.Writer.TryComplete();
    }

    public static CommunicationTrace Create(string preferredDirectory, string operation, object metadata)
    {
        string? error = null;
        foreach (string directory in new[] { preferredDirectory, Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "C2000Updater", "Logs") })
        {
            try
            {
                Directory.CreateDirectory(directory);
                string name = $"communication_{DateTime.Now:yyyyMMdd_HHmmss_fff}_{Guid.NewGuid():N}.jsonl";
                string path = Path.GetFullPath(Path.Combine(directory, name));
                var output = new StreamWriter(new FileStream(path, FileMode.CreateNew, FileAccess.Write, FileShare.ReadWrite), new UTF8Encoding(false));
                var trace = new CommunicationTrace(output, path, null);
                trace.Record("trace_start", new { operation, metadata, preferredDirectory, fallbackReason = error });
                return trace;
            }
            catch (Exception ex) { error = ex.GetType().Name + ": " + ex.Message; }
        }
        return new CommunicationTrace(null, null, error);
    }

    public void Record(string eventName, object details)
    {
        try
        {
            queue.Writer.TryWrite(new { timestamp = DateTimeOffset.Now.ToString("yyyy-MM-ddTHH:mm:ss.fffzzz"), elapsed_ms = Stopwatch.GetElapsedTime(started).TotalMilliseconds, @event = eventName, details });
        }
        catch (Exception) { /* Diagnostics must never interrupt communication. */ }
    }

    private async Task WriteAsync(StreamWriter output)
    {
        try
        {
            await foreach (object entry in queue.Reader.ReadAllAsync())
            {
                await output.WriteLineAsync(JsonSerializer.Serialize(entry));
                await output.FlushAsync();
            }
        }
        catch (Exception ex) { Error = ex.GetType().Name + ": " + ex.Message; queue.Writer.TryComplete(); }
        finally { try { await output.DisposeAsync(); } catch (Exception) { } }
    }

    public async ValueTask DisposeAsync()
    {
        queue.Writer.TryComplete();
        try { await writer; } catch (Exception) { }
    }
}
