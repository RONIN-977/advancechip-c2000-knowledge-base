using System.IO;
using System.Windows;
using System.Windows.Controls;
using Updater.Core;

namespace Updater.Desktop;

// Command-line self-check only. All fault injection wraps the unchanged core simulators.
internal static class OneClickSelfCheck
{
    public static async Task<List<string>> Run()
    {
        var checks = new List<string>();
        const string confirmation = "与设备程序通讯正常，设备信息读取成功。";
        void Check(bool condition, string name, MainWindow window)
        {
            if (!condition) throw new InvalidOperationException(name + "\n" + window.Log.Text);
            checks.Add(name);
        }
        MainWindow Window(int chip, int mode)
        {
            var window = new MainWindow(false);
            window.EnableSimulation(); window.ChipModel.SelectedIndex = chip; window.TransportMode.SelectedIndex = mode;
            return window;
        }
        async Task Idle(MainWindow window)
        {
            for (int i = 0; !window.SettingsPanel.IsEnabled && i < 2000; i++) await Task.Delay(10);
            if (!window.SettingsPanel.IsEnabled) throw new TimeoutException("One-click self-check timed out");
            await Task.Delay(20);
        }
        DeviceProfile Profile(int chip) => chip == 4 ? DeviceProfile.F2800157 : chip == 2 ? DeviceProfile.F280049 : chip == 3 ? DeviceProfile.F280035 : DeviceProfile.F280039C;
        for (int chip = 0; chip < 5; chip++)
        foreach (int mode in chip == 1 ? new[] { 0 } : new[] { 0, 1 })
        {
            string prefix = $"chip {chip}, mode {mode}";
            foreach (bool app in new[] { true, false })
            {
                var window = Window(chip, mode); int opens = 0;
                var general = new GeneralWire(mode == 1, Profile(chip), app);
                var avp = new AvpWire(app);
                window.SimulationTransportFactory = () => { opens++; return general; };
                window.Simulation335TransportFactory = () => { opens++; return avp; };
                window.LoadDemoImage(); window.StartButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                await Idle(window);
                Check(opens == 1 && window.Progress.Value == 100 && window.StatusLabel.Text == "升级完成，启动请求已确认",
                    $"{prefix}, {(app ? "APP" : "BOOT")}: one click, one connection, automatic reset", window);
                Check(chip == 1
                    ? avp.SwitchesFromApp == (app ? 1 : 0) && avp.Inner.EraseCount == 1 && avp.Inner.CommitCount == 1 && avp.Commands.Count(c => c == 0x75) == 1
                    : general.Commands.IndexOf((byte)Command.EnterBoot) < general.Commands.IndexOf((byte)Command.Begin) &&
                      general.Inner.Device.EraseCount == 1 && general.Inner.Device.CommitCount == 1 && general.Commands.Count(c => c == (byte)Command.Reset) == 1,
                    $"{prefix}, {(app ? "APP" : "BOOT")}: enter before erase; no duplicate commit/reset", window);
            }
            foreach (string fault in new[] { "entry-error", "entry-timeout", "entry-cancel" })
            {
                var window = Window(chip, mode);
                var general = new GeneralWire(mode == 1, Profile(chip), true, fault);
                var avp = new AvpWire(true, fault);
                window.SimulationTransportFactory = () => general; window.Simulation335TransportFactory = () => avp;
                window.LoadDemoImage(); window.StartButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                if (fault == "entry-cancel") window.CancelButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                await Idle(window);
                Check(chip == 1 ? avp.Inner.EraseCount == 0 && !avp.Commands.Contains(0x70)
                                : general.Inner.Device.EraseCount == 0 && !general.Commands.Contains((byte)Command.Begin),
                    $"{prefix}, {fault}: no erase command", window);
                Check(window.StatusLabel.Text.Contains(fault == "entry-cancel" ? "升级已停止" : "升级失败"),
                    $"{prefix}, {fault}: explicit failure/cancellation result", window);
            }
            foreach (string fault in new[] { "", "info-timeout", "info-length", "info-status", "info-identity" })
            {
                var window = Window(chip, mode);
                var general = new GeneralWire(mode == 1, Profile(chip), false, fault);
                var avp = new AvpWire(false, fault);
                window.SimulationTransportFactory = () => general; window.Simulation335TransportFactory = () => avp;
                window.ProbeButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent)); await Idle(window);
                Check(fault.Length == 0 ? window.Log.Text.Contains(confirmation) && window.StatusLabel.Text.StartsWith("通讯正常")
                                        : !window.Log.Text.Contains(confirmation) && window.StatusLabel.Text == "操作失败，请查看日志",
                    $"{prefix}, probe {(fault.Length == 0 ? "valid reply" : fault)}: communication result is truthful", window);
                Check(chip == 1 ? avp.Inner.EraseCount == 0 && avp.Commands.All(c => c == 1)
                                : general.Inner.Device.EraseCount == 0 && general.Commands.All(c => c == (byte)Command.Info),
                    $"{prefix}, probe: information request only", window);
            }
        }
        return checks;
    }

    private sealed class GeneralWire(bool can, DeviceProfile profile, bool inApp, string fault = "") : ITransport
    {
        public SimulatedTransport Inner { get; } = new(can, true, 0, profile);
        public List<byte> Commands { get; } = [];
        private bool app = inApp;
        public async Task<Packet> ExchangeAsync(Packet request, TimeSpan timeout, CancellationToken cancellation)
        {
            Commands.Add(request.Command); cancellation.ThrowIfCancellationRequested();
            if (request.Command == (byte)Command.EnterBoot)
            {
                if (fault == "entry-cancel") await Task.Delay(1000, cancellation);
                if (fault == "entry-timeout") throw new TimeoutException("自检：进入升级无应答");
                if (fault == "entry-error") return new Packet((byte)(request.Command | 0x80), request.Session, request.Sequence, [(byte)Status.BadState, 0, 0, 0, 0, 0]);
                app = false;
            }
            if (request.Command == (byte)Command.Begin && app) throw new IOException("自检：APP 尚未进入 BOOT，禁止擦写");
            if (request.Command == (byte)Command.Info && fault == "info-timeout") throw new TimeoutException("自检：读取设备信息无应答");
            var reply = await Inner.ExchangeAsync(request, timeout, cancellation);
            if (request.Command != (byte)Command.Info) return reply;
            if (fault == "info-length") return reply with { Payload = reply.Payload[..^1] };
            if (fault == "info-status") { var payload = reply.Payload.ToArray(); payload[0] = (byte)Status.BadState; return reply with { Payload = payload }; }
            if (fault == "info-identity") return reply with { Command = 0xff };
            return reply;
        }
        public ValueTask DisposeAsync() => Inner.DisposeAsync();
    }

    private sealed class AvpWire(bool inApp, string fault = "") : IAvp335Transport
    {
        public Avp335SimulatedTransport Inner { get; } = new(true) { InBoot = !inApp };
        public List<ushort> Commands { get; } = [];
        public int SwitchesFromApp { get; private set; }
        public async Task<Avp335Packet> ExchangeAsync(Avp335Packet request, TimeSpan timeout, CancellationToken cancellation)
        {
            Commands.Add(request.Command); cancellation.ThrowIfCancellationRequested();
            if (request.Command == 0x72)
            {
                if (fault == "entry-cancel") await Task.Delay(1000, cancellation);
                if (fault == "entry-timeout") throw new TimeoutException("自检：335 进入升级无应答");
                if (fault == "entry-error") return new Avp335Packet(request.Command, request.Sequence, [5], Inner.InBoot ? (byte)0x89 : (byte)0x81);
                if (!Inner.InBoot) SwitchesFromApp++;
            }
            if (request.Command == 1 && fault == "info-timeout") throw new TimeoutException("自检：335 读取设备信息无应答");
            var reply = await Inner.ExchangeAsync(request, timeout, cancellation);
            if (request.Command != 1) return reply;
            if (fault == "info-length") return reply with { Payload = reply.Payload[..^1] };
            if (fault == "info-status") { var payload = reply.Payload.ToArray(); payload[0] = 5; return reply with { Payload = payload }; }
            if (fault == "info-identity") return reply with { Sequence = (ushort)(request.Sequence + 1) };
            return reply;
        }
        public ValueTask DisposeAsync() => Inner.DisposeAsync();
    }
}
