using System.Text;
using System.Text.Json;
using Updater.Desktop;

if (args.Length == 2 && args[0] == "--discover-can")
    return Updater.Discovery.CanDiscovery.Run(args[1]);

var checks = new List<string>();
var logs = new List<string>();
void Assert(bool ok, string name)
{
    if (!ok) throw new InvalidOperationException(name);
    checks.Add(name);
    Console.WriteLine("PASS " + name);
}
var zlg = new CanDriver.Driver("周立功", @"C:\test drivers\zlg\usbcan.dll");
var cx = new CanDriver.Driver("创芯科技", @"C:\test drivers\创芯\ControlCAN.dll");
var both = new[] { zlg, cx };
DetectedCanDevice[] Frame(string serial = "SAME") => [new(4, 0, 2, serial)];
Task<DetectedCanDevice[]> Success(string _, CancellationToken __) => Task.FromResult(Frame());

var result = await DeviceDiscovery.ScanDriversAsync(both, Success, default, logs.Add);
Assert(result.Length == 2 && result[0].Brand == "周立功" && result[1].Brand == "创芯科技",
    "two vendors with identical index and serial remain separate");
Assert(result[0].DriverPath == zlg.Path && result[1].DriverPath == cx.Path,
    "each device remains bound to its detected driver");
Assert(result[0].ToString().Contains("周立功") && result[1].ToString().Contains("创芯科技"),
    "device list labels identify driver vendor");
result = await DeviceDiscovery.ScanDriversAsync([cx], Success, default, logs.Add);
Assert(result.Length == 1 && result[0].DriverPath == cx.Path, "single Chuangxin device auto-selection candidate");
result = await DeviceDiscovery.ScanDriversAsync([zlg], Success, default, logs.Add);
Assert(result.Length == 1 && result[0].DriverPath == zlg.Path, "single ZLG device auto-selection candidate");

foreach (var failure in new Exception[] { new FileNotFoundException("missing DLL"), new TimeoutException("probe timeout"), new BadImageFormatException("bad DLL") })
{
    result = await DeviceDiscovery.ScanDriversAsync(both, (path, _) => path == zlg.Path
        ? Task.FromException<DetectedCanDevice[]>(failure) : Task.FromResult(Frame()), default, logs.Add);
    Assert(result.Length == 1 && result[0].Brand == "创芯科技", failure.GetType().Name + " from ZLG does not hide Chuangxin");
    result = await DeviceDiscovery.ScanDriversAsync(both, (path, _) => path == cx.Path
        ? Task.FromException<DetectedCanDevice[]>(failure) : Task.FromResult(Frame()), default, logs.Add);
    Assert(result.Length == 1 && result[0].Brand == "周立功", failure.GetType().Name + " from Chuangxin does not hide ZLG");
}
Assert(logs.Count == 6, "per-vendor failures recorded");
result = await DeviceDiscovery.ScanDriversAsync(both, (_, _) => Task.FromResult(Array.Empty<DetectedCanDevice>()), default, logs.Add);
Assert(result.Length == 0, "no device produces an empty list");

using (var cancellation = new CancellationTokenSource())
{
    cancellation.Cancel();
    try
    {
        await DeviceDiscovery.ScanDriversAsync(both, (_, token) => Task.FromCanceled<DetectedCanDevice[]>(token), cancellation.Token, logs.Add);
        throw new InvalidOperationException("cancellation swallowed");
    }
    catch (OperationCanceledException) { Assert(true, "explicit cancellation propagates"); }
}
using (var cancellation = new CancellationTokenSource())
{
    var started = new TaskCompletionSource();
    var scanning = DeviceDiscovery.ScanDriversAsync(both, async (_, token) =>
    {
        started.TrySetResult();
        await Task.Delay(Timeout.Infinite, token);
        return Frame();
    }, cancellation.Token, logs.Add);
    await started.Task;
    cancellation.Cancel();
    try { await scanning; throw new InvalidOperationException("mid-scan cancellation swallowed"); }
    catch (OperationCanceledException) { Assert(true, "cancellation during scan propagates"); }
}
result = await DeviceDiscovery.ScanDriversAsync(both, (_, _) => Task.FromResult<DetectedCanDevice[]>(
    [new(3, 0, 1, "one"), new(4, 0, 2, "two")]), default, logs.Add);
Assert(result.Count(device => device.Brand == "创芯科技") == 1 && result.Last().Type == 4,
    "Chuangxin SDK only accepts documented device type 4");
result = await DeviceDiscovery.ScanDriversAsync([zlg], (_, _) => Task.FromResult<DetectedCanDevice[]>(
    [new(4, 0, 2, ""), new(4, 1, 2, ""), new(4, 0, 2, "")]), default, logs.Add);
Assert(result.Length == 2, "empty serial uses index and removes exact duplicates");

var defaults = CanDriver.Candidates();
var manual = CanDriver.Candidates(defaults[0].Path.ToUpperInvariant());
Assert(defaults.Length == 2 && manual.Length == 2, "manual duplicate driver path is not probed twice");
Assert(CanDriver.Candidates(@"C:\additional\ControlCAN.dll").Length == 3, "manual extra driver preserves both bundled candidates");
Assert(defaults.Single(driver => driver.Brand == "创芯科技").Path.StartsWith(AppContext.BaseDirectory),
    "Chuangxin bundled path is relative to deployed application directory");

var parsed = DeviceDiscovery.ParseCanDevices("4\t0\t2\t" + Convert.ToBase64String(Encoding.ASCII.GetBytes("abc")));
Assert(parsed.Length == 1 && parsed[0].Serial == "abc", "existing discovery wire format retained");
try { DeviceDiscovery.ParseCanDevices("4\t0\t1\tYQ=="); throw new InvalidOperationException("invalid layout accepted"); }
catch (InvalidDataException) { Assert(true, "malformed channel count rejected"); }

string output = args.Length > 0 ? args[0] : "can-vendor-tests.json";
Directory.CreateDirectory(Path.GetDirectoryName(Path.GetFullPath(output))!);
File.WriteAllText(output, JsonSerializer.Serialize(new { Passed = checks.Count, Checks = checks, FailureLogs = logs,
    Boundary = "Real scanner orchestration, injected enumeration results; no physical CAN transmit or driver installation." },
    new JsonSerializerOptions { WriteIndented = true }));
Console.WriteLine($"All {checks.Count} CAN vendor tests passed.");
return 0;
