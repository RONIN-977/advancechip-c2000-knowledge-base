using Microsoft.Win32;
using System.Buffers.Binary;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using Updater.Core;

namespace Updater.Desktop;
public partial class MainWindow : Window
{
    private FirmwareImage? image;
    private Avp335FirmwareImage? avpImage;
    private bool Is335 => ChipModel.SelectedIndex == 1;
    private CancellationTokenSource? running;
    private bool ready;
    public MainWindow()
    {
        InitializeComponent();
        // WorkArea uses WPF logical units and excludes the taskbar.
        Width = Math.Min(Width, SystemParameters.WorkArea.Width);
        Height = Math.Min(Height, SystemParameters.WorkArea.Height);
        DllPath.Text = CanDriver.DefaultPath;
        ready = true; UpdateMode();
        Write($"程序为 {IntPtr.Size * 8} 位；32 位 CAN DLL 自动通过桥接加载。首次安装 Bootloader 需使用 CCS/JTAG。");
        if (File.Exists(DllPath.Text)) Write($"已自动关联 CAN 驱动：{DllPath.Text}");
        Write("设备复位后有 5 秒升级窗口；读取设备可保持在 Bootloader。应用无效时始终等待升级。");
    }
    private void Write(string text) { Log.AppendText($"[{DateTime.Now:HH:mm:ss}] {text}\n"); Log.ScrollToEnd(); }
    private void Mode_Changed(object sender, SelectionChangedEventArgs e) { if (ready) UpdateMode(); }
    private void Chip_Changed(object sender, SelectionChangedEventArgs e)
    {
        if (!ready) return;
        image = null; avpImage = null; Progress.Value = 0; StatusLabel.Text = "就绪";
        foreach (int index in new[] { 1, 3 }) ((ComboBoxItem)TransportMode.Items[index]).IsEnabled = !Is335;
        TransportMode.SelectedIndex = Is335 ? 0 : 1;
        ImageLabel.Text = Is335 ? "选择配套 335 APP 的 C28x COFF .out，自动生成 BIN 数据" : "直接选择 CCS 应用 .out（自动转换），也支持 SCI8 .txt / .hex 和 JSON";
        UpdateMode(); Write($"已选择 {(Is335 ? "AVP32F335 / SCI-A" : "F280039C")}，请重新载入对应固件。");
    }
    private void UpdateMode()
    {
        bool can = TransportMode.SelectedIndex is 1 or 3, real = TransportMode.SelectedIndex >= 2;
        BaudRate.ItemsSource = can ? new[] { 10000, 20000, 50000, 100000, 125000, 250000, 500000, 800000, 1000000 } : new[] { 9600, 19200, 38400, 57600, 115200, 230400, 460800 };
        BaudRate.SelectedItem = can ? 500000 : 115200;
        ComPort.Visibility = can ? Visibility.Collapsed : Visibility.Visible;
        CanModel.Visibility = can ? Visibility.Visible : Visibility.Collapsed;
        CanIndex.IsEnabled = CanChannel.IsEnabled = NodeId.IsEnabled = DllPath.IsEnabled = can;
        DropAck.IsEnabled = !real;
        ModeNotice.Text = real ? "实际设备模式：开始升级将擦写应用分区。请选择按 0x090000 重新链接的应用固件；上位机选择波特率不会自动修改设备波特率。" : "仿真模式：不打开 USB 设备、不擦写实际 Flash。可使用演示数据验证分包、校验与 ACK 丢失重传。";
        if (Is335)
        {
            BaudRate.ItemsSource = new[] { 115200 }; BaudRate.SelectedItem = 115200;
            ModeNotice.Text = real ? "335 SCI-A：GPIO28/RX、GPIO29/TX，115200 / 8N1。首次需通过 JTAG 烧录对应晶振的 V201 BOOT；升级前自动核对版本。请选择配套 APP 的 OUT。" : "335 SCI 仿真：可验证 128 字节分包、回读、MD5 与 ACK 丢失重传。实际升级需 V201 BOOT。";
        }
        PartitionLabel.Text = Is335 ? "335 APP：0x300000–0x337FFD · 最大 448 KB 减 4 字节 · SCI-A 115200 / 8N1" : "应用：0x090000–0x0AEFFF（16 位字地址） · 默认 SCI 115200 / CAN 500k";
        StartButton.Content = real ? "开始升级" : "开始仿真升级";
    }
    private void ShowImage(string name)
    {
        if (Is335)
        {
            ImageLabel.Text = $"{name}\n{avpImage!.Bytes.Length:N0} 字节 · MD5 {Convert.ToHexString(avpImage.Digest)} · 入口 0x300000";
            Write($"已载入 {name}，{avpImage.Bytes.Length:N0} 字节；空洞填充 0xFFFF，有效标志由 BOOT 提交。"); return;
        }
        ImageLabel.Text = $"{name}\n{image!.Words.Length * 2:N0} 字节 · CRC32 {Crc32.Compute(image.Bytes):X8} · 入口 0x{image.EntryWordAddress:X6}";
        Write($"已载入 {name}，{image.Words.Length:N0} 个 16 位字。");
    }
    private void Demo_Click(object sender, RoutedEventArgs e) { if (Is335) avpImage = Avp335FirmwareImage.Demo(); else image = FirmwareImage.Demo(); ShowImage("演示数据（仅仿真）"); }
    private async void Load_Click(object sender, RoutedEventArgs e)
    {
        var dialog = new OpenFileDialog
        {
            Filter = Is335 ? "335 APP OUT (*.out)|*.out" : "升级固件 (*.out;*.txt;*.hex;*.json)|*.out;*.txt;*.hex;*.json|CCS 应用 (*.out)|*.out|TI SCI8 文件 (*.txt;*.hex)|*.txt;*.hex|JSON 固件包 (*.json)|*.json"
        };
        if (dialog.ShowDialog() != true) return;

        image = null; avpImage = null;
        using var cancellation = new CancellationTokenSource();
        running = cancellation;
        SetBusy(true);
        ImageLabel.Text = "正在载入并校验固件…";
        try
        {
            if (Is335)
            {
                avpImage = await Task.Run(() => Avp335OutLoader.Load(dialog.FileName), cancellation.Token);
                Write("335 OUT 装载段、codestart、有效标志和分区边界检查通过。");
            }
            else if (Path.GetExtension(dialog.FileName).Equals(".out", StringComparison.OrdinalIgnoreCase))
            {
                StatusLabel.Text = "正在查找 TI 转换工具…";
                string? converter = await Task.Run(TiToolLocator.Find, cancellation.Token);
                cancellation.Token.ThrowIfCancellationRequested();
                if (converter is null)
                {
                    Write("未自动找到 hex2000.exe，请选择 CCS 的 tools/compiler/ti-cgt-c2000_*/bin/hex2000.exe；此路径会保存供下次使用。");
                    var picker = new OpenFileDialog { Title = "选择 TI C2000 转换工具 hex2000.exe", Filter = "TI hex2000|hex2000.exe" };
                    if (picker.ShowDialog() != true)
                    {
                        ImageLabel.Text = "尚未选择转换工具，未载入固件。";
                        StatusLabel.Text = "已取消载入";
                        return;
                    }
                    converter = picker.FileName;
                    try { TiToolLocator.Remember(converter); }
                    catch (Exception ex) when (ex is IOException or UnauthorizedAccessException)
                    {
                        Write($"工具路径未能保存，本次仍继续转换：{ex.Message}");
                    }
                }
                Write($"自动转换 .out，TI 工具：{converter}");
                StatusLabel.Text = "正在转换并校验 .out…";
                image = await Task.Run(() => TiOutConverter.LoadAsync(dialog.FileName, converter, cancellation.Token));
                Write(".out 转换完成；入口、分区边界和镜像检查通过。");
            }
            else
            {
                image = await Task.Run(() => FirmwareImage.Load(dialog.FileName), cancellation.Token);
            }
            cancellation.Token.ThrowIfCancellationRequested();
            ShowImage(Path.GetFileName(dialog.FileName));
            StatusLabel.Text = "固件已就绪，可开始升级";
        }
        catch (OperationCanceledException)
        {
            image = null; avpImage = null;
            ImageLabel.Text = "固件载入已取消";
            StatusLabel.Text = "已取消载入";
        }
        catch (Exception ex)
        {
            image = null; avpImage = null;
            ImageLabel.Text = "固件载入失败";
            StatusLabel.Text = "固件载入失败";
            Write(ex.Message);
        }
        finally
        {
            running = null;
            SetBusy(false);
        }
    }
    private void Dll_Click(object sender, RoutedEventArgs e)
    {
        var dialog = new OpenFileDialog { Filter = "CAN 驱动库|ControlCAN.dll;usbcan.dll|动态链接库|*.dll" };
        if (dialog.ShowDialog() == true) DllPath.Text = dialog.FileName;
    }
    private async Task<ITransport> OpenTransport()
    {
        int mode = TransportMode.SelectedIndex, baud = (int)BaudRate.SelectedItem;
        if (mode < 2) return new SimulatedTransport(mode == 1, DropAck.IsChecked == true, 15);
        string port = ComPort.Text.Trim(), dll = DllPath.Text.Trim();
        if (mode == 2) return await Task.Run<ITransport>(() => new SerialTransport(port, baud));
        uint type = CanModel.SelectedIndex == 0 ? 3u : 4u;
        uint index = uint.Parse(CanIndex.Text), channel = (uint)CanChannel.SelectedIndex, node = uint.Parse(NodeId.Text);
        return await Task.Run<ITransport>(() => new ControlCanTransport(dll, type, index, channel, baud, node));
    }
    private async Task<IAvp335Transport> Open335Transport()
    {
        if (TransportMode.SelectedIndex == 0) return new Avp335SimulatedTransport(DropAck.IsChecked == true);
        if (TransportMode.SelectedIndex != 2) throw new InvalidOperationException("335 本次仅支持 SCI。");
        string port = ComPort.Text.Trim();
        return await Task.Run<IAvp335Transport>(() => new Avp335SerialTransport(port));
    }
    private async Task Update335(IProgress<UpdateProgress> progress, CancellationToken cancellation)
    {
        await using var transport = await Open335Transport();
        var client = new Avp335UpdaterClient(transport);
        await client.UpdateAsync(avpImage!, progress, cancellation);
        Write("335 整包 MD5 与 Flash 回读通过，有效标志已提交。");
        if (transport is Avp335SimulatedTransport sim) Write($"仿真计数：擦除 {sim.EraseCount}，写入 {sim.ProgramCount}，提交 {sim.CommitCount}。");
        if (AutoReset.IsChecked == true)
        {
            try { await client.ResetAsync(cancellation); Write("335 已确认启动 APP 请求。"); }
            catch (Exception ex) { Write($"固件已提交，复位应答未确认：{ex.Message} 可手动复位。"); }
        }
    }
    private async void Start_Click(object sender, RoutedEventArgs e)
    {
        if (Is335 ? avpImage is null : image is null) { Write("请先选择固件。"); return; }
        if ((Is335 ? avpImage!.SimulationOnly : image!.SimulationOnly) && TransportMode.SelectedIndex >= 2) { Write("演示数据不是可执行固件，只能用于仿真。"); return; }
        running = new CancellationTokenSource(); SetBusy(true); Progress.Value = 0;
        try
        {
            var progress = new Progress<UpdateProgress>(p => { Progress.Value = p.Percent; StatusLabel.Text = p.Message; });
            if (Is335) { await Update335(progress, running.Token); return; }
            await using var transport = await OpenTransport();
            Write("通信通道已打开，开始读取设备与升级。");
            var client = new UpdaterClient(transport);
            await client.UpdateAsync(image!, progress, running.Token);
            Write("应用 CRC32 校验通过，有效标记已提交。");
            if (transport is SimulatedTransport sim) Write($"仿真计数：擦除 {sim.Device.EraseCount}，写入 {sim.Device.ProgramCount}，提交 {sim.Device.CommitCount}。");
            if (AutoReset.IsChecked == true)
            {
                try { await client.ResetAsync(running.Token); Write("设备已确认复位请求。"); }
                catch (Exception ex) { Write($"固件已经提交，但复位应答未确认：{ex.Message} 可手动复位设备。"); }
            }
        }
        catch (OperationCanceledException) { StatusLabel.Text = "传输已取消"; Write(Is335 ? "已取消 335 升级；等待当前擦写操作完成，保持在 BOOT，重新开始升级。" : "已取消传输；若设备仍在擦除，需等待其完成。未提交镜像不会启动，重连后从头升级。"); }
        catch (Exception ex) { StatusLabel.Text = "升级失败"; Write(Explain(ex)); }
        finally { running.Dispose(); running = null; SetBusy(false); }
    }
    private async void Probe_Click(object sender, RoutedEventArgs e)
    {
        SetBusy(true);
        try
        {
            if (Is335)
            {
                await using var sci = await Open335Transport();
                var info = await new Avp335UpdaterClient(sci).ProbeAsync(CancellationToken.None);
                Write($"335 状态 {(info.InBoot ? "BOOT" : "APP")}，BOOT 版本 {info.BootVersion}，APP 版本 {info.AppVersion}；升级需要 BOOT V201。"); return;
            }
            await using var transport = await OpenTransport();
            var request = new Packet((byte)Command.Info, 0, 0, []);
            var reply = await transport.ExchangeAsync(request, TimeSpan.FromSeconds(3), CancellationToken.None);
            if (!ReplyMatch.Is(reply, request) || reply.Payload.Length != 24 || reply.Payload[0] != 0) throw new InvalidDataException("设备信息应答格式错误。");
            Write($"设备 0x{BinaryPrimitives.ReadUInt32LittleEndian(reply.Payload.AsSpan(6)):X8}，状态 {(BootState)reply.Payload[1]}，应用起点 0x{BinaryPrimitives.ReadUInt32LittleEndian(reply.Payload.AsSpan(16)):X6}。");
        }
        catch (Exception ex) { Write(Explain(ex)); }
        finally { SetBusy(false); }
    }
    private async void Reset_Click(object sender, RoutedEventArgs e)
    {
        SetBusy(true);
        try { if (Is335) { await using var sci = await Open335Transport(); await new Avp335UpdaterClient(sci).ResetAsync(CancellationToken.None); } else { await using var transport = await OpenTransport(); await new UpdaterClient(transport).ResetAsync(CancellationToken.None); } Write("设备已确认复位请求。"); }
        catch (Exception ex) { Write(Explain(ex)); }
        finally { SetBusy(false); }
    }
    private async void Enter_Click(object sender, RoutedEventArgs e)
    {
        SetBusy(true);
        try { if (Is335) { await using var sci = await Open335Transport(); await new Avp335UpdaterClient(sci).EnterBootAsync(CancellationToken.None); } else { await using var transport = await OpenTransport(); await new UpdaterClient(transport).EnterBootAsync(CancellationToken.None); } Write("设备已进入升级模式。"); }
        catch (Exception ex) { Write(Explain(ex)); }
        finally { SetBusy(false); }
    }
    private void SaveLog_Click(object sender, RoutedEventArgs e)
    {
        var dialog = new SaveFileDialog { Filter = "文本日志|*.txt", FileName = $"upgrade-{DateTime.Now:yyyyMMdd-HHmmss}.txt" };
        if (dialog.ShowDialog() == true) { try { File.WriteAllText(dialog.FileName, Log.Text); } catch (Exception ex) { Write(ex.Message); } }
    }
    private static string Explain(Exception ex) => ex is BadImageFormatException ? "ControlCAN.dll 与程序位数不匹配，请使用对应的官方 x64 / x86 库。" : ex.Message;
    private void Cancel_Click(object sender, RoutedEventArgs e) => running?.Cancel();
    private void SetBusy(bool busy)
    {
        StartButton.IsEnabled = LoadButton.IsEnabled = DemoButton.IsEnabled = SettingsPanel.IsEnabled = ProbeButton.IsEnabled = EnterButton.IsEnabled = ResetButton.IsEnabled = AutoReset.IsEnabled = !busy;
        CancelButton.IsEnabled = busy;
    }
    protected override void OnClosed(EventArgs e) { running?.Cancel(); base.OnClosed(e); }
}
