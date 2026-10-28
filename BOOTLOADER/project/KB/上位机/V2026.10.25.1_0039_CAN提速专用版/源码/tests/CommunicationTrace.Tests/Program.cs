using System.Text.Json;
using Updater.Core;

string root = Path.Combine(Path.GetTempPath(), "UpdaterTraceTests", Guid.NewGuid().ToString("N"));
int passed = 0;
void Check(bool condition, string name) { if (!condition) throw new Exception(name); Console.WriteLine("PASS " + name); passed++; }
JsonElement[] Read(string path) => File.ReadLines(path).Select(line => JsonDocument.Parse(line).RootElement.Clone()).ToArray();
bool Event(JsonElement e, string name) => e.GetProperty("event").GetString() == name;
await using (var trace = CommunicationTrace.Create(root, "drain", new { version = "test" }))
{
    Parallel.For(0, 10000, i => trace.Record("queued", new { i }));
    await trace.DisposeAsync();
    var lines = Read(trace.FilePath!);
    Check(lines.Count(e => Event(e, "queued")) == 10000, "dispose drains all 10000 concurrent events");
    Check(lines.All(e => e.TryGetProperty("timestamp", out _) && e.GetProperty("elapsed_ms").GetDouble() >= 0), "timestamp and monotonic elapsed present");
}
foreach (string mode in new[] { "success", "retry", "failure", "cancel", "timeout" })
{
    await using var trace = CommunicationTrace.Create(root, mode, new { });
    await using var transport = new ScenarioTransport(mode);
    using var cancellation = new CancellationTokenSource();
    if (mode == "cancel") cancellation.Cancel();
    Exception? failure = null;
    try { await new UpdaterClient(transport, trace).UpdateAsync(FirmwareImage.Demo(), null, cancellation.Token); }
    catch (Exception ex) { failure = ex; }
    await trace.DisposeAsync();
    var lines = Read(trace.FilePath!);
    var summary = lines.Single(e => Event(e, "update_summary")).GetProperty("details");
    if (mode is "success" or "retry")
    {
        Check(failure is null && summary.GetProperty("outcome").GetString() == "success", mode + " succeeds");
        Check(summary.GetProperty("acknowledgedWords").GetInt32() == FirmwareImage.Demo().Words.Length, mode + " acknowledged word count");
        Check(summary.GetProperty("retries").GetInt32() == (mode == "retry" ? 1 : 0), mode + " retry count");
    }
    else Check(failure is not null && summary.GetProperty("outcome").GetString() == (mode == "cancel" ? "cancelled" : "failed"), mode + " recorded without masking exception");
    if (mode == "timeout") Check(transport.Calls == 3 && summary.GetProperty("timeouts").GetInt32() == 3 && summary.GetProperty("retries").GetInt32() == 2, "original three-attempt policy unchanged");
    Check(lines.All(e => !e.GetProperty("details").TryGetProperty("payload", out _) && !e.GetProperty("details").TryGetProperty("Words", out _)), mode + " does not log firmware content");
}
Directory.CreateDirectory(root);
string blocked = Path.Combine(root, "not-a-directory");
File.WriteAllText(blocked, "block");
await using (var trace = CommunicationTrace.Create(blocked, "fallback", new { }))
{
    Check(trace.FilePath is not null && trace.Error is null && !trace.FilePath.StartsWith(blocked, StringComparison.OrdinalIgnoreCase), "unwritable preferred path falls back");
    await using var transport = new SimulatedTransport(true, false, 0);
    await new UpdaterClient(transport, trace).UpdateAsync(FirmwareImage.Demo(), null, CancellationToken.None);
    await trace.DisposeAsync();
    Check(Read(trace.FilePath!).Any(e => Event(e, "update_summary")), "fallback journal usable and upgrade unaffected");
}
await using (var trace = CommunicationTrace.Create(root, "writer-failure", new { }))
{
    trace.Record("unserializable", new { unsupported = typeof(Action) });
    using var wait = new CancellationTokenSource(TimeSpan.FromSeconds(5));
    while (trace.Error is null) await Task.Delay(10, wait.Token);
    Check(trace.Error.Contains("NotSupportedException", StringComparison.Ordinal), "background serialization failure is visible");
    await using var transport = new SimulatedTransport(true, false, 0);
    await new UpdaterClient(transport, trace).UpdateAsync(FirmwareImage.Demo(), null, CancellationToken.None);
    await trace.DisposeAsync();
    Check(transport.Device.State == BootState.Valid && transport.Device.CommitCount == 1, "failed log writer cannot interrupt upgrade or disposal");
}
Console.WriteLine($"All {passed} communication trace tests passed. Artifacts: {root}");

sealed class ScenarioTransport(string mode) : ITransport
{
    private readonly SimulatedTransport inner = new(true, mode == "retry", 0);
    public int Calls { get; private set; }
    public Task<Packet> ExchangeAsync(Packet request, TimeSpan timeout, CancellationToken cancellation)
    {
        Calls++;
        cancellation.ThrowIfCancellationRequested();
        if (mode == "timeout") throw new TimeoutException("injected");
        if (mode == "failure") inner.Device.FailWrite = true;
        return inner.ExchangeAsync(request, timeout, cancellation);
    }
    public ValueTask DisposeAsync() => inner.DisposeAsync();
}
