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
    private readonly TextBlock _status = new() { Text = "就绪", Foreground = Brush("#64748B"), Margin = new Thickness(12, 2, 12, 5), TextTrimming = TextTrimming.CharacterEllipsis, FontSize = 11 };
    private readonly List<Button> _navigation = [];
    private readonly Dictionary<int, UIElement> _pages = [];
    private readonly Dictionary<string, string> _settings;
    private readonly string _settingsFile = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "FaeRegisterTool", "settings.json");
    private readonly List<Action> _smokeChecks = [];
    private double _renderScale = 1;
    private int _currentPage;
    private bool _smoke;
    private static readonly string[] Titles = ["寄存器查询", "CAN 波特率", "PWM 频率", "ADC 换算", "地址计算", "程序员计算器"];

    public MainWindow()
    {
        Title = "FAE 寄存器与嵌入式计算工具"; Width = Math.Min(960, SystemParameters.WorkArea.Width - 16); Height = Math.Min(640, SystemParameters.WorkArea.Height - 16); MinWidth = 800; MinHeight = 520;
        WindowStartupLocation = WindowStartupLocation.CenterScreen;
        try { _settings = JsonSerializer.Deserialize<Dictionary<string, string>>(File.ReadAllText(_settingsFile)) ?? []; } catch { _settings = []; }
        var root = new Grid(); root.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(140) }); root.ColumnDefinitions.Add(new ColumnDefinition());
        var sidebar = new DockPanel { Background = Brush("#14243E") };
        var brand = new StackPanel { Margin = new Thickness(14, 18, 8, 18) };
        brand.Children.Add(new TextBlock { Text = "FAE TOOLKIT", Foreground = Brush("#93B7FF"), FontSize = 11, FontWeight = FontWeights.SemiBold });
        brand.Children.Add(new TextBlock { Text = "寄存器 · 计算", Foreground = Brushes.White, FontSize = 15, FontWeight = FontWeights.SemiBold, Margin = new Thickness(0, 6, 0, 0) });
        DockPanel.SetDock(brand, Dock.Top); sidebar.Children.Add(brand);
        var nav = new StackPanel { Margin = new Thickness(7, 0, 7, 0) };
        for (int i = 0; i < Titles.Length; i++)
        {
            int index = i;
            var button = new Button { Content = Titles[i], HorizontalContentAlignment = HorizontalAlignment.Left, Foreground = Brush("#CBD8EA"), Background = Brushes.Transparent, FontSize = 12, Padding = new Thickness(12, 10, 4, 10), Margin = new Thickness(0, 2, 0, 2) };
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
        _status.Visibility = index == 0 ? Visibility.Collapsed : Visibility.Visible;
    }
    private Grid CompactPage(int index)
    {
        var panel = new Grid { Margin = new Thickness(12, 10, 12, 5) };
        panel.RowDefinitions.Add(new RowDefinition { Height = GridLength.Auto });
        panel.RowDefinitions.Add(new RowDefinition());
        panel.Children.Add(new TextBlock { Text = Titles[index], FontSize = 18, FontWeight = FontWeights.SemiBold, Margin = new Thickness(0, 0, 0, 8) });
        return panel;
    }
    private StackPanel Page(int index)
    {
        var panel = new StackPanel { Margin = new Thickness(12, 10, 12, 5) };
        panel.Children.Add(new TextBlock { Text = Titles[index], FontSize = 18, FontWeight = FontWeights.SemiBold }); return panel;
    }
    private static ScrollViewer Scroll(UIElement content) => new() { Content = content, VerticalScrollBarVisibility = ScrollBarVisibility.Auto, HorizontalScrollBarVisibility = ScrollBarVisibility.Disabled };
    private static StackPanel Card(Panel parent, string? title = null)
    {
        var panel = new StackPanel();
        if (title != null) panel.Children.Add(new TextBlock { Text = title, FontSize = 13, FontWeight = FontWeights.SemiBold, Margin = new Thickness(0, 0, 0, 6) });
        parent.Children.Add(new Border { Background = Brushes.White, BorderBrush = Brush("#E0E7F1"), BorderThickness = new Thickness(1), CornerRadius = new CornerRadius(5), Padding = new Thickness(10), Margin = new Thickness(0, 0, 0, 8), Child = panel }); return panel;
    }
    private static WrapPanel Row(Panel panel) { var row = new WrapPanel(); panel.Children.Add(row); return row; }
    private static TextBlock Note(Panel panel, string text, string color = "#64748B")
    { var t = new TextBlock { Text = text, TextWrapping = TextWrapping.Wrap, Foreground = Brush(color), Margin = new Thickness(0, 3, 0, 5), LineHeight = 17, FontSize = 11 }; panel.Children.Add(t); return t; }
    private static TextBox Input(Panel panel, string label, string value, double width = 158)
    { var wrap = new StackPanel { Width = width }; wrap.Children.Add(new TextBlock { Text = label, Foreground = Brush("#52617A"), FontSize = 11, TextTrimming = TextTrimming.CharacterEllipsis }); var box = new TextBox { Text = value }; wrap.Children.Add(box); panel.Children.Add(wrap); return box; }
    private static ComboBox Choice(Panel panel, string label, IEnumerable<string> values, int selected = 0, double width = 175)
    { var wrap = new StackPanel { Width = width }; wrap.Children.Add(new TextBlock { Text = label, Foreground = Brush("#52617A"), FontSize = 11, TextTrimming = TextTrimming.CharacterEllipsis }); var combo = new ComboBox { ItemsSource = values.ToArray(), SelectedIndex = selected }; wrap.Children.Add(combo); panel.Children.Add(wrap); return combo; }
    private ComboBox ChipChoice(Panel panel, string module, Action<Chip>? changed = null)
    {
        int selected = Array.FindIndex(Chips.All, c => c.Id == (_settings.GetValueOrDefault(module) ?? "335"));
        var combo = Choice(panel, "芯片型号", Chips.All.Select(c => c.Name), Math.Max(0, selected), 155);
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
    private TextBox CodeTab(TabControl tabs, string title = "配置代码")
    {
        var grid = new Grid { Margin = new Thickness(8) }; grid.RowDefinitions.Add(new RowDefinition()); grid.RowDefinitions.Add(new RowDefinition { Height = GridLength.Auto });
        var code = new TextBox { IsReadOnly = true, FontFamily = new FontFamily("Consolas"), FontSize = 11, AcceptsReturn = true, TextWrapping = TextWrapping.NoWrap, HorizontalScrollBarVisibility = ScrollBarVisibility.Auto, VerticalScrollBarVisibility = ScrollBarVisibility.Auto, Margin = new Thickness(0) }; grid.Children.Add(code);
        var actions = new StackPanel { Orientation = Orientation.Horizontal }; var copy = ActionButton(actions, "复制代码", () => Copy(code.Text)); copy.Tag = "layout-required"; Grid.SetRow(actions, 1); grid.Children.Add(actions);
        tabs.Items.Add(new TabItem { Header = title, Content = grid }); return code;
    }
    public void ConfigureTestLayout(double width, double height, double scale)
    { MinWidth = 1; MinHeight = 1; Width = width; Height = height; _renderScale = scale; }
    private void ValidateVisibleLayout()
    {
        void Visit(DependencyObject parent)
        {
            for (int i = 0; i < VisualTreeHelper.GetChildrenCount(parent); i++)
            {
                var child = VisualTreeHelper.GetChild(parent, i);
                if (child is FrameworkElement element && element.Tag?.ToString() == "layout-required" && element.IsVisible)
                {
                    var point = element.TranslatePoint(new Point(), _body);
                    if (element.ActualWidth <= 0 || element.ActualHeight <= 0 || point.X < -1 || point.Y < -1 || point.X + element.ActualWidth > _body.ActualWidth + 1 || point.Y + element.ActualHeight > _body.ActualHeight + 1)
                        throw new InvalidOperationException("操作控件超出固定页面：" + (element is ContentControl c ? c.Content?.ToString() : element.Name));
                }
                Visit(child);
            }
        }
        Visit(_body);
        if (_pages[_currentPage] is ScrollViewer) throw new InvalidOperationException("页面仍使用整页滚动容器。");
    }
    public async Task SmokeTest(string folder, bool testClipboard = false)
    {
        _smoke = true; Directory.CreateDirectory(folder);
        for (int i = 0; i < Titles.Length; i++)
        {
            ShowPage(i); UpdateLayout(); await Dispatcher.InvokeAsync(() => { }, DispatcherPriority.ApplicationIdle);
            var checks = _smokeChecks.ToArray(); foreach (var check in checks) check();
            if (_pages[i] is ScrollViewer pageScroll) pageScroll.ScrollToTop();
            UpdateLayout(); await Dispatcher.InvokeAsync(() => { }, DispatcherPriority.ApplicationIdle);
            ValidateVisibleLayout();
            var bitmap = new RenderTargetBitmap((int)Math.Ceiling(ActualWidth * _renderScale), (int)Math.Ceiling(ActualHeight * _renderScale), 96 * _renderScale, 96 * _renderScale, PixelFormats.Pbgra32); bitmap.Render(this);
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
        File.WriteAllText(Path.Combine(folder, "smoke-result.txt"), $"PASS {DateTime.Now:O}\n六个固定页面已载入、完成计算、关键控件边界检查并渲染截图。\n窗口 {Width}×{Height} DIP，渲染密度 {_renderScale * 100}%；渲染模拟不等于物理显示器DPI验证。\n寄存器条目 {_registers.Count}。\n{clipboardResult}\n");
    }
}
