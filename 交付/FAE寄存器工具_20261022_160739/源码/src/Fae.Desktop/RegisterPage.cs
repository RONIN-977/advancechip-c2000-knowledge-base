using System.IO;
using System.Text.Json;
using System.Windows;
using System.Windows.Controls;
using Fae.Core;

namespace Fae.Desktop;
public partial class MainWindow
{
    private List<RegisterEntry> _registers = [];
    private readonly Dictionary<RegisterEntry, (string Meaning, string Body)> _searchBodies = [];
    private UIElement RegisterPage()
    {
        var page = Page(0); string loadMessage;
        try
        {
            var pack = JsonSerializer.Deserialize<RegisterPack>(File.ReadAllText(Path.Combine(AppContext.BaseDirectory, "Data", "registers.json")), new JsonSerializerOptions { PropertyNameCaseInsensitive = true }) ?? throw new InvalidDataException("资料包为空");
            _registers = pack.Registers;
            foreach (var r in _registers)
            {
                string meaning = $"{r.Description} {string.Join(" ", r.Fields.Select(f => $"{f.Name} {f.Description} {f.Values} {f.Notes}"))}";
                _searchBodies[r] = (meaning, $"{r.Name} {r.Address} {r.Peripheral} {r.Keywords} {meaning}");
            }
            loadMessage = $"离线资料 {pack.Version} · {pack.GeneratedOn} · 共 {_registers.Count:N0} 个寄存器条目";
        }
        catch (Exception ex) { loadMessage = "资料未载入：" + ex.Message + "。请将 Data 文件夹与 EXE 放在一起。"; }
        var filter = Card(page); var row = Row(filter); var chip = ChipChoice(row, "register");
        var peripherals = new[] { "全部外设" }.Concat(_registers.Select(r => r.Peripheral).Distinct().Order()).ToArray();
        var peripheral = Choice(row, "外设分类", peripherals, 0, 200);
        var search = Input(row, "搜索中文、英文、字段名或地址（多个词用空格分隔）", "", 420);
        var count = Note(filter, loadMessage);
        var grid = new Grid(); grid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(0.85, GridUnitType.Star) }); grid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(1.45, GridUnitType.Star) });
        var list = new ListBox { Height = 545, BorderThickness = new Thickness(0), Background = System.Windows.Media.Brushes.Transparent, HorizontalContentAlignment = HorizontalAlignment.Stretch };
        list.SetValue(ScrollViewer.HorizontalScrollBarVisibilityProperty, ScrollBarVisibility.Disabled);
        var factory = new FrameworkElementFactory(typeof(StackPanel)); factory.SetValue(StackPanel.MarginProperty, new Thickness(9, 9, 9, 10));
        var name = new FrameworkElementFactory(typeof(TextBlock)); name.SetBinding(TextBlock.TextProperty, new System.Windows.Data.Binding("Name")); name.SetValue(TextBlock.FontWeightProperty, FontWeights.SemiBold); name.SetValue(TextBlock.FontSizeProperty, 15.0); factory.AppendChild(name);
        var desc = new FrameworkElementFactory(typeof(TextBlock)); desc.SetBinding(TextBlock.TextProperty, new System.Windows.Data.Binding("Description")); desc.SetValue(TextBlock.TextWrappingProperty, TextWrapping.Wrap); desc.SetValue(TextBlock.MarginProperty, new Thickness(0, 5, 0, 0)); desc.SetValue(TextBlock.ForegroundProperty, Brush("#64748B")); factory.AppendChild(desc);
        var addr = new FrameworkElementFactory(typeof(TextBlock)); addr.SetBinding(TextBlock.TextProperty, new System.Windows.Data.Binding("Address") { StringFormat = "地址：{0}（16 位字）" }); addr.SetValue(TextBlock.FontSizeProperty, 11.0); addr.SetValue(TextBlock.ForegroundProperty, Brush("#7E8AA0")); addr.SetValue(TextBlock.MarginProperty, new Thickness(0, 5, 0, 0)); factory.AppendChild(addr);
        list.ItemTemplate = new DataTemplate { VisualTree = factory };
        grid.Children.Add(new Border { Child = list, Background = System.Windows.Media.Brushes.White, CornerRadius = new CornerRadius(10), BorderBrush = Brush("#E0E7F1"), BorderThickness = new Thickness(1), Margin = new Thickness(0, 0, 14, 0) });
        var detail = new StackPanel { Margin = new Thickness(18) }; var detailScroll = Scroll(detail); detailScroll.Height = 545;
        var detailBorder = new Border { Child = detailScroll, Background = System.Windows.Media.Brushes.White, CornerRadius = new CornerRadius(10), BorderBrush = Brush("#E0E7F1"), BorderThickness = new Thickness(1) }; Grid.SetColumn(detailBorder, 1); grid.Children.Add(detailBorder); page.Children.Add(grid);
        Note(page, "资料解释采用所示 TI 基线；进芯差异单独标注。缺项显示“暂无资料”，不代表数值为零。", "#64748B");
        void ShowDetail()
        {
            detail.Children.Clear();
            if (list.SelectedItem is not RegisterEntry r) { Note(detail, "选择左侧寄存器查看中文说明与逐字段定义。"); return; }
            detail.Children.Add(new TextBlock { Text = r.Name, FontSize = 23, FontWeight = FontWeights.SemiBold, TextWrapping = TextWrapping.Wrap });
            Note(detail, $"{r.Peripheral}   ·   {r.Address}（C28x 16 位字地址）   ·   {r.Width} 位");
            Note(detail, r.Description, "#18263D");
            Note(detail, "寄存器基线：" + SelectedChip(chip).TiModel);
            if (!string.IsNullOrWhiteSpace(r.Notes)) Note(detail, r.Notes, "#915B14");
            foreach (var f in r.Fields.OrderByDescending(f => f.Lsb))
            {
                var field = new StackPanel { Margin = new Thickness(0, 10, 0, 0) };
                var bits = f.Width == 1 ? $"bit {f.Lsb}" : $"bits {f.Lsb + f.Width - 1}:{f.Lsb}";
                field.Children.Add(new TextBlock { Text = $"{f.Name}    [{bits}]", FontWeight = FontWeights.SemiBold, TextWrapping = TextWrapping.Wrap });
                Note(field, $"访问：{Missing(f.Access)}    复位：{Missing(f.Reset)}");
                Note(field, Missing(f.Description), "#26364F");
                if (!string.IsNullOrWhiteSpace(f.Values)) Note(field, "取值：" + f.Values);
                if (!string.IsNullOrWhiteSpace(f.Notes)) Note(field, "注意：" + f.Notes, "#915B14");
                detail.Children.Add(new Border { Child = field, BorderBrush = Brush("#E6ECF5"), BorderThickness = new Thickness(0, 1, 0, 0), Padding = new Thickness(0, 5, 0, 5) });
            }
            if (r.Fields.Count == 0) Note(detail, "逐字段解释：暂无资料。");
            Note(detail, "资料来源：" + Missing(r.Source));
            ActionButton(detail, "复制寄存器说明", () => Copy($"{r.Name} {r.Address}\n{r.Description}\n{string.Join("\n", r.Fields.Select(f => $"{f.Name} [{f.Lsb + f.Width - 1}:{f.Lsb}] {f.Access} {f.Description} {f.Values} {f.Notes}"))}\n来源：{r.Source}"));
        }
        void Refresh()
        {
            string id = SelectedChip(chip).Id; string category = peripheral.SelectedItem?.ToString() ?? "全部外设"; var terms = QueryTerms(search.Text);
            var matches = _registers.Where(r => r.ChipId == id && (category == "全部外设" || r.Peripheral == category)).Select(r => (Entry: r, Score: SearchScore(r, terms))).Where(r => r.Score >= 0).OrderByDescending(r => r.Score).ThenBy(r => r.Entry.Peripheral).ThenBy(r => r.Entry.Name).Select(r => r.Entry).ToArray();
            list.ItemsSource = matches; count.Text = $"{SelectedChip(chip).Name} / {SelectedChip(chip).TiModel}  ·  找到 {matches.Length:N0} 条   |   {loadMessage}";
            if (matches.Length > 0) list.SelectedIndex = 0; else { detail.Children.Clear(); Note(detail, "没有匹配项。可减少关键词、选择“全部外设”，或使用寄存器英文名。"); }
        }
        var debounce = new System.Windows.Threading.DispatcherTimer { Interval = TimeSpan.FromMilliseconds(170) }; debounce.Tick += (_, _) => { debounce.Stop(); Refresh(); };
        list.SelectionChanged += (_, _) => ShowDetail(); chip.SelectionChanged += (_, _) => Refresh(); peripheral.SelectionChanged += (_, _) => Refresh(); search.TextChanged += (_, _) => { debounce.Stop(); debounce.Start(); }; Refresh();
        _smokeChecks.Add(() =>
        {
            if (_registers.Count == 0) throw new InvalidDataException("寄存器资料包为空，不能通过启动自检。");
            int original = chip.SelectedIndex;
            for (int i = 0; i < Chips.All.Length; i++)
            {
                chip.SelectedIndex = i; peripheral.SelectedIndex = 0; search.Text = ""; Refresh();
                if (list.Items.Count == 0) throw new InvalidDataException("型号无寄存器资料：" + Chips.All[i].Name);
                var r = (RegisterEntry)list.Items[0];
                var chinese = new string(r.Description.Where(c => c >= 0x4e00 && c <= 0x9fff).Take(2).ToArray());
                var compactAddress = "0x" + Convert.ToUInt32(r.Address[2..], 16).ToString("X");
                foreach (var query in new[] { r.Name, r.Address, compactAddress, r.Fields.FirstOrDefault()?.Name ?? r.Name, chinese, "通讯", "CAN波特率", "串口波特率" }.Where(s => s.Length > 0))
                { search.Text = query; Refresh(); if (list.Items.Count == 0) throw new InvalidDataException("寄存器搜索自检失败：" + query); }
            }
            chip.SelectedIndex = original; search.Text = ""; debounce.Stop(); Refresh();
        });
        return Scroll(page);
    }
    private static string Missing(string? value) => string.IsNullOrWhiteSpace(value) ? "暂无资料" : value;
    private static string[] QueryTerms(string query)
    {
        query = query.Replace("通讯", "通信").Replace("通訊", "通信").Replace("串口", "SCI ").Replace("脉宽调制", "PWM ");
        query = System.Text.RegularExpressions.Regex.Replace(query, @"(?<=[A-Za-z])(?=[\u4e00-\u9fff])|(?<=[\u4e00-\u9fff])(?=[A-Za-z])", " ");
        return query.Split((char[]?)null, StringSplitOptions.RemoveEmptyEntries);
    }
    private int SearchScore(RegisterEntry r, string[] terms)
    {
        if (terms.Length == 0) return 0;
        var cached = _searchBodies[r];
        int score = 0;
        foreach (var term in terms)
        {
            if (term.StartsWith("0x", StringComparison.OrdinalIgnoreCase) && uint.TryParse(term[2..], System.Globalization.NumberStyles.HexNumber, System.Globalization.CultureInfo.InvariantCulture, out uint address) && address == Convert.ToUInt32(r.Address[2..], 16)) score += 100;
            else if (r.Name.Equals(term, StringComparison.OrdinalIgnoreCase)) score += 100;
            else if (r.Name.Contains(term, StringComparison.OrdinalIgnoreCase)) score += 30;
            else if (cached.Meaning.Contains(term, StringComparison.OrdinalIgnoreCase)) score += 15;
            else if (cached.Body.Contains(term, StringComparison.OrdinalIgnoreCase)) score += 5;
            else if (term.Length > 1 && term.All(char.IsAsciiLetterOrDigit) && IsSubsequence(term, r.Name)) score++;
            else return -1;
        }
        return score;
    }
    private static bool IsSubsequence(string term, string text)
    { int i = 0; foreach (char c in text) if (i < term.Length && char.ToUpperInvariant(c) == char.ToUpperInvariant(term[i])) i++; return i == term.Length; }
}
