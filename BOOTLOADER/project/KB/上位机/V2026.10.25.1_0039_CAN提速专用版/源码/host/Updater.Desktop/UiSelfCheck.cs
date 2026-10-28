using System.IO;
using System.Text;
using System.Windows;
using System.Windows.Controls;

namespace Updater.Desktop;

// Explicit command-line self-check only; never enumerates or opens physical devices.
internal static class UiSelfCheck
{
    public static async Task Run(MainWindow window, string output)
    {
        var checks = new List<string>();
        void Assert(bool condition, string name)
        {
            if (!condition) throw new InvalidOperationException(name + "\n" + window.Log.Text);
            checks.Add(name);
        }
        async Task Idle(MainWindow target)
        {
            for (int i = 0; !target.SettingsPanel.IsEnabled && i < 2000; i++) await Task.Delay(10);
            if (!target.SettingsPanel.IsEnabled) throw new TimeoutException("UI operation did not finish");
            await Task.Delay(20);
        }
        Assert(window.Width == 800 && window.Height == 620, "default 800x620 window");
        Assert(window.TransportMode.Items.Count == 2 && window.TransportMode.SelectedIndex == 0,
            "real SCI default; no simulation controls");
        Assert(!window.CanOptions.IsEnabled && !window.CanChannelPanel.IsEnabled, "SCI disables CAN parameters");
        Assert(window.FindName("EnterButton") is null && window.StartButton.Content.ToString() == "开始升级" && window.FindName("AutoReset") is null, "one-click upgrade; automatic reset has no checkbox");
        Assert(!window.ComPort.IsEditable && !window.CanModel.IsEditable, "devices cannot be typed manually");
        string[] names = window.ChipModel.Items.Cast<ComboBoxItem>().Select(item => item.Content.ToString()!).ToArray();
        Assert(names.SequenceEqual(new[] { "AVP32F0039", "AVP32F335", "AVP32F0049", "ADP32F035", "AVP32F00157" }), "Advancechip display names");
        for (int chip=0; chip<names.Length; chip++)
        {
            window.ChipModel.SelectedIndex=chip;
            string original=window.AppStartAddress.Text;
            uint start=Convert.ToUInt32(original[2..],16)+0x1000U;
            window.LoadDemoImage(); window.AppStartAddress.Text=$"0x{start:X6}";
            Assert(window.ImageLabel.Text.Contains("重新选择") && window.Progress.Value==0,"address edit clears loaded image");
            foreach(int mode in chip==1?new[]{0}:new[]{0,1})
            {
                window.TransportMode.SelectedIndex=mode;
                window.LoadDemoImage();window.StartButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                Assert(!window.SettingsPanel.IsEnabled,"custom address locked during update");
                await Idle(window);Assert(window.Progress.Value==100,$"custom {names[chip]} mode {mode} update");
            }
            window.AppStartAddress.Text="0xFFFFFFFF";
            Assert(!window.LoadButton.IsEnabled && !window.StartButton.IsEnabled,"invalid address blocks load/update");
            window.DefaultAddressButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
            Assert(window.AppStartAddress.Text==original && window.LoadButton.IsEnabled,"restore default address");
        }
        window.ChipModel.SelectedIndex=4;window.AppStartAddress.Text="08A000";
        window.ChipModel.SelectedIndex=0;window.ChipModel.SelectedIndex=4;
        Assert(window.AppStartAddress.Text=="08A000","per-model address retained within session");
        window.DefaultAddressButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
        window.ChipModel.SelectedIndex=0;window.TransportMode.SelectedIndex=0;
        string serial = Convert.ToBase64String(Encoding.ASCII.GetBytes("TEST001".PadRight(20, '\0')));
        var devices = DeviceDiscovery.ParseCanDevices($"3\t0\t1\t{serial}\n4\t2\t2\t{serial}\n4\t2\t2\t{serial}\n");
        Assert(devices.Length == 2 && devices[0].Type == 3 && devices[1].Index == 2, "CAN discovery parsing and deduplication");
        bool rejected = false;
        try { DeviceDiscovery.ParseCanDevices($"3\t0\t2\t{serial}\n"); } catch (InvalidDataException) { rejected = true; }
        Assert(rejected && DeviceDiscovery.ParseCanDevices("").Length == 0, "invalid and empty discovery results");
        window.TransportMode.SelectedIndex = 1;
        window.CanModel.ItemsSource = devices; window.CanModel.SelectedIndex = 0;
        Assert(window.CanOptions.IsEnabled && window.CanChannel.Items.Count == 1, "CAN enables fields and one-channel adapter");
        window.CanModel.SelectedIndex = 1;
        Assert(window.CanChannel.Items.Count == 2 && window.CanChannel.SelectedIndex == 0, "two-channel adapter selection");
        window.ChipModel.SelectedIndex = 1;
        Assert(window.TransportMode.SelectedIndex == 0 && !((ComboBoxItem)window.TransportMode.Items[1]).IsEnabled &&
            window.BaudRate.Items.Count == 1 && (int)window.BaudRate.SelectedItem == 115200, "335 SCI-only constraint");
        for (int chip = 0; chip < names.Length; chip++)
        {
            window.ChipModel.SelectedIndex = chip;
            foreach (int mode in chip == 1 ? new[] { 0 } : new[] { 0, 1 })
            {
                window.TransportMode.SelectedIndex = mode;
                window.LoadDemoImage();
                window.StartButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                Assert(!window.SettingsPanel.IsEnabled && !window.RefreshButton.IsEnabled && window.CancelButton.IsEnabled,
                    $"{names[chip]} mode {mode}: settings and discovery locked during update");
                double percent = window.Progress.Value;
                window.ClearLogForCheck();
                Assert(window.Log.Text.Length == 0 && window.Progress.Value == percent && !window.SettingsPanel.IsEnabled,
                    $"{names[chip]} mode {mode}: clearing log preserves active task");
                await Idle(window);
                Assert(window.Progress.Value == 100 && window.PercentLabel.Text == "100%" &&
                    window.StatusLabel.Text == "升级完成，启动请求已确认" && window.Log.Text.Length > 0,
                    $"{names[chip]} mode {mode}: lost-ACK simulation, success status and subsequent logs");
            }
        }
        window.ChipModel.SelectedIndex = 1;
        window.ResetButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent)); await Idle(window);
        Assert(window.StatusLabel.Text == "操作失败，请查看日志", "fresh 335 simulator refuses starting an uncommitted image");
        window.ComPort.ItemsSource = new[] { "COM3", "COM4" }; window.ComPort.SelectedIndex = 0;
        Assert(window.ResetButton.Content.ToString() == "启动 / 重启程序", "changing device invalidates cached 335 state");
        var finish = typeof(MainWindow).GetMethod("FinishUpgrade", System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.NonPublic)!;
        await (Task)finish.Invoke(window, new object[] { (Func<Task>)(() => Task.FromException(new IOException("self-check lost reset acknowledgement"))) })!;
        Assert(window.StatusLabel.Text == "升级完成，启动未确认" && window.Progress.Value == 100, "unconfirmed startup is not upgrade failure");
        window.ChipModel.SelectedIndex = 0; window.LoadDemoImage();
        window.StartButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
        window.CancelButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent)); await Idle(window);
        Assert(window.StatusLabel.Text.Contains("升级已停止") && window.Progress.Value < 100, "stop update cancellation status");
        var vendors = new MainWindow(false);
        vendors.TransportMode.SelectedIndex = 1;
        var zlg = new DetectedCanDevice(4, 0, 2, "SAME", "周立功", @"C:\local drivers\zlg\usbcan.dll");
        var cx = new DetectedCanDevice(4, 0, 2, "SAME", "创芯科技", @"C:\local drivers\创芯\ControlCAN.dll");
        vendors.CanModel.ItemsSource = new[] { zlg, cx };
        vendors.CanModel.SelectedItem = zlg;
        Assert(vendors.DllPath.Text == zlg.DriverPath && vendors.CanChannel.Items.Count == 2,
            "ZLG selection binds its local DLL and channels");
        vendors.CanModel.SelectedItem = cx;
        Assert(vendors.DllPath.Text == cx.DriverPath && vendors.CanChannel.SelectedIndex == 0,
            "Chuangxin selection switches local DLL automatically");
        Assert(((ComboBoxItem)vendors.TransportMode.Items[1]).Content.ToString() == "CAN",
            "CAN requires no manual vendor selection");
        var real = new MainWindow(false);
        real.ProbeButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent)); await Idle(real);
        Assert(real.StatusLabel.Text == "操作失败，请查看日志" && real.Log.Text.Contains("请连接串口"), "missing serial selection never opens transport");
        real.TransportMode.SelectedIndex = 1;
        real.ProbeButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent)); await Idle(real);
        Assert(real.Log.Text.Contains("请连接 CAN 盒"), "missing CAN selection never opens transport");
        real.StartButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
        Assert(real.StatusLabel.Text == "请先选择升级文件", "missing file is explained");
        checks.AddRange(await OneClickSelfCheck.Run());
        foreach (var size in new[] { new Size(800, 620), new Size(640, 480) })
        {
            var content = (FrameworkElement)window.Content;
            window.TopArea.MaxHeight = size.Height - 180;
            content.Measure(size); content.Arrange(new Rect(size)); content.UpdateLayout();
            Point logPosition = window.Log.TranslatePoint(new Point(), content);
            Assert(window.Log.ActualHeight >= 70 && logPosition.Y + window.Log.ActualHeight <= size.Height,
                $"log area remains accessible at {size.Width}x{size.Height} (height {window.Log.ActualHeight}, bottom {logPosition.Y + window.Log.ActualHeight})");
        }
        File.WriteAllText(output, System.Text.Json.JsonSerializer.Serialize(new
        {
            Checks = checks, SimulatedUpgradePaths = names.Length * 2 - 1, PhysicalDeviceAccess = "none",
            CoreLogic = "custom APP start; legacy default profiles preserved", Runtime = Environment.ProcessPath
        }, new System.Text.Json.JsonSerializerOptions { WriteIndented = true }));
    }
}
