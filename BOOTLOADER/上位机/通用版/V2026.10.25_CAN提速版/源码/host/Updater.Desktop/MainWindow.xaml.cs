using Microsoft.Win32;
using System.Buffers.Binary;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using Updater.Core;

namespace Updater.Desktop;
public partial class MainWindow : Window
{
    internal static bool Customer335Edition =>
#if CUSTOMER335_ONLY
        true;
#else
        false;
#endif
    private FirmwareImage? image;
    private Avp335FirmwareImage? avpImage;
    private bool Is335 => ChipModel.SelectedIndex == 1;
    private DeviceProfile DefaultProfile => ChipModel.SelectedIndex switch
    {
        2 => DeviceProfile.F280049,
        3 => DeviceProfile.F280035,
        4 => DeviceProfile.F2800157,
        _ => DeviceProfile.F280039C
    };
    private DeviceProfile SelectedProfile => Is335 ? DefaultProfile : DefaultProfile.WithAppStart(SelectedAppStart);
    private uint SelectedAppStart
    {
        get
        {
            string text = AppStartAddress.Text.Trim();
            if (text.StartsWith("0x", StringComparison.OrdinalIgnoreCase)) text = text[2..];
            if (!uint.TryParse(text, System.Globalization.NumberStyles.AllowHexSpecifier,
                System.Globalization.CultureInfo.InvariantCulture, out uint address))
                throw new InvalidDataException("请输入有效的十六进制 APP 字地址，例如 0x088000。");
            return Is335 ? Avp335Profile.ValidateStart(address) : DefaultProfile.WithAppStart(address).AppStart;
        }
    }
    private readonly Dictionary<int, string> appAddresses = new();
    private bool changingAddress;
    private bool AddressValid { get { try { _ = SelectedAppStart; return true; } catch (InvalidDataException) { return false; } } }
    private void RestoreAddress()
    {
        changingAddress = true;
        AppStartAddress.Text = appAddresses.GetValueOrDefault(ChipModel.SelectedIndex,
            $"0x{(Is335 ? Avp335Profile.AppStart : DefaultProfile.AppStart):X6}");
        changingAddress = false;
    }
    private void DefaultAddress_Click(object sender, RoutedEventArgs e) =>
        AppStartAddress.Text = $"0x{(Is335 ? Avp335Profile.AppStart : DefaultProfile.AppStart):X6}";
    private void AppStart_Changed(object sender, TextChangedEventArgs e)
    {
        if (!ready || changingAddress) return;
        appAddresses[ChipModel.SelectedIndex] = AppStartAddress.Text;
        image = null; avpImage = null; loadedPath = null;
        ImageLabel.Text = "地址已修改，请重新选择升级文件";
        ImageLabel.ToolTip = null; ImageSummaryLabel.Text = "";
        Progress.Value = 0; PercentLabel.Text = "0%";
        RefreshPartition(); UpdateControls();
        SetStatus(AddressValid ? "地址已修改，请重新选择匹配的 APP 文件" : "APP 起始地址无效，请检查输入", "notice");
    }
    private void RefreshPartition()
    {
        try
        {
            uint start = SelectedAppStart, end = Is335 ? Avp335Profile.AppEnd : DefaultProfile.AppEnd;
            PartitionLabel.Text = $"{ChipName} 应用：0x{start:X6}–0x{end-1:X6}（16 位字地址） · 可用 {(end-start)*2:N0} 字节";
            ModeNotice.ToolTip = $"APP 必须按 0x{start:X6} 链接；此设置不会修改设备中的 BOOT。结束地址保持原型号配置。";
        }
        catch (InvalidDataException ex) { PartitionLabel.Text = ex.Message; }
    }
    private CancellationTokenSource? running;
    private bool ready, busy, discovering, simulation, dropOneAck, committed;
    private readonly bool automaticDiscovery;
    private bool? known335InBoot;
    private string operation = "";
    private string? loadedPath;
    private CancellationTokenSource? discoveryCancellation;
    private string? manualCanDriverPath;
    private bool IsCan => TransportMode.SelectedIndex == 1;
    private string ChipName => ((ComboBoxItem)ChipModel.SelectedItem).Content.ToString()!;
    public MainWindow(bool discoverDevices = true)
    {
        automaticDiscovery = discoverDevices;
        InitializeComponent();
        if (Customer335Edition)
        {
            Title = WindowHeading.Text = "335 SCI 固件升级工具";
            ChipModel.SelectedIndex = 1; ChipModel.IsEnabled = false;
            for (int i = 0; i < ChipModel.Items.Count; i++)
                if (i != 1) ((ComboBoxItem)ChipModel.Items[i]).Visibility = Visibility.Collapsed;
            TransportMode.SelectedIndex = 0; TransportMode.IsEnabled = false;
            ((ComboBoxItem)TransportMode.Items[1]).Visibility = Visibility.Collapsed;
            BaudRate.IsEnabled = false;
            ProbeButton.Visibility = ResetButton.Visibility = CanOptions.Visibility = CanChannelPanel.Visibility = Visibility.Collapsed;
            SettingsPanel.ColumnDefinitions[3].Width = SettingsPanel.ColumnDefinitions[4].Width = new GridLength(0);
            StartButton.ToolTip = "点击后等待 BOOT 9 秒，请在等待期间给板子上电或复位；握手成功后自动升级并请求启动 APP。";
        }
        Width = Math.Min(Width, SystemParameters.WorkArea.Width);
        Height = Math.Min(Height, SystemParameters.WorkArea.Height);
        DllPath.Text = CanDriver.DefaultPath;
        RestoreAddress(); ready = true; UpdateMode();
        Write("上位机版本 V2026.10.25（CAN提速版，DATA帧间隔1ms，控制报文保留原节拍）。");
        Write($"程序为 {IntPtr.Size * 8} 位；首次安装 BOOT 程序需使用 CCS/JTAG。");
        if (!Customer335Edition && File.Exists(DllPath.Text)) Write($"默认 CAN 驱动（刷新时自动检测周立功及创芯）：{DllPath.Text}");
        Write(Customer335Edition ? "请提前连接并选好串口、加载 APP 文件；点击开始升级后，在 9 秒等待期间给板子上电或复位。" :
            "请选择芯片、通信设备及配套应用文件；设备复位后的升级窗口由 BOOT 程序决定。");
        if (discoverDevices) Loaded += async (_, _) => await RefreshDevicesAsync();
    }
    private void Window_SizeChanged(object sender, SizeChangedEventArgs e)
    {
        if (TopArea is not null) TopArea.MaxHeight = Math.Max(180, ActualHeight - 180);
    }
    private void Write(string text) { communicationTrace?.Record("ui_log", new { message = text }); Log.AppendText($"[{DateTime.Now:HH:mm:ss}] {text}\n"); Log.ScrollToEnd(); }
    private void SetStatus(string text, string kind = "normal")
    {
        StatusLabel.Text = text;
        StatusLabel.Foreground = new SolidColorBrush((Color)ColorConverter.ConvertFromString(kind switch
        { "success" => "#68D8CE", "error" => "#FF9696", "notice" => "#F2C779", _ => "#DCE5F3" }));
    }
    private async void Mode_Changed(object sender, SelectionChangedEventArgs e)
    {
        if (!ready) return;
        if (Customer335Edition && TransportMode.SelectedIndex != 0) { TransportMode.SelectedIndex = 0; return; }
        known335InBoot = null; UpdateMode();
        if (automaticDiscovery && !simulation) await RefreshDevicesAsync();
    }
    private void Chip_Changed(object sender, SelectionChangedEventArgs e)
    {
        if (!ready) return;
        if (Customer335Edition && ChipModel.SelectedIndex != 1) { ChipModel.SelectedIndex = 1; return; }
        image = null; avpImage = null; loadedPath = null; known335InBoot = null;
        RestoreAddress();
        Progress.Value = 0; PercentLabel.Text = "0%"; SetStatus("请选择对应芯片的升级文件");
        ((ComboBoxItem)TransportMode.Items[1]).IsEnabled = !Is335;
        if (Is335) TransportMode.SelectedIndex = 0;
        ImageLabel.Text = "尚未选择升级文件"; ImageSummaryLabel.Text = ""; ImageLabel.ToolTip = null;
        UpdateMode(); Write($"已选择 {ChipName}，请重新载入对应升级文件。");
    }
    private void UpdateMode()
    {
        BaudRate.ItemsSource = IsCan ? new[] { 10000, 20000, 50000, 100000, 125000, 250000, 500000, 800000, 1000000 } : new[] { 9600, 19200, 38400, 57600, 115200, 230400, 460800 };
        BaudRate.SelectedItem = IsCan ? 500000 : 115200;
        BaudLabel.Text = IsCan ? "CAN 波特率" : "串口波特率";
        DeviceLabel.Text = IsCan ? "CAN 盒" : "串口";
        ComPort.Visibility = IsCan ? Visibility.Collapsed : Visibility.Visible;
        CanModel.Visibility = IsCan ? Visibility.Visible : Visibility.Collapsed;
        CanOptions.IsEnabled = CanChannelPanel.IsEnabled = IsCan;
        CanOptions.Opacity = CanChannelPanel.Opacity = IsCan ? 1 : 0.45;
        ModeNotice.Text = "请选择配套应用文件；通信速率须与设备一致。开始升级会擦写应用分区。";
        ModeNotice.ToolTip = "APP 起始地址须与设备分区、固件入口一致。";
        if (Is335)
        {
            BaudRate.ItemsSource = new[] { 115200 }; BaudRate.SelectedItem = 115200;
            ModeNotice.Text = "335 仅支持 SCI 115200 / V201；协议不传输或查询起始地址，须自行匹配 BOOT 的写入基址与 APP。";
            ModeNotice.ToolTip = "SCI-A：GPIO28/RX、GPIO29/TX，115200 / 8N1。首次通过 JTAG 烧录对应晶振的 V201 BOOT；升级前自动核对版本。";
            if (Customer335Edition)
                ModeNotice.Text = "AVP32F335 / SCI / 115200 · 点击升级后在 9 秒内给板子上电或复位；升级完成后自动请求启动 APP。";
        }
        RefreshPartition();
        UpdateDeviceHint(); UpdateResetButton(); UpdateControls();
    }
    private void UpdateResetButton()
    {
        ResetButton.Content = !Is335 || known335InBoot == false ? "重启设备" : known335InBoot == true ? "启动应用程序" : "启动 / 重启程序";
        ResetButton.ToolTip = Is335 ? "设备在 BOOT 时请求启动有效应用；应用运行时请求重启。按钮根据最近一次读取的状态显示，应答不代表应用已确认运行。" : "发送设备复位请求；应答不代表应用已确认运行。";
    }
    private void Device_Changed(object sender, SelectionChangedEventArgs e)
    {
        if (!ready) return;
        known335InBoot = null; UpdateResetButton(); UpdateDeviceHint();
    }
    private void CanDevice_Changed(object sender, SelectionChangedEventArgs e)
    {
        if (!ready) return;
        if (CanModel.SelectedItem is DetectedCanDevice selected && selected.DriverPath.Length > 0)
            DllPath.Text = selected.DriverPath;
        int previous = CanChannel.SelectedIndex;
        byte count = (CanModel.SelectedItem as DetectedCanDevice)?.Channels ?? 0;
        CanChannel.ItemsSource = Enumerable.Range(0, count).ToArray();
        CanChannel.SelectedIndex = count == 0 ? -1 : previous >= 0 && previous < count ? previous : 0;
        UpdateDeviceHint();
    }
    private void UpdateDeviceHint()
    {
        if (discovering) return;
        int count = IsCan ? CanModel.Items.Count : ComPort.Items.Count;
        DeviceHint.Text = count == 0 ? "未检测到设备，请连接后刷新" : $"检测到 {count} 个设备";
        ComPort.ToolTip = ComPort.SelectedItem;
        CanModel.ToolTip = CanModel.SelectedItem?.ToString();
    }
    private async void Refresh_Click(object sender, RoutedEventArgs e) => await RefreshDevicesAsync();
    private async Task RefreshDevicesAsync()
    {
        if (busy || discovering || simulation) return;
        discovering = true; UpdateControls();
        DeviceHint.Text = "正在查找设备…"; SetStatus(IsCan ? "正在查找 CAN 盒…" : "正在查找串口…");
        using var cancellation = new CancellationTokenSource(); discoveryCancellation = cancellation;
        try
        {
            if (IsCan)
            {
                var previous = CanModel.SelectedItem as DetectedCanDevice;
                var devices = await DeviceDiscovery.AllCanDevicesAsync(manualCanDriverPath, cancellation.Token, Write);
                CanModel.ItemsSource = devices;
                CanModel.SelectedItem = devices.FirstOrDefault(device => previous is not null && device.Type == previous.Type && device.DriverPath == previous.DriverPath &&
                    (previous.Serial.Length > 0 ? device.Serial == previous.Serial : device.Index == previous.Index)) ?? devices.FirstOrDefault();
            }
            else
            {
                string? previous = ComPort.SelectedItem as string;
                var ports = await Task.Run(DeviceDiscovery.SerialPorts, cancellation.Token);
                cancellation.Token.ThrowIfCancellationRequested();
                ComPort.ItemsSource = ports;
                ComPort.SelectedItem = ports.Contains(previous) ? previous : ports.FirstOrDefault();
            }
            int count = IsCan ? CanModel.Items.Count : ComPort.Items.Count;
            SetStatus(count > 0 ? "设备列表已更新，请选择设备和升级文件" : "未检测到设备，请连接后点击刷新设备", count > 0 ? "normal" : "notice");
            Write($"{(IsCan ? "CAN 盒" : "串口")}检索完成，共 {count} 个。" + (count == 0 && IsCan ? "请检查驱动、连接及其他软件是否占用盒子。" : ""));
        }
        catch (OperationCanceledException) { }
        catch (Exception ex)
        {
            if (IsCan) { CanModel.ItemsSource = Array.Empty<DetectedCanDevice>(); CanChannel.ItemsSource = Array.Empty<int>(); }
            else ComPort.ItemsSource = Array.Empty<string>();
            SetStatus("设备检索失败，请查看日志后刷新", "error"); Write(Explain(ex));
        }
        finally { discoveryCancellation = null; discovering = false; UpdateDeviceHint(); UpdateControls(); }
    }
    internal void EnableSimulation(bool dropAck = true) { simulation = true; dropOneAck = dropAck; }
    internal Func<ITransport>? SimulationTransportFactory { get; set; }
    internal Func<IAvp335Transport>? Simulation335TransportFactory { get; set; }
    internal void LoadDemoImage()
    {
        loadedPath = null;
        if (Is335) avpImage = Avp335FirmwareImage.Demo(SelectedAppStart); else image = FirmwareImage.Demo(SelectedProfile);
        ShowImage("后台仿真数据");
    }
    private void ShowImage(string name)
    {
        string details = Is335
            ? $"{avpImage!.Bytes.Length:N0} 字节 · MD5 {Convert.ToHexString(avpImage.Digest)} · 入口 0x{avpImage.StartWordAddress:X6}"
            : $"{image!.Words.Length * 2:N0} 字节 · CRC32 {Crc32.Compute(image.Bytes):X8} · 入口 0x{image.EntryWordAddress:X6}";
        ImageLabel.Text = name;
        ImageSummaryLabel.Text = $"{(Is335 ? avpImage!.Bytes.Length : image!.Words.Length * 2):N0} 字节 · 已加载";
        ImageLabel.ToolTip = $"{loadedPath ?? name}\n{details}";
        Write($"已载入 {loadedPath ?? name}；{details}。");
        if (Is335) Write("空洞填充 0xFFFF，有效标志由 BOOT 提交。");
    }
    private async void Load_Click(object sender, RoutedEventArgs e)
    {
        var dialog = new OpenFileDialog
        {
            Filter = Is335 ? "AVP32F335 应用文件 (*.out)|*.out" : "升级文件 (*.out;*.txt;*.hex;*.json)|*.out;*.txt;*.hex;*.json|CCS 应用 (*.out)|*.out|TI SCI8 文件 (*.txt;*.hex)|*.txt;*.hex|JSON 固件包 (*.json)|*.json"
        };
        if (dialog.ShowDialog() != true) return;
        var target = SelectedProfile;
        uint startAddress = SelectedAppStart;

        image = null; avpImage = null; loadedPath = dialog.FileName; ImageLabel.ToolTip = null; ImageSummaryLabel.Text = "";
        using var cancellation = new CancellationTokenSource();
        running = cancellation;
        SetBusy(true, "load");
        ImageLabel.Text = "正在载入并校验固件…";
        try
        {
            if (Is335)
            {
                avpImage = await Task.Run(() => Avp335OutLoader.Load(dialog.FileName, startAddress), cancellation.Token);
                Write("AVP32F335 OUT 装载段、codestart、有效标志和分区边界检查通过。");
            }
            else if (Path.GetExtension(dialog.FileName).Equals(".out", StringComparison.OrdinalIgnoreCase))
            {
                SetStatus("正在查找 TI 转换工具…");
                string? converter = await Task.Run(TiToolLocator.Find, cancellation.Token);
                cancellation.Token.ThrowIfCancellationRequested();
                if (converter is null)
                {
                    Write("未自动找到 hex2000.exe，请选择 CCS 的 tools/compiler/ti-cgt-c2000_*/bin/hex2000.exe；此路径会保存供下次使用。");
                    var picker = new OpenFileDialog { Title = "选择 TI C2000 转换工具 hex2000.exe", Filter = "TI hex2000|hex2000.exe" };
                    if (picker.ShowDialog() != true)
                    {
                        ImageLabel.Text = "尚未选择转换工具，未载入固件。";
                        SetStatus("文件载入已取消", "notice");
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
                SetStatus("正在转换并校验 .out…");
                image = await Task.Run(() => TiOutConverter.LoadAsync(dialog.FileName, converter, cancellation.Token, target));
                Write(".out 转换完成；入口、分区边界和镜像检查通过。");
            }
            else
            {
                image = await Task.Run(() => FirmwareImage.Load(dialog.FileName, target), cancellation.Token);
            }
            cancellation.Token.ThrowIfCancellationRequested();
            ShowImage(Path.GetFileName(dialog.FileName));
            SetStatus("文件已加载，可开始升级", "success");
        }
        catch (OperationCanceledException)
        {
            image = null; avpImage = null;
            ImageLabel.Text = "固件载入已取消";
            SetStatus("文件载入已取消", "notice");
        }
        catch (Exception ex)
        {
            image = null; avpImage = null;
            ImageLabel.Text = "固件载入失败";
            SetStatus("文件载入失败，请查看日志", "error");
            Write(ex.Message);
        }
        finally
        {
            running = null;
            SetBusy(false);
        }
    }
    private async void Dll_Click(object sender, RoutedEventArgs e)
    {
        var dialog = new OpenFileDialog { Filter = "CAN 驱动库|ControlCAN.dll;usbcan.dll|动态链接库|*.dll" };
        if (dialog.ShowDialog() == true) { manualCanDriverPath = dialog.FileName; DllPath.Text = dialog.FileName; await RefreshDevicesAsync(); }
    }
    private async Task<ITransport> OpenTransport()
    {
        var watch = System.Diagnostics.Stopwatch.StartNew();
        communicationTrace?.Record("transport_open_start", new { transport = IsCan ? "CAN" : "SCI" });
        try
        {
            var transport = await OpenTransportCore();
            communicationTrace?.Record("transport_open_end", new { elapsed_ms = watch.Elapsed.TotalMilliseconds });
            return transport;
        }
        catch (Exception ex)
        {
            communicationTrace?.Record("transport_open_error", new { elapsed_ms = watch.Elapsed.TotalMilliseconds, errorType = ex.GetType().Name, error = ex.Message });
            throw;
        }
    }
    private async Task<ITransport> OpenTransportCore()
    {
        int baud = (int)BaudRate.SelectedItem;
        if (simulation) return TraceTransport(SimulationTransportFactory?.Invoke() ?? new SimulatedTransport(IsCan, dropOneAck, 15, SelectedProfile));
        if (!IsCan)
        {
            string port = ComPort.SelectedItem as string ?? throw new InvalidOperationException("请连接串口并点击刷新设备，再选择串口。");
            return TraceTransport(await Task.Run<ITransport>(() => new SerialTransport(port, baud)));
        }
        var device = CanModel.SelectedItem as DetectedCanDevice ?? throw new InvalidOperationException("请连接 CAN 盒并点击刷新设备，再选择盒子。");
        if (CanChannel.SelectedItem is not int selectedChannel) throw new InvalidOperationException("请选择 CAN 通道。");
        string dll = device.DriverPath.Length > 0 ? device.DriverPath : DllPath.Text;
        uint node = uint.Parse(NodeId.Text);
        return TraceTransport(await Task.Run<ITransport>(() => new ControlCanTransport(dll, device.Type, device.Index, (uint)selectedChannel, baud, node, communicationTrace)));
    }
    private async Task<IAvp335Transport> Open335Transport()
    {
        if (IsCan) throw new InvalidOperationException("AVP32F335 仅支持串口（SCI）。");
        if (simulation) return Simulation335TransportFactory?.Invoke() ?? new Avp335SimulatedTransport(dropOneAck);
        string port = ComPort.SelectedItem as string ?? throw new InvalidOperationException("请连接串口并点击刷新设备，再选择串口。");
        return await Task.Run<IAvp335Transport>(() => new Avp335SerialTransport(port));
    }
    private async Task FinishUpgrade(Func<Task> reset)
    {
        committed = true; Progress.Value = 100; PercentLabel.Text = "100%";
        if (Is335) { known335InBoot = true; UpdateResetButton(); }
        SetStatus("升级完成，正在请求复位启动…");
        try { await reset(); SetStatus("升级完成，启动请求已确认", "success"); Write("设备已确认复位或启动请求；应用运行状态未做额外读取。"); }
        catch (Exception ex) { SetStatus("升级完成，启动未确认", "notice"); Write($"固件已提交，复位或启动应答未确认：{ex.Message} 可手动复位。"); }
        if (Is335) { known335InBoot = null; UpdateResetButton(); }
    }
    private async Task Update335(IProgress<UpdateProgress> progress, CancellationToken cancellation)
    {
        await using var transport = await Open335Transport();
        var client = new Avp335UpdaterClient(transport);
        if (Customer335Edition) Write("串口已打开，开始等待 BOOT：总等待 9 秒，约每 50ms 查询版本。");
        await client.UpdateAsync(avpImage!, progress, cancellation, waitForPowerOn: Customer335Edition);
        Write("335 整包 MD5 与 Flash 回读通过，有效标志已提交。");
        if (transport is Avp335SimulatedTransport sim) Write($"仿真计数：擦除 {sim.EraseCount}，写入 {sim.ProgramCount}，提交 {sim.CommitCount}。");
        await FinishUpgrade(() => client.ResetAsync(cancellation));
    }
    private async void Start_Click(object sender, RoutedEventArgs e)
    {
        if (Is335 ? avpImage is null : image is null) { SetStatus("请先选择升级文件", "notice"); Write("请先选择升级文件。"); return; }
        if ((Is335 ? avpImage!.SimulationOnly : image!.SimulationOnly) && !simulation) { Write("演示数据不是可执行固件，只能用于仿真。"); return; }
        running = new CancellationTokenSource(); committed = false; SetBusy(true, "upgrade"); Progress.Value = 0; PercentLabel.Text = "0%"; SetStatus("正在打开通信设备…");
        BeginCommunicationTrace("upgrade");
        try
        {
            var progress = new Progress<UpdateProgress>(p =>
            {
                if (!busy || committed) return;
                Progress.Value = p.Percent; PercentLabel.Text = $"{p.Percent}%"; Progress.ToolTip = p.Message;
                SetStatus(p.Message.StartsWith("等待板子", StringComparison.Ordinal) ? p.Message : p.Percent == 100 ? "文件校验通过，正在完成升级…" : p.Message.Contains("正在擦除") ? "正在擦除应用程序…" : p.Message.Contains("已写入") ? "正在写入升级文件…" : "正在核对设备，准备升级…");
            });
            if (Is335) { await Update335(progress, running.Token); return; }
            await using var transport = await OpenTransport();
            Write("通信通道已打开，开始读取设备与升级。");
            var client = new UpdaterClient(transport, communicationTrace);
            SetStatus("正在自动进入升级模式…");
            Write("正在自动进入升级模式…");
            await client.EnterBootAsync(running.Token);
            Write("设备已进入升级模式，开始下载升级文件。");
            await client.UpdateAsync(image!, progress, running.Token);
            Write("应用 CRC32 校验通过，有效标记已提交。");
            if (transport is SimulatedTransport sim) Write($"仿真计数：擦除 {sim.Device.EraseCount}，写入 {sim.Device.ProgramCount}，提交 {sim.Device.CommitCount}。");
            await FinishUpgrade(() => client.ResetAsync(running.Token));
        }
        catch (OperationCanceledException) { SetStatus("升级已停止，请等待设备当前操作完成后重新升级", "notice"); Write(Is335 ? "已取消 335 升级；等待当前擦写操作完成，保持在 BOOT，重新开始升级。" : "已取消传输；若设备仍在擦除，需等待其完成。未提交镜像不会启动，重连后从头升级。"); }
        catch (Exception ex) { SetStatus("升级失败，请查看日志", "error"); Write(Explain(ex)); }
        finally { await EndCommunicationTrace(); running.Dispose(); running = null; SetBusy(false); }
    }
    private async void Probe_Click(object sender, RoutedEventArgs e)
    {
        SetBusy(true); SetStatus("正在读取设备信息…");
        BeginCommunicationTrace("probe");
        try
        {
            if (Is335)
            {
                await using var sci = await Open335Transport();
                var info = await new Avp335UpdaterClient(sci).ProbeAsync(CancellationToken.None);
                known335InBoot = info.InBoot; UpdateResetButton();
                SetStatus($"通讯正常，设备信息已读取：{(info.InBoot ? "升级模式" : "应用程序运行模式")}", "success");
                Write("与设备程序通讯正常，设备信息读取成功。");
                Write($"AVP32F335 状态 {(info.InBoot ? "BOOT" : "APP")}，BOOT 版本 {info.BootVersion}，APP 版本 {info.AppVersion}；升级需要 BOOT V201。"); return;
            }
            await using var transport = await OpenTransport();
            var request = new Packet((byte)Command.Info, 0, 0, []);
            var reply = await transport.ExchangeAsync(request, TimeSpan.FromSeconds(3), CancellationToken.None);
            if (!ReplyMatch.Is(reply, request) || reply.Payload.Length != 24 || reply.Payload[0] != 0) throw new InvalidDataException("设备信息应答格式错误。");
            SetStatus("通讯正常，设备信息已读取，请查看日志", "success");
            Write("与设备程序通讯正常，设备信息读取成功。");
            Write($"设备 0x{BinaryPrimitives.ReadUInt32LittleEndian(reply.Payload.AsSpan(6)):X8}，状态 {(BootState)reply.Payload[1]}，应用起点 0x{BinaryPrimitives.ReadUInt32LittleEndian(reply.Payload.AsSpan(16)):X6}。");
        }
        catch (Exception ex) { SetStatus("操作失败，请查看日志", "error"); Write(Explain(ex)); }
        finally { await EndCommunicationTrace(); SetBusy(false); }
    }
    private async void Reset_Click(object sender, RoutedEventArgs e)
    {
        SetBusy(true); SetStatus("正在请求复位或启动…");
        BeginCommunicationTrace("reset");
        try { if (Is335) { await using var sci = await Open335Transport(); await new Avp335UpdaterClient(sci).ResetAsync(CancellationToken.None); } else { await using var transport = await OpenTransport(); await new UpdaterClient(transport, communicationTrace).ResetAsync(CancellationToken.None); } known335InBoot = null; UpdateResetButton(); SetStatus("复位或启动请求已确认", "success"); Write("设备已确认复位或启动请求；应用运行状态未做额外读取。"); }
        catch (Exception ex) { SetStatus("操作失败，请查看日志", "error"); Write(Explain(ex)); }
        finally { await EndCommunicationTrace(); SetBusy(false); }
    }
    private void SaveLog_Click(object sender, RoutedEventArgs e)
    {
        var dialog = new SaveFileDialog { Filter = "文本日志|*.txt", FileName = $"upgrade-{DateTime.Now:yyyyMMdd-HHmmss}.txt" };
        if (dialog.ShowDialog() == true) { try { File.WriteAllText(dialog.FileName, Log.Text); } catch (Exception ex) { Write(ex.Message); } }
    }
    private static string Explain(Exception ex) => ex is BadImageFormatException ? "ControlCAN.dll 与程序位数不匹配，请使用对应的官方 x64 / x86 库。" : ex.Message;
    private void Cancel_Click(object sender, RoutedEventArgs e) => running?.Cancel();
    private void ClearLog_Click(object sender, RoutedEventArgs e) => Log.Clear();
    internal void ClearLogForCheck() => ClearLog_Click(this, new RoutedEventArgs());
    private void SetBusy(bool value, string task = "")
    {
        busy = value; operation = value ? task : "";
        CancelButton.Content = task == "load" ? "取消载入" : "停止升级";
        CancelButton.ToolTip = task == "load" ? "取消本次文件载入，不会擦写设备。" : "请求停止传输；设备正在执行的 Flash 操作可能仍需完成。请等待后重新升级。";
        UpdateControls();
    }
    private void UpdateControls()
    {
        bool available = !busy && !discovering;
        StartButton.IsEnabled = LoadButton.IsEnabled = SettingsPanel.IsEnabled = ProbeButton.IsEnabled = ResetButton.IsEnabled = available;
        StartButton.IsEnabled = LoadButton.IsEnabled = available && AddressValid;
        RefreshButton.IsEnabled = available;
        CancelButton.IsEnabled = busy && running is not null && (operation is "load" or "upgrade");
    }
    protected override void OnClosed(EventArgs e) { running?.Cancel(); discoveryCancellation?.Cancel(); base.OnClosed(e); }
}
