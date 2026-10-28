using System.IO;
using System.Text.Json;
using System.Windows;
using System.Windows.Controls;
using Updater.Core;

namespace Updater.Desktop;

internal static class Sci335UiSelfCheck
{
    internal static async Task Run(MainWindow window, string report)
    {
        static void Assert(bool condition, string message)
        { if (!condition) throw new InvalidOperationException(message); }
        Assert(MainWindow.Customer335Edition && window.ChipModel.SelectedIndex == 1 &&
            !window.ChipModel.IsEnabled && !window.TransportMode.IsEnabled && !window.BaudRate.IsEnabled,
            "335 SCI settings must be fixed.");
        Assert(window.TransportMode.SelectedIndex == 0 && (int)window.BaudRate.SelectedItem == 115200, "SCI 115200 required.");
        Assert(window.ChipModel.Items.Cast<ComboBoxItem>().Count(i => i.Visibility == Visibility.Visible) == 1 &&
            ((ComboBoxItem)window.TransportMode.Items[1]).Visibility == Visibility.Collapsed, "Other model and CAN entries must be hidden.");
        Assert(new FrameworkElement[] { window.ProbeButton, window.ResetButton, window.CanOptions, window.CanChannelPanel }
            .All(i => i.Visibility == Visibility.Collapsed), "Unused buttons and CAN settings must be hidden.");
        Assert(!window.Title.Contains("客户") && !window.WindowHeading.Text.Contains("客户") &&
            !window.Log.Text.Contains("5 秒"), "Neutral title and current instructions required.");
        Assert(window.AppStartAddress.Visibility == Visibility.Visible && window.ComPort.Visibility == Visibility.Visible,
            "Address and serial selection required.");
        var simulator = new Avp335SimulatedTransport(true);
        window.Simulation335TransportFactory = () => simulator;
        window.LoadDemoImage();
        window.StartButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
        for (int i = 0; !window.SettingsPanel.IsEnabled && i < 1500; i++) await Task.Delay(10);
        Assert(window.SettingsPanel.IsEnabled && window.Progress.Value == 100 &&
            window.StatusLabel.Text == "升级完成，启动请求已确认" &&
            simulator.CommitCount == 1 && !simulator.InBoot, "Upgrade and automatic APP start failed.");
        File.WriteAllText(report, JsonSerializer.Serialize(new
        {
            Result = "passed", Title = window.Title, Chip = "AVP32F335", Transport = "SCI", Baud = 115200,
            HiddenEntries = "passed", UpgradeWithLostAck = "passed", AutomaticAppStart = "passed",
            BootAppAddressQuery = "absent", Runtime = System.Diagnostics.Process.GetCurrentProcess().Modules
                .Cast<System.Diagnostics.ProcessModule>().FirstOrDefault(m => m.ModuleName == "coreclr.dll")?.FileName
        }, new JsonSerializerOptions { WriteIndented = true }));
    }
}
