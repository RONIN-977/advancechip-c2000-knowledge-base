using System.IO;
using System.Text.Json;
using System.Windows;
using System.Windows.Controls;
using Updater.Core;

namespace Updater.Desktop;

// Explicit simulation-only entry point; no physical discovery or transport is used.
internal static class Can0039UiSelfCheck
{
    internal static async Task Run(MainWindow window, string report)
    {
        var checks = new List<string>();
        void Assert(bool condition, string name)
        {
            if (!condition) throw new InvalidOperationException(name + "\n" + window.Log.Text);
            checks.Add(name);
        }
        void AssertFixedUi()
        {
            Assert(window.ChipModel.SelectedIndex == 0 && window.TransportMode.SelectedIndex == 1,
                "0039 and CAN remain selected");
            Assert(!window.ChipModel.IsEnabled && !window.TransportMode.IsEnabled,
                "model and communication selectors remain disabled");
            Assert(window.ProbeButton.Visibility == Visibility.Collapsed && window.ResetButton.Visibility == Visibility.Collapsed,
                "read-info and reset buttons remain hidden");
        }
        Assert(MainWindow.Customer0039CanEdition, "0039 CAN edition enabled");
        AssertFixedUi();
        Assert(window.ChipModel.Items.Count == 5 && window.TransportMode.Items.Count == 2,
            "all original model and transport entries retained");
        Assert(window.ChipModel.Items.Cast<ComboBoxItem>().Select((item, index) =>
            item.Visibility == (index == 0 ? Visibility.Visible : Visibility.Collapsed)).All(value => value),
            "only 0039 model entry visible");
        Assert(((ComboBoxItem)window.TransportMode.Items[0]).Visibility == Visibility.Collapsed &&
            ((ComboBoxItem)window.TransportMode.Items[1]).Visibility == Visibility.Visible,
            "SCI retained but hidden; CAN visible");
        Assert(window.AppStartAddress.Text == "0x087000" && !window.AppStartAddress.IsReadOnly && window.AppStartAddress.IsEnabled,
            "default APP start is editable 0x087000");
        window.AppStartAddress.Text = "0x088000";
        Assert(window.AppStartAddress.Text == "0x088000" && window.LoadButton.IsEnabled,
            "custom APP start remains supported");
        window.DefaultAddressButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
        Assert(window.AppStartAddress.Text == "0x087000", "restore-default returns to 0x087000");
        window.ChipModel.SelectedIndex = 1;
        window.TransportMode.SelectedIndex = 0;
        AssertFixedUi();
        Assert(window.AppStartAddress.Text == "0x087000", "selection guards preserve 0039 APP default");

        var transport = new RetryCountingTransport();
        window.SimulationTransportFactory = () => transport;
        window.LoadDemoImage();
        window.StartButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
        Assert(!window.SettingsPanel.IsEnabled, "settings locked during CAN simulation");
        for (int i = 0; !window.SettingsPanel.IsEnabled && i < 2000; i++) await Task.Delay(10);
        Assert(window.SettingsPanel.IsEnabled, "CAN simulation completed within timeout");
        Assert(window.Progress.Value == 100 && window.PercentLabel.Text == "100%" &&
            window.StatusLabel.Text == "升级完成，启动请求已确认", "full upgrade completed and automatic reset acknowledged");
        Assert(transport.DataRetries == 1, "lost DATA acknowledgement retried exactly once");
        AssertFixedUi();
        File.WriteAllText(report, JsonSerializer.Serialize(new
        {
            Result = "passed", Checks = checks, Chip = "AVP32F0039", Transport = "CAN",
            DefaultAppStart = "0x087000", LostAckDataRetries = transport.DataRetries,
            PhysicalDeviceAccess = "none", Runtime = Environment.ProcessPath
        }, new JsonSerializerOptions { WriteIndented = true }));
    }

    private sealed class RetryCountingTransport : ITransport
    {
        private readonly SimulatedTransport simulator = new(true, true, 1,
            DeviceProfile.F280039C.WithAppStart(0x087000));
        private readonly HashSet<string> dataRequests = new();
        internal int DataRetries { get; private set; }
        public Task<Packet> ExchangeAsync(Packet request, TimeSpan timeout, CancellationToken cancellation)
        {
            if (request.Command == (byte)Command.Data && !dataRequests.Add($"{request.Session}:{request.Sequence}")) DataRetries++;
            return simulator.ExchangeAsync(request, timeout, cancellation);
        }
        public ValueTask DisposeAsync() => simulator.DisposeAsync();
    }
}
