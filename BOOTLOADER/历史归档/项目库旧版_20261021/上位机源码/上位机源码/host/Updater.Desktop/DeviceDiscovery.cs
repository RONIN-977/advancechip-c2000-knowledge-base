using Microsoft.Win32;
using System.Diagnostics;
using System.IO;
using System.Text;

namespace Updater.Desktop;

internal sealed record DetectedCanDevice(uint Type, uint Index, byte Channels, string Serial)
{
    public string Model => Type == 3 ? "USBCAN-I" : "USBCAN-II";
    public override string ToString() => $"{Model} · 设备 {Index}" + (Serial.Length == 0 ? "" : $" · {Serial}");
}

internal static class DeviceDiscovery
{
    public static string[] SerialPorts()
    {
        using var key = Registry.LocalMachine.OpenSubKey(@"HARDWARE\DEVICEMAP\SERIALCOMM");
        return (key?.GetValueNames() ?? [])
            .Select(name => key?.GetValue(name)?.ToString())
            .OfType<string>()
            .Where(port => port.StartsWith("COM", StringComparison.OrdinalIgnoreCase) &&
                int.TryParse(port.AsSpan(3), out int number) && number > 0)
            .Select(port => port.ToUpperInvariant()).Distinct()
            .OrderBy(port => int.Parse(port.AsSpan(3))).ToArray();
    }

    public static async Task<DetectedCanDevice[]> CanDevicesAsync(string dll, CancellationToken cancellation)
    {
        if (!File.Exists(dll)) throw new FileNotFoundException("未找到 CAN 驱动，请点击“浏览”选择 usbcan.dll 或 ControlCAN.dll。", dll);
        bool x86 = CanDriver.IsX86(dll);
        string bridge = Path.Combine(AppContext.BaseDirectory, "bridge", "x86", "CanBridge32.exe");
        if (!File.Exists(bridge)) bridge = Path.Combine(AppContext.BaseDirectory, "CanBridge32.exe");
        string executable = x86 ? bridge : Environment.ProcessPath ?? throw new IOException("无法定位设备检索程序。");
        var start = new ProcessStartInfo(executable)
        {
            UseShellExecute = false, CreateNoWindow = true,
            RedirectStandardOutput = true, RedirectStandardError = true
        };
        if (!x86 && Path.GetFileNameWithoutExtension(executable).Equals("dotnet", StringComparison.OrdinalIgnoreCase))
            start.ArgumentList.Add(typeof(DeviceDiscovery).Assembly.Location);
        start.ArgumentList.Add(x86 ? "--discover" : "--discover-can");
        start.ArgumentList.Add(Path.GetFullPath(dll));
        using var process = Process.Start(start) ?? throw new IOException("无法启动 CAN 设备检索程序。");
        using var timeout = CancellationTokenSource.CreateLinkedTokenSource(cancellation);
        timeout.CancelAfter(TimeSpan.FromSeconds(12));
        var output = process.StandardOutput.ReadToEndAsync();
        var errors = process.StandardError.ReadToEndAsync();
        try { await process.WaitForExitAsync(timeout.Token); }
        catch (OperationCanceledException)
        {
            if (!process.HasExited) process.Kill(true);
            await process.WaitForExitAsync(CancellationToken.None);
            if (cancellation.IsCancellationRequested) throw;
            throw new TimeoutException("CAN 设备检索超时，请检查驱动及盒子是否被其他软件占用。");
        }
        string result = await output, error = await errors;
        if (process.ExitCode != 0) throw new IOException("CAN 设备检索失败：" + error.Trim());
        return ParseCanDevices(result);
    }

    internal static DetectedCanDevice[] ParseCanDevices(string output)
    {
        var devices = new List<DetectedCanDevice>();
        foreach (string line in output.Split('\n', StringSplitOptions.RemoveEmptyEntries))
        {
            string[] fields = line.TrimEnd('\r').Split('\t');
            if (fields.Length != 4 || !uint.TryParse(fields[0], out uint type) || type is not (3 or 4) ||
                !uint.TryParse(fields[1], out uint index) || index > 15 ||
                !byte.TryParse(fields[2], out byte channels) || channels != (type == 3 ? 1 : 2))
                throw new InvalidDataException("CAN 设备检索结果格式错误。");
            byte[] serialBytes = Convert.FromBase64String(fields[3]);
            string serial = Encoding.ASCII.GetString(serialBytes).TrimEnd('\0', ' ');
            devices.Add(new(type, index, channels, serial));
        }
        return devices.DistinctBy(device => device.Serial.Length == 0
            ? $"{device.Type}:{device.Index}" : $"{device.Type}:{device.Serial}").ToArray();
    }
}
