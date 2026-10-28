using System.Text.Json;
using Updater.Desktop;

string root = Path.Combine(Path.GetTempPath(), "CommunicationReportTests-" + Guid.NewGuid().ToString("N"));
Directory.CreateDirectory(root);
int assertions = 0;
object Row(string name, object details, double elapsed = 0) => new { @event = name, details, elapsed_ms = elapsed };
void Contains(string actual, string expected)
{
    if (!actual.Contains(expected, StringComparison.Ordinal)) throw new Exception($"Missing [{expected}] in:\n{actual}");
    ++assertions;
}
string Report(string test, params object[] rows)
{
    string path = Path.Combine(root, test + ".jsonl");
    File.WriteAllLines(path, rows.Select(r => JsonSerializer.Serialize(r)));
    CommunicationReport.Save(path);
    return File.ReadAllText(Path.ChangeExtension(path, ".summary.txt"));
}

string success = Report("success",
    Row("trace_start", new { operation = "upgrade", brand = "创芯" }),
    Row("stage_start", new { stage = "enter_boot" }),
    Row("command_end", new { command = "EnterBoot", elapsed_ms = 25 }),
    Row("stage_end", new { stage = "enter_boot" }),
    Row("command_end", new { command = "Data", elapsed_ms = 100 }),
    Row("command_end", new { command = "Data", elapsed_ms = 200 }),
    Row("can_exchange", new { total_ms = 100, pacing_ms = 60, tx_dll_ms = 5, rx_dll_ms = 25, rx_idle_ms = 10, txFrames = 4, rxFrames = 1, requested_pacing_ms = 4, pacing_mode = "high-resolution-timer" }),
    Row("can_exchange", new { total_ms = 200, pacing_ms = 90, tx_dll_ms = 10, rx_dll_ms = 80, rx_idle_ms = 20, txFrames = 6, rxFrames = 3, requested_pacing_ms = 18, pacing_mode = "legacy-control" }),
    Row("operation_result", new { status = "升级成功" }, 1200));
Contains(success, "创芯");
Contains(success, "升级成功");
Contains(success, "1200.000 ms");
Contains(success, "命令 EnterBoot：成功 1 次，累计 25.000 ms");
Contains(success, "命令 Data：成功 2 次，累计 300.000 ms");
Contains(success, "重试 0 次；命令超时 0 次");
Contains(success, "CAN交换 2 次，发送 10 帧，收到 4 帧");
Contains(success, "CAN交换累计：300.000 ms");
Contains(success, "其中帧间等待：150.000 ms");
Contains(success, "其中发送DLL/桥接调用：15.000 ms");
Contains(success, "其中接收DLL/桥接调用：105.000 ms");
Contains(success, "其中空读后等待：30.000 ms");
Contains(success, "平均每帧等待：15.000 ms");
Contains(success, "请求帧间等待累计：22.000 ms");
Contains(success, "等待模式 high-resolution-timer：4 帧");
Contains(success, "等待模式 legacy-control：6 帧");
Console.WriteLine("PASS successful command aggregation and CAN timing totals");

string retry = Report("retries",
    Row("transport_exchange_error", new { errorType = "TimeoutException", error = "timeout" }),
    Row("exchange_timeout", new { command = "Data", attempt = 1 }),
    Row("exchange_retry", new { command = "Data", next_attempt = 2 }),
    Row("transport_exchange_error", new { errorType = "TimeoutException", error = "timeout" }),
    Row("exchange_timeout", new { command = "Data", attempt = 2 }),
    Row("exchange_retry", new { command = "Data", next_attempt = 3 }),
    Row("transport_exchange_error", new { errorType = "TimeoutException", error = "timeout" }),
    Row("command_failure", new { command = "Data", attempt = 3, error = "TimeoutException" }),
    Row("update_failure", new { error = "TimeoutException" }),
    Row("operation_result", new { status = "失败" }, 9010));
Contains(retry, "重试 2 次；命令超时 3 次");
Contains(retry, "CAN交换 0 次，发送 0 帧，收到 0 帧");
Contains(retry, "异常详情");
if (retry.Contains("平均每帧等待", StringComparison.Ordinal)) throw new Exception("Zero frame division shown");
++assertions;
Console.WriteLine("PASS three timeout attempts counted once, two retries, no divide by zero");

string badState = Report("badstate",
    Row("exchange_response", new { command = "Data", status = "BadState" }),
    Row("command_failure", new { command = "Data", error = "IOException", Message = "设备拒绝命令 Data: BadState" }),
    Row("update_failure", new { error = "IOException", Message = "设备拒绝命令 Data: BadState" }),
    Row("operation_result", new { status = "失败" }, 42));
Contains(badState, "BadState");
Contains(badState, "重试 0 次；命令超时 0 次");
Contains(badState, "42.000 ms");
Console.WriteLine("PASS BadState retained as failure without false retry or timeout");
Console.WriteLine($"PASS {assertions} assertions. Fixtures and summaries: {root}");
