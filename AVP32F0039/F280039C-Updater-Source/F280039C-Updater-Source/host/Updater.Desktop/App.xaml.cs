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
        var window = new MainWindow();
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
                if (((System.Windows.Controls.ComboBoxItem)window.TransportMode.Items[3]).IsEnabled)
                    throw new InvalidOperationException("335 CAN should be disabled");
                window.DemoButton.RaiseEvent(new RoutedEventArgs(System.Windows.Controls.Button.ClickEvent));
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
            int width = e.Args.Length >= 4 ? int.Parse(e.Args[2]) : 1080;
            int height = e.Args.Length >= 4 ? int.Parse(e.Args[3]) : 800;
            if (e.Args.Length >= 5 && e.Args[4] == "335") window.ChipModel.SelectedIndex = 1;
            var content = (FrameworkElement)window.Content;
            content.Measure(new Size(width, height)); content.Arrange(new Rect(0, 0, width, height)); content.UpdateLayout();
            var bitmap = new RenderTargetBitmap(width, height, 96, 96, PixelFormats.Pbgra32);
            bitmap.Render(content);
            var png = new PngBitmapEncoder(); png.Frames.Add(BitmapFrame.Create(bitmap));
            using (var file = File.Create(e.Args[1])) png.Save(file);
            Shutdown(); return;
        }
        MainWindow = window; window.Show();
    }
}
