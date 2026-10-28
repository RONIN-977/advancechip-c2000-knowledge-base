using System.IO;

namespace Updater.Desktop;

/// <summary>发现本机 TI 工具；不从 PATH 中自动执行同名的未知程序。</summary>
internal static class TiToolLocator
{
    private static readonly string SettingsPath = Path.Combine(
        Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
        "F280039C-Updater", "hex2000-path.txt");

    public static string? Find()
    {
        string bundled = Path.Combine(AppContext.BaseDirectory, "tools", "ti", "hex2000.exe");
        if (File.Exists(bundled)) return bundled;
        // 用户上次明确选择的工具优先，迁移/卸载后自动重新发现。
        try
        {
            if (File.Exists(SettingsPath))
            {
                string saved = File.ReadAllText(SettingsPath).Trim();
                if (File.Exists(saved)) return saved;
            }
        }
        catch (IOException) { }
        catch (UnauthorizedAccessException) { }

        var roots = new[]
        {
            @"C:\ti", @"C:\CCS12", @"C:\CCS21",
            Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles), "Texas Instruments"),
            Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86), "Texas Instruments")
        };
        var candidates = new List<string>();
        // 仅枚举安装目录和有限深度的 CCS 子目录，不扫描整个磁盘。
        foreach (string root in roots)
            Scan(root, 0, candidates);
        return candidates.OrderByDescending(File.GetLastWriteTimeUtc).FirstOrDefault();
    }

    private static void Scan(string directory, int depth, List<string> candidates)
    {
        if (depth > 6 || !Directory.Exists(directory)) return;
        try
        {
            string executable = Path.Combine(directory, "hex2000.exe");
            if (File.Exists(executable)) candidates.Add(executable);
            foreach (string child in Directory.GetDirectories(directory))
            {
                if ((File.GetAttributes(child) & FileAttributes.ReparsePoint) != 0) continue;
                string name = Path.GetFileName(child);
                if (name.StartsWith("ccs", StringComparison.OrdinalIgnoreCase) ||
                    name.StartsWith("ti-cgt-c2000", StringComparison.OrdinalIgnoreCase) ||
                    name.Equals("tools", StringComparison.OrdinalIgnoreCase) ||
                    name.Equals("compiler", StringComparison.OrdinalIgnoreCase) ||
                    name.Equals("bin", StringComparison.OrdinalIgnoreCase))
                    Scan(child, depth + 1, candidates);
            }
        }
        catch (IOException) { }
        catch (UnauthorizedAccessException) { }
    }

    public static void Remember(string path)
    {
        Directory.CreateDirectory(Path.GetDirectoryName(SettingsPath)!);
        File.WriteAllText(SettingsPath, Path.GetFullPath(path));
    }
}
