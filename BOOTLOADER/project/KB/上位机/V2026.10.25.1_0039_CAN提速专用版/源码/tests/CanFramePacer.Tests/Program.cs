using System.Diagnostics;
using System.Text.Json;
using Updater.Desktop;

int checks = 0;
void Check(bool value, string message)
{
    if (!value) throw new Exception(message);
    ++checks;
}
void CheckCanceled(Action action, CancellationToken expected)
{
    try { action(); throw new Exception("Expected cancellation"); }
    catch (OperationCanceledException ex) { Check(ex.CancellationToken == expected, "Cancellation token changed"); }
}

using var high = new CanFramePacer();
Check(high.Mode == "high-resolution-timer", "This Windows must support high-resolution timer for this hardware-free validation");
using var fallback = new CanFramePacer(forceFallback: true);
Check(fallback.Mode == "legacy-wait-fallback", "Forced fallback did not select fallback");
foreach (var pacer in new[] { high, fallback })
{
    for (int i = 0; i < 50; ++i)
    {
        long started = Stopwatch.GetTimestamp();
        pacer.Wait(CancellationToken.None);
        Check(Stopwatch.GetElapsedTime(started).TotalMilliseconds >= 1, "Wait returned earlier than 1 ms");
    }
    // A stalled caller must still wait a fresh interval, never catch up old deadlines.
    Thread.Sleep(25);
    long afterStall = Stopwatch.GetTimestamp();
    pacer.Wait(CancellationToken.None);
    Check(Stopwatch.GetElapsedTime(afterStall).TotalMilliseconds >= 1, "Catch-up burst after delayed caller");
    using var canceled = new CancellationTokenSource();
    canceled.Cancel();
    CheckCanceled(() => pacer.Wait(canceled.Token), canceled.Token);
    using var during = new CancellationTokenSource();
    during.CancelAfter(20);
    var cancellationWatch = Stopwatch.StartNew();
    CheckCanceled(() => { while (true) pacer.Wait(during.Token); }, during.Token);
    Check(cancellationWatch.ElapsedMilliseconds < 1000, "Cancellation too slow");
}
var dead = new CanFramePacer();
dead.Dispose();
dead.Dispose();
try { dead.Wait(CancellationToken.None); throw new Exception("Disposed wait accepted"); }
catch (ObjectDisposedException) { ++checks; }

object Measure(string name, Action action, int count)
{
    for (int i = 0; i < 20; ++i) action();
    var process = Process.GetCurrentProcess();
    process.Refresh();
    var cpuBefore = process.TotalProcessorTime;
    var wall = Stopwatch.StartNew();
    var samples = new double[count];
    for (int i = 0; i < samples.Length; ++i)
    {
        long start = Stopwatch.GetTimestamp();
        action();
        samples[i] = Stopwatch.GetElapsedTime(start).TotalMilliseconds;
    }
    wall.Stop();
    process.Refresh();
    double cpuMs = (process.TotalProcessorTime - cpuBefore).TotalMilliseconds;
    Array.Sort(samples);
    double P(double p) => samples[(int)Math.Ceiling(p * samples.Length) - 1];
    return new { name, count, minimumMs = samples[0], averageMs = samples.Average(),
        p50Ms = P(.50), p90Ms = P(.90), p95Ms = P(.95), p99Ms = P(.99), maximumMs = samples[^1],
        wallMs = wall.Elapsed.TotalMilliseconds, cpuMs, cpuPercentOfOneCore = cpuMs / wall.Elapsed.TotalMilliseconds * 100 };
}
var measurements = new[] {
    Measure("high-resolution-relative-1ms", () => high.Wait(CancellationToken.None), 1000),
    Measure("legacy-WaitOne-3ms", () => CancellationToken.None.WaitHandle.WaitOne(3), 200),
    Measure("forced-fallback-WaitOne-1ms", () => fallback.Wait(CancellationToken.None), 200)
};
string output = JsonSerializer.Serialize(new { utc = DateTime.UtcNow, os = Environment.OSVersion.ToString(),
    checksPassed = checks, timerMode = high.Mode, measurements,
    limitation = "Software timing and cancellation only. No CAN device opened; actual bus spacing and firmware upgrade require hardware regression." },
    new JsonSerializerOptions { WriteIndented = true });
if (args.Length > 0)
{
    Directory.CreateDirectory(Path.GetDirectoryName(Path.GetFullPath(args[0]))!);
    File.WriteAllText(args[0], output);
}
Console.WriteLine(output);
