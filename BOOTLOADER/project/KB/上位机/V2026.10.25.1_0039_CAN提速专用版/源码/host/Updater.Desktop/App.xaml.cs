using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.IO;
namespace Updater.Desktop;
public partial class App : Application
{
    protected override async void OnStartup(StartupEventArgs e)
    {
        base.OnStartup(e);
        if (e.Args.Length == 2 && e.Args[0] == "--discover-can")
        {
            Shutdown(Updater.Discovery.CanDiscovery.Run(e.Args[1])); return;
        }
        bool selfCheck = e.Args.Length >= 2 && (e.Args[0] is "--self-check-0039-can" or "--self-check" or "--self-check-280049" or "--self-check-2800157" or "--self-check-ui" or "--self-check-sci335");
        bool automation = selfCheck || (e.Args.Length > 0 && e.Args[0] == "--snapshot");
        var window = new MainWindow(!automation);
        if (selfCheck) window.EnableSimulation();
        if (e.Args.Length >= 2 && e.Args[0] == "--self-check-0039-can")
        {
            try { await Can0039UiSelfCheck.Run(window, e.Args[1]); Shutdown(0); }
            catch (Exception ex) { File.WriteAllText(e.Args[1], ex.ToString()); Shutdown(1); }
            return;
        }
        if (e.Args.Length >= 2 && e.Args[0] == "--self-check-sci335")
        {
            try { await Sci335UiSelfCheck.Run(window, e.Args[1]); Shutdown(0); }
            catch (Exception ex) { File.WriteAllText(e.Args[1], ex.ToString()); Shutdown(1); }
            return;
        }
        if (e.Args.Length >= 2 && e.Args[0] == "--self-check-ui")
        {
            try
            {
                await UiSelfCheck.Run(window, e.Args[1]); Shutdown(0);
            }
            catch (Exception ex) { File.WriteAllText(e.Args[1], ex.ToString()); Shutdown(1); }
            return;
        }
        if (e.Args.Length >= 2 && e.Args[0] is "--self-check-280049" or "--self-check-2800157")
        {
            try
            {
                bool is00157 = e.Args[0] == "--self-check-2800157";
                window.ChipModel.SelectedIndex = is00157 ? 4 : 2;
                if (!window.PartitionLabel.Text.Contains("09FFEF") ||
                    !((System.Windows.Controls.ComboBoxItem)window.TransportMode.Items[1]).IsEnabled)
                    throw new InvalidOperationException("Selected device partition or CAN selection is incorrect");
                foreach (int mode in new[] { 0, 1 })
                {
                    window.TransportMode.SelectedIndex = mode;
                    window.LoadDemoImage();
                    window.StartButton.RaiseEvent(new RoutedEventArgs(System.Windows.Controls.Button.ClickEvent));
                    if (window.SettingsPanel.IsEnabled) throw new InvalidOperationException("Settings were not locked");
                    for (int i = 0; !window.SettingsPanel.IsEnabled && i < 1500; i++) await Task.Delay(10);
                    if (!window.SettingsPanel.IsEnabled || window.Progress.Value != 100)
                        throw new InvalidOperationException("SCI/CAN UI simulation failed: " + window.Log.Text);
                }
                window.ChipModel.SelectedIndex = 1;
                if (((System.Windows.Controls.ComboBoxItem)window.TransportMode.Items[1]).IsEnabled)
                    throw new InvalidOperationException("335 CAN should be disabled");
                window.ChipModel.SelectedIndex = 0;
                if (window.Progress.Value != 0 || !window.PartitionLabel.Text.Contains("0AEFFF"))
                    throw new InvalidOperationException("Legacy profile did not restore");
                File.WriteAllText(e.Args[1], System.Text.Json.JsonSerializer.Serialize(new
                {
                    Model = is00157 ? "AVP32F00157" : "AVP32F0049", Sci = "passed", Can = "passed", LostAck = "passed",
                    ModelSwitch = "passed", LegacyPartition = "passed", Avp335CanRestriction = "passed",
                    Runtime = System.Diagnostics.Process.GetCurrentProcess().Modules.Cast<System.Diagnostics.ProcessModule>()
                        .FirstOrDefault(m => string.Equals(m.ModuleName, "coreclr.dll", StringComparison.OrdinalIgnoreCase))?.FileName,
                    Converter = TiToolLocator.Find(), Driver = CanDriver.DefaultPath
                }, new System.Text.Json.JsonSerializerOptions { WriteIndented = true }));
                Shutdown(0);
            }
            catch (Exception ex) { File.WriteAllText(e.Args[1], ex.ToString()); Shutdown(1); }
            return;
        }
        if (e.Args.Length >= 2 && e.Args[0] == "--self-check")
        {
            try
            {
                string converter = TiToolLocator.Find() ?? throw new FileNotFoundException("TI converter not found");
                string driver = CanDriver.DefaultPath;
                using (var bridge = new CanBridge(driver)) { }
                string firmwareRoot = e.Args.Length >= 3 ? e.Args[2] : Path.Combine(AppContext.BaseDirectory, "firmware");
                var firmware = await Updater.Core.TiOutConverter.LoadAsync(Path.Combine(firmwareRoot,
                    e.Args.Length >= 3 ? "0039c/app/application.out" : "application.out"), converter);
                var avpFirmware = Updater.Core.Avp335OutLoader.Load(Path.Combine(firmwareRoot,
                    e.Args.Length >= 3 ? "335/30m/app/AVP32F335_APP_30MHz.out" : "335/30MHz/AVP32F335_APP_30MHz.out"));
                var avp33Firmware = Updater.Core.Avp335OutLoader.Load(Path.Combine(firmwareRoot,
                    e.Args.Length >= 3 ? "335/33.3m/app/AVP32F335_APP_33.3MHz.out" : "335/33.3MHz/AVP32F335_APP_33.3MHz.out"));
                window.ChipModel.SelectedIndex = 1;
                if (((System.Windows.Controls.ComboBoxItem)window.TransportMode.Items[1]).IsEnabled)
                    throw new InvalidOperationException("335 CAN should be disabled");
                window.LoadDemoImage();
                window.StartButton.RaiseEvent(new RoutedEventArgs(System.Windows.Controls.Button.ClickEvent));
                if (window.SettingsPanel.IsEnabled) throw new InvalidOperationException("Settings were not locked");
                for (int i = 0; !window.SettingsPanel.IsEnabled && i < 1500; i++) await Task.Delay(10);
                if (!window.SettingsPanel.IsEnabled || window.Progress.Value != 100 || !window.Log.Text.Contains("MD5"))
                    throw new InvalidOperationException("335 UI simulation did not complete");
                window.ChipModel.SelectedIndex = 0;
                if (window.Progress.Value != 0 || window.ImageLabel.Text.Contains("演示数据"))
                    throw new InvalidOperationException("Chip switch did not clear firmware");
                File.WriteAllText(e.Args[1], System.Text.Json.JsonSerializer.Serialize(new
                {
                    Runtime = System.Diagnostics.Process.GetCurrentProcess().Modules.Cast<System.Diagnostics.ProcessModule>()
                        .FirstOrDefault(m => string.Equals(m.ModuleName, "coreclr.dll", StringComparison.OrdinalIgnoreCase))?.FileName
                        ?? Environment.ProcessPath,
                    Driver = driver,
                    Converter = converter,
                    firmware.EntryWordAddress,
                    WordCount = firmware.Words.Length,
                    Avp335Bytes30MHz = avpFirmware.Bytes.Length,
                    Avp335Bytes33MHz = avp33Firmware.Bytes.Length,
                    Avp335UiSimulation = "passed",
                    ResizeMode = window.ResizeMode.ToString(),
                    window.MinWidth,
                    window.MinHeight
                }, new System.Text.Json.JsonSerializerOptions { WriteIndented = true }));
                Shutdown(0);
            }
            catch (Exception ex)
            {
                File.WriteAllText(e.Args[1], ex.ToString());
                Shutdown(1);
            }
            return;
        }
        if (e.Args.Length >= 2 && e.Args[0] == "--snapshot")
        {
            int width = e.Args.Length >= 4 ? int.Parse(e.Args[2]) : 800;
            int height = e.Args.Length >= 4 ? int.Parse(e.Args[3]) : 620;
            if (e.Args.Length >= 5 && e.Args[4] == "335") window.ChipModel.SelectedIndex = 1;
            if (e.Args.Length >= 5 && e.Args[4] == "280049") window.ChipModel.SelectedIndex = 2;
            if (e.Args.Length >= 5 && e.Args[4] == "280035") window.ChipModel.SelectedIndex = 3;
            if (e.Args.Length >= 5 && e.Args[4] == "2800157") window.ChipModel.SelectedIndex = 4;
            if (e.Args.Length >= 6 && e.Args[5] == "can" && window.ChipModel.SelectedIndex != 1) window.TransportMode.SelectedIndex = 1;
            if (e.Args.Length >= 7 && e.Args[6] == "longfile")
            {
                window.ImageLabel.Text = "客户应用程序_非常长的文件名称_20261008_Release_升级验证版本.out";
                window.ImageSummaryLabel.Text = "65,536 字节 · 已加载";
                window.ImageLabel.ToolTip = "布局测试示例文件，并未加载固件";
            }
            window.TopArea.MaxHeight = Math.Max(180, height - 180);
            var content = (FrameworkElement)window.Content;
            content.Measure(new Size(width, height)); content.Arrange(new Rect(0, 0, width, height)); content.UpdateLayout();
            double scale = e.Args.Length >= 8 ? double.Parse(e.Args[7], System.Globalization.CultureInfo.InvariantCulture) : 1;
            var bitmap = new RenderTargetBitmap((int)(width * scale), (int)(height * scale), 96 * scale, 96 * scale, PixelFormats.Pbgra32);
            bitmap.Render(content);
            var png = new PngBitmapEncoder(); png.Frames.Add(BitmapFrame.Create(bitmap));
            using (var file = File.Create(e.Args[1])) png.Save(file);
            Shutdown(); return;
        }
        MainWindow = window; window.Show();
    }
}
