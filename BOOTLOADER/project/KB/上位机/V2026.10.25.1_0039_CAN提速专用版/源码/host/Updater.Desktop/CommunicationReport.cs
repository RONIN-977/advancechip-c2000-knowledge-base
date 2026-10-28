using System.Globalization;
using System.IO;
using System.Text;
using System.Text.Json;

namespace Updater.Desktop;

internal static class CommunicationReport
{
    public static void Save(string path)
    {
        string Readable(JsonElement value) => JsonSerializer.Serialize(value, new JsonSerializerOptions { Encoder = System.Text.Encodings.Web.JavaScriptEncoder.UnsafeRelaxedJsonEscaping });
        var commands = new Dictionary<string, (int Count, double Milliseconds)>();
        double total = 0, pacing = 0, tx = 0, rx = 0, idle = 0, exchange = 0;
        int retries = 0, timeouts = 0, txFrames = 0, rxFrames = 0, exchanges = 0;
        var modes = new Dictionary<string, int>();
        double requestedPacing = 0;
        var errors = new List<string>();
        var text = new StringBuilder("通讯诊断摘要 V2026.10.25.1\n");
        foreach (string line in File.ReadLines(path))
        {
            using var json = JsonDocument.Parse(line);
            var row = json.RootElement;
            string? name = row.GetProperty("event").GetString();
            var d = row.GetProperty("details");
            double Number(string key) => d.TryGetProperty(key, out var v) && v.ValueKind == JsonValueKind.Number ? v.GetDouble() : 0;
            if (name == "trace_start") text.AppendLine(Readable(d));
            if (name == "operation_result") { total = row.GetProperty("elapsed_ms").GetDouble(); text.AppendLine("结果：" + Readable(d)); }
            if (name == "command_end")
            {
                string command = d.GetProperty("command").GetString()!;
                var sum = commands.GetValueOrDefault(command);
                commands[command] = (sum.Count + 1, sum.Milliseconds + Number("elapsed_ms"));
            }
            if (name == "exchange_retry") ++retries;
            if (name == "exchange_timeout" || (name == "command_failure" && d.TryGetProperty("error", out var e) && e.GetString() == "TimeoutException")) ++timeouts;
            if (name is "command_failure" or "transport_exchange_error" or "update_failure" or "transport_open_error") errors.Add(Readable(d));
            if (name == "can_exchange")
            {
                ++exchanges; exchange += Number("total_ms"); pacing += Number("pacing_ms");
                tx += Number("tx_dll_ms"); rx += Number("rx_dll_ms"); idle += Number("rx_idle_ms");
                txFrames += (int)Number("txFrames"); rxFrames += (int)Number("rxFrames");
                requestedPacing += Number("requested_pacing_ms");
                if (d.TryGetProperty("pacing_mode", out var mode))
                {
                    string key = mode.GetString() ?? "unknown";
                    modes[key] = modes.GetValueOrDefault(key) + (int)Number("txFrames");
                }
            }
        }
        string Milliseconds(double n) => n.ToString("F3", CultureInfo.InvariantCulture) + " ms";
        text.AppendLine("操作总耗时（开始创建日志至结果记录）：" + Milliseconds(total));
        foreach (var pair in commands) text.AppendLine($"命令 {pair.Key}：成功 {pair.Value.Count} 次，累计 {Milliseconds(pair.Value.Milliseconds)}（含该命令此前重试）");
        text.AppendLine($"重试 {retries} 次；命令超时 {timeouts} 次。");
        text.AppendLine($"CAN交换 {exchanges} 次，发送 {txFrames} 帧，收到 {rxFrames} 帧。");
        text.AppendLine("CAN交换累计：" + Milliseconds(exchange));
        text.AppendLine("其中发送DLL/桥接调用：" + Milliseconds(tx));
        text.AppendLine("其中帧间等待：" + Milliseconds(pacing));
        text.AppendLine("请求帧间等待累计：" + Milliseconds(requestedPacing));
        foreach (var mode in modes) text.AppendLine($"等待模式 {mode.Key}：{mode.Value} 帧");
        text.AppendLine("其中接收DLL/桥接调用：" + Milliseconds(rx));
        text.AppendLine("其中空读后等待：" + Milliseconds(idle));
        if (txFrames > 0) text.AppendLine("平均每帧等待：" + Milliseconds(pacing / txFrames));
        text.AppendLine("说明：命令耗时与CAN交换耗时互相包含，不能相加。DLL返回发送成功不等于目标已确认；接收等待含USB、设备处理、总线等时间，不能单独认定为Flash耗时。x86桥接调用还含进程通信。335 SCI仅记录操作/UI日志，不含上述CAN指标。");
        if (errors.Count > 0) text.AppendLine("异常详情：\n" + string.Join("\n", errors));
        File.WriteAllText(Path.ChangeExtension(path, ".summary.txt"), text.ToString(), new UTF8Encoding(false));
    }
}
