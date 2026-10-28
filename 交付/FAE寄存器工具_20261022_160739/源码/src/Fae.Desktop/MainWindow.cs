using System.Globalization;
using System.IO;
using System.Text.Json;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Threading;
using Fae.Core;

namespace Fae.Desktop;

public partial class MainWindow : Window
{
    private readonly Grid _body = new();
    private readonly TextBlock _status = new() { Text = "就绪 · 离线资料 · 参数工具", Foreground = Brush("#64748B"), Margin = new Thickness(24, 8, 24, 12), TextWrapping = TextWrapping.Wrap };
    private readonly List<Button> _navigation = [];
    private readonly Dictionary<int, UIElement> _pages = [];
    private readonly Dictionary<string, string> _settings;
    private readonly string _settingsFile = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "FaeRegisterTool", "settings.json");
    private readonly List<Action> _smokeChecks = [];
    private int _currentPage;
    private bool _smoke;
    private static readonly string[] Titles = ["寄存器查询", "CAN 波特率", "PWM 频率", "ADC 换算", "地址计算", "程序员计算器"];
    private static readonly string[] Hints = ["按芯片、外设或关键词查找中文字段解释", "选择时序方案，复制当前型号的波特率配置", "计算计数周期、实际频率与误差", "原始码、电压与电流双向换算", "明确地址单位，核对存储空间", "定宽整数、进制、位运算与位视图"];

    public MainWindow()
    {
        Title = "FAE 寄存器与嵌入式计算工具"; Width = Math.Min(1360, SystemParameters.WorkArea.Width - 24); Height = Math.Min(900, SystemParameters.WorkArea.Height - 24); MinWidth = 1000; MinHeight = 640;
        WindowStartupLocation = WindowStartupLocation.CenterScreen;
        try { _settings = JsonSerializer.Deserialize<Dictionary<string, string>>(File.ReadAllText(_settingsFile)) ?? []; } catch { _settings = []; }
        var root = new Grid(); root.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(224) }); root.ColumnDefinitions.Add(new ColumnDefinition());
        var sidebar = new DockPanel { Background = Brush("#14243E") };
        var brand = new StackPanel { Margin = new Thickness(23, 30, 18, 28) };
        brand.Children.Add(new TextBlock { Text = "FAE TOOLKIT", Foreground = Brush("#93B7FF"), FontSize = 12, FontWeight = FontWeights.SemiBold });
        brand.Children.Add(new TextBlock { Text = "寄存器 · 计算工具", Foreground = Brushes.White, FontSize = 21, FontWeight = FontWeights.SemiBold, Margin = new Thickness(0, 10, 0, 0) });
        DockPanel.SetDock(brand, Dock.Top); sidebar.Children.Add(brand);
        var foot = new TextBlock { Text = "离线运行  /  四系列芯片\nTI 寄存器基线 · 中文说明", Foreground = Brush("#91A4BF"), FontSize = 12, Margin = new Thickness(23, 20, 15, 25), LineHeight = 22 };
        DockPanel.SetDock(foot, Dock.Bottom); sidebar.Children.Add(foot);
        var nav = new StackPanel { Margin = new Thickness(12, 0, 12, 0) };
        for (int i = 0; i < Titles.Length; i++)
        {
            int index = i;
            var button = new Button { Content = $"{i + 1:00}   {Titles[i]}", HorizontalContentAlignment = HorizontalAlignment.Left, Foreground = Brush("#CBD8EA"), Background = Brushes.Transparent, FontSize = 15, Padding = new Thickness(16, 14, 8, 14), Margin = new Thickness(0, 3, 0, 3) };
            button.Click += (_, _) => ShowPage(index); _navigation.Add(button); nav.Children.Add(button);
        }
        sidebar.Children.Add(nav); root.Children.Add(sidebar);
        var content = new DockPanel(); Grid.SetColumn(content, 1); DockPanel.SetDock(_status, Dock.Bottom); content.Children.Add(_status); content.Children.Add(_body); root.Children.Add(content); Content = root;
        Closing += (_, _) => { if (!_smoke) SaveSettings(); };
        ShowPage(0);
    }

    private static SolidColorBrush Brush(string color) => new((Color)ColorConverter.ConvertFromString(color));
    private void SaveSettings()
    {
        try { Directory.CreateDirectory(Path.GetDirectoryName(_settingsFile)!); File.WriteAllText(_settingsFile, JsonSerializer.Serialize(_settings, new JsonSerializerOptions { WriteIndented = true })); }
        catch (Exception ex) { _status.Text = "设置未保存：" + ex.Message; }
    }
    private void ShowPage(int index)
    {
        _currentPage = index;
        if (!_pages.TryGetValue(index, out var page))
        {
            page = index switch { 0 => RegisterPage(), 1 => CanPage(), 2 => PwmPage(), 3 => AdcPage(), 4 => AddressPage(), _ => ProgrammerPage() }; _pages[index] = page;
        }
        _body.Children.Clear(); _body.Children.Add(page);
        for (int i = 0; i < _navigation.Count; i++) { _navigation[i].Background = i == index ? Brush("#294B80") : Brushes.Transparent; _navigation[i].Foreground = i == index ? Brushes.White : Brush("#CBD8EA"); }
        _status.Text = "就绪 · " + Titles[index] + " · 本工具不连接或写入芯片";
    }
    private StackPanel Page(int index)
    {
        var panel = new StackPanel { Margin = new Thickness(26, 24, 26, 10) };
        panel.Children.Add(new TextBlock { Text = Titles[index], FontSize = 28, FontWeight = FontWeights.SemiBold });
        panel.Children.Add(new TextBlock { Text = Hints[index], FontSize = 14, Foreground = Brush("#64748B"), Margin = new Thickness(0, 7, 0, 20) }); return panel;
    }
    private static ScrollViewer Scroll(UIElement content) => new() { Content = content, VerticalScrollBarVisibility = ScrollBarVisibility.Auto, HorizontalScrollBarVisibility = ScrollBarVisibility.Disabled };
    private static StackPanel Card(Panel parent, string? title = null)
    {
        var panel = new StackPanel();
        if (title != null) panel.Children.Add(new TextBlock { Text = title, FontSize = 16, FontWeight = FontWeights.SemiBold, Margin = new Thickness(0, 0, 0, 12) });
        parent.Children.Add(new Border { Background = Brushes.White, BorderBrush = Brush("#E0E7F1"), BorderThickness = new Thickness(1), CornerRadius = new CornerRadius(10), Padding = new Thickness(20, 17, 20, 17), Margin = new Thickness(0, 0, 0, 16), Child = panel }); return panel;
    }
    private static WrapPanel Row(Panel panel) { var row = new WrapPanel(); panel.Children.Add(row); return row; }
    private static TextBlock Note(Panel panel, string text, string color = "#64748B")
    { var t = new TextBlock { Text = text, TextWrapping = TextWrapping.Wrap, Foreground = Brush(color), Margin = new Thickness(0, 4, 0, 9), LineHeight = 23 }; panel.Children.Add(t); return t; }
    private static TextBox Input(Panel panel, string label, string value, double width = 158)
    { var wrap = new StackPanel { Width = width }; wrap.Children.Add(new TextBlock { Text = label, Foreground = Brush("#52617A"), FontSize = 12 }); var box = new TextBox { Text = value }; wrap.Children.Add(box); panel.Children.Add(wrap); return box; }
    private static ComboBox Choice(Panel panel, string label, IEnumerable<string> values, int selected = 0, double width = 175)
    { var wrap = new StackPanel { Width = width }; wrap.Children.Add(new TextBlock { Text = label, Foreground = Brush("#52617A"), FontSize = 12 }); var combo = new ComboBox { ItemsSource = values.ToArray(), SelectedIndex = selected }; wrap.Children.Add(combo); panel.Children.Add(wrap); return combo; }
    private ComboBox ChipChoice(Panel panel, string module, Action<Chip>? changed = null)
    {
        int selected = Array.FindIndex(Chips.All, c => c.Id == (_settings.GetValueOrDefault(module) ?? "335"));
        var combo = Choice(panel, "芯片型号", Chips.All.Select(c => c.Name), Math.Max(0, selected), 210);
        combo.SelectionChanged += (_, _) => { var chip = Chips.All[Math.Max(0, combo.SelectedIndex)]; if (!_smoke) _settings[module] = chip.Id; changed?.Invoke(chip); };
        return combo;
    }
    private static Chip SelectedChip(ComboBox combo) => Chips.All[Math.Max(0, combo.SelectedIndex)];
    private static Button ActionButton(Panel panel, string text, Action action, bool primary = false)
    { var b = new Button { Content = text }; if (primary) { b.Background = Brush("#2563EB"); b.Foreground = Brushes.White; } b.Click += (_, _) => action(); panel.Children.Add(b); return b; }
    private void Guard(Action action)
    { try { action(); _status.Foreground = Brush("#64748B"); } catch (Exception ex) { _status.Text = "请检查输入：" + ex.Message; _status.Foreground = Brush("#B42318"); if (_smoke) throw; } }
    private static double Number(TextBox input) => double.TryParse(input.Text.Trim(), NumberStyles.Float, CultureInfo.InvariantCulture, out var value) && double.IsFinite(value) ? value : throw new ArgumentException("请输入有效数字（小数点用 .）：" + input.Text);
    private static int Integer(TextBox input) => int.TryParse(input.Text.Trim(), out var value) ? value : throw new ArgumentException("请输入整数：" + input.Text);
    private void Copy(string text) => Guard(() => { if (string.IsNullOrWhiteSpace(text)) throw new ArgumentException("暂无可复制内容，请先计算。"); Clipboard.SetText(text); _status.Text = "已复制到剪贴板"; });
    private TextBox Code(Panel parent)
    {
        var code = new TextBox { IsReadOnly = true, FontFamily = new FontFamily("Cascadia Mono, Consolas"), FontSize = 13, AcceptsReturn = true, TextWrapping = TextWrapping.NoWrap, HorizontalScrollBarVisibility = ScrollBarVisibility.Auto, VerticalScrollBarVisibility = ScrollBarVisibility.Auto, Height = 250, Margin = new Thickness(0), Background = Brush("#F8FAFD") };
        parent.Children.Add(code); ActionButton(parent, "复制代码", () => Copy(code.Text)); return code;
    }
    private static void Column(DataGrid grid, string label, string binding, double width = 100) => grid.Columns.Add(new DataGridTextColumn { Header = label, Binding = new System.Windows.Data.Binding(binding), Width = width });
    public async Task SmokeTest(string folder, bool testClipboard = false)
    {
        _smoke = true; Directory.CreateDirectory(folder);
        for (int i = 0; i < Titles.Length; i++)
        {
            ShowPage(i); UpdateLayout(); await Dispatcher.InvokeAsync(() => { }, DispatcherPriority.ApplicationIdle);
            var checks = _smokeChecks.ToArray(); foreach (var check in checks) check();
            if (_pages[i] is ScrollViewer pageScroll) pageScroll.ScrollToTop();
            UpdateLayout(); await Dispatcher.InvokeAsync(() => { }, DispatcherPriority.ApplicationIdle);
            var bitmap = new RenderTargetBitmap((int)ActualWidth, (int)ActualHeight, 96, 96, PixelFormats.Pbgra32); bitmap.Render(this);
            var encoder = new PngBitmapEncoder(); encoder.Frames.Add(BitmapFrame.Create(bitmap)); using var output = File.Create(Path.Combine(folder, $"{i + 1:00}-{Titles[i]}.png")); encoder.Save(output);
        }
        string clipboardResult = "剪贴板未测试。";
        if (testClipboard)
        {
            var original = Clipboard.GetDataObject();
            var backup = new DataObject();
            if (original != null)
                foreach (var format in original.GetFormats(false))
                {
                    var value = original.GetData(format, false);
                    if (value != null) backup.SetData(format, value, false);
                }
            try
            {
                const string probe = "FAE 配置代码复制测试 0xA53C";
                Copy(probe);
                if (Clipboard.GetText() != probe) throw new InvalidOperationException("剪贴板复制自检失败。");
                clipboardResult = "剪贴板写入/读取通过，已恢复原内容。";
            }
            finally { if (original == null) Clipboard.Clear(); else Clipboard.SetDataObject(backup, true); }
        }
        File.WriteAllText(Path.Combine(folder, "smoke-result.txt"), $"PASS {DateTime.Now:O}\n六个页面已载入、完成默认计算并渲染截图。寄存器条目 {_registers.Count}。\n{clipboardResult}\n");
    }
}
