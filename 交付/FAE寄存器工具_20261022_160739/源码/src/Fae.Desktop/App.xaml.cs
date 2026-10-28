using System.IO;
using System.Windows;

namespace Fae.Desktop;
public partial class App : Application
{
    protected override void OnStartup(StartupEventArgs e)
    {
        base.OnStartup(e);
        bool smoke = e.Args.Contains("--smoke-test");
        DispatcherUnhandledException += (_, a) => { if (smoke) { File.WriteAllText(Path.Combine(Path.GetTempPath(), "fae-smoke-error.txt"), a.Exception.ToString()); Shutdown(2); } else MessageBox.Show(a.Exception.Message, "FAE 工具：操作未完成"); a.Handled = true; };
        MainWindow window;
        try { window = new MainWindow(); MainWindow = window; }
        catch (Exception ex) { File.WriteAllText(Path.Combine(Path.GetTempPath(), "fae-smoke-error.txt"), ex.ToString()); if (!smoke) MessageBox.Show(ex.Message, "FAE 工具启动失败"); Shutdown(2); return; }
        if (e.Args.Contains("--smoke-test"))
        {
            window.Loaded += async (_, _) =>
            {
                try
                {
                    int idx = Array.IndexOf(e.Args, "--screenshot");
                    string folder = idx >= 0 && idx + 1 < e.Args.Length ? e.Args[idx + 1] : Path.Combine(AppContext.BaseDirectory, "smoke");
                    await window.SmokeTest(folder, e.Args.Contains("--test-clipboard"));
                    Shutdown(0);
                }
                catch (Exception ex) { File.WriteAllText(Path.Combine(Path.GetTempPath(), "fae-smoke-error.txt"), ex.ToString()); Shutdown(2); }
            };
        }
        window.Show();
    }
}
