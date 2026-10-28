using System.Diagnostics;

namespace Updater.Core;

/// <summary>调用 TI 原厂 hex2000，将 CCS .out 转换为已验证的升级镜像。</summary>
public static class TiOutConverter
{
    /// <summary>
    /// 转换只写入本次任务的临时目录，不修改用户的 .out，也不打开通信设备。
    /// 后续解析仍执行入口、分区边界、重叠地址和 ECC 对齐检查。
    /// </summary>
    public static async Task<FirmwareImage> LoadAsync(
        string outPath,
        string hex2000Path,
        CancellationToken cancellation = default, DeviceProfile? target = null)
    {
        ArgumentException.ThrowIfNullOrWhiteSpace(outPath);
        ArgumentException.ThrowIfNullOrWhiteSpace(hex2000Path);
        cancellation.ThrowIfCancellationRequested();

        string input = Path.GetFullPath(outPath);
        string converter = Path.GetFullPath(hex2000Path);
        if (!File.Exists(converter))
            throw new FileNotFoundException("未找到 TI hex2000.exe，请选择 CCS 编译器 bin 目录中的转换工具。", converter);
        var file = new FileInfo(input);
        if (!file.Exists)
            throw new FileNotFoundException("未找到 .out 固件。", input);
        if (file.Length == 0 || file.Length > 128L * 1024 * 1024)
            throw new InvalidDataException(".out 文件为空或超过 128 MB 限制。");

        string temporary = Path.Combine(Path.GetTempPath(), "F280039C-Updater", Guid.NewGuid().ToString("N"));
        Directory.CreateDirectory(temporary);
        try
        {
            // 固定临时文件名也避免原文件名中的特殊字符被 TI 工具解释为选项。
            string snapshot = Path.Combine(temporary, "application.out");
            File.Copy(input, snapshot);
            string output = Path.Combine(temporary, "application.txt");
            var start = new ProcessStartInfo(converter)
            {
                UseShellExecute = false,
                CreateNoWindow = true,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                WorkingDirectory = temporary
            };
            start.ArgumentList.Add("--boot");
            start.ArgumentList.Add("--sci8");
            start.ArgumentList.Add("--ascii");
            start.ArgumentList.Add("--outfile=" + output);
            start.ArgumentList.Add(snapshot);

            using var process = Process.Start(start)
                ?? throw new IOException("无法启动 TI hex2000 转换工具。");
            Task<string> stdout = process.StandardOutput.ReadToEndAsync();
            Task<string> stderr = process.StandardError.ReadToEndAsync();
            using var deadline = CancellationTokenSource.CreateLinkedTokenSource(cancellation);
            deadline.CancelAfter(TimeSpan.FromSeconds(30));
            try
            {
                await process.WaitForExitAsync(deadline.Token).ConfigureAwait(false);
            }
            catch (OperationCanceledException)
            {
                if (!process.HasExited)
                    process.Kill(entireProcessTree: true);
                await process.WaitForExitAsync(CancellationToken.None).ConfigureAwait(false);
                await Task.WhenAll(stdout, stderr).ConfigureAwait(false);
                if (cancellation.IsCancellationRequested)
                    throw;
                throw new TimeoutException(".out 转换超过 30 秒，已停止转换工具。");
            }

            string diagnostics = (await stderr.ConfigureAwait(false) + "\n" + await stdout.ConfigureAwait(false)).Trim();
            if (process.ExitCode != 0 || !File.Exists(output))
                throw new InvalidDataException("TI hex2000 转换失败，请确认文件为 C2000 应用 .out。\n" + diagnostics);

            cancellation.ThrowIfCancellationRequested();
            // 复用已实板验收的 TI boot table 导入链路，不自行解释 ELF 装载地址。
            return TiBootTable.Load(output, target: target);
        }
        finally
        {
            try { Directory.Delete(temporary, recursive: true); }
            catch (IOException) { /* 杀毒软件短暂占用时不覆盖原始错误或成功结果。 */ }
            catch (UnauthorizedAccessException) { }
        }
    }
}
