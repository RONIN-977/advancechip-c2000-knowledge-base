using System.Globalization;
using System.IO;
using System.Text.Json;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using Fae.Core;

namespace Fae.Desktop;

public partial class MainWindow
{
    private List<RegisterEntry> _registers = [];
    private ManualPack _manual = new("", [], []);
    private readonly Dictionary<RegisterEntry, (string Meaning, string Body)> _searchBodies = [];
    private readonly Dictionary<string, BitmapImage> _manualImages = [];

    private UIElement RegisterPage()
    {
        var options = new JsonSerializerOptions { PropertyNameCaseInsensitive = true };
        _registers = (JsonSerializer.Deserialize<RegisterPack>(File.ReadAllText(Path.Combine(AppContext.BaseDirectory, "Data", "registers.json")), options) ?? throw new InvalidDataException("Register data is empty.")).Registers;
        _manual = JsonSerializer.Deserialize<ManualPack>(File.ReadAllText(Path.Combine(AppContext.BaseDirectory, "Data", "manual-content.json")), options) ?? throw new InvalidDataException("Manual content is empty.");
        foreach (var register in _registers)
        {
            var doc = ManualFor(register);
            var meaning = $"{doc?.Introduction} {string.Join(" ", doc?.Fields.Select(f => $"{f.Name} {f.Description} {f.Values}") ?? [])}";
            _searchBodies[register] = (meaning, $"{register.Name} {register.Address} {register.Peripheral} {register.Keywords} {register.Description} {meaning} {string.Join(" ", register.Fields.Select(f => f.Name))}");
        }

        var page = new Grid { Margin = new Thickness(10, 10, 10, 8) };
        page.RowDefinitions.Add(new RowDefinition { Height = GridLength.Auto });
        page.RowDefinitions.Add(new RowDefinition());
        var filters = new Grid { Margin = new Thickness(0, 0, 0, 7) };
        foreach (double width in new[] { 150.0, 120.0 }) filters.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(width) });
        filters.ColumnDefinitions.Add(new ColumnDefinition());
        var chipHost = new StackPanel(); var chip = ChipChoice(chipHost, "register"); filters.Children.Add(chipHost);
        if (chipHost.Children[0] is FrameworkElement chipWrap) chipWrap.Width = 150;
        var moduleHost = new StackPanel(); Grid.SetColumn(moduleHost, 1); filters.Children.Add(moduleHost);
        var modules = new[] { "全部外设" }.Concat(_registers.Select(r => r.Peripheral).Distinct().Order()).ToArray();
        var module = Choice(moduleHost, "外设", modules, 0, 120);
        var searchHost = new StackPanel(); Grid.SetColumn(searchHost, 2); filters.Children.Add(searchHost);
        searchHost.Children.Add(new TextBlock { Text = "寄存器 / 字段 / 地址", FontSize = 11, Foreground = Brush("#52617A") });
        var search = new TextBox { Margin = new Thickness(0, 2, 0, 0), MinWidth = 80 }; searchHost.Children.Add(search); page.Children.Add(filters);

        var content = new Grid(); Grid.SetRow(content, 1); page.Children.Add(content);
        content.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(150) });
        content.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(7) });
        content.ColumnDefinitions.Add(new ColumnDefinition());
        var list = new ListBox { BorderBrush = Brush("#D7DEE8"), BorderThickness = new Thickness(1), HorizontalContentAlignment = HorizontalAlignment.Stretch, FontSize = 11 };
        list.SetValue(ScrollViewer.HorizontalScrollBarVisibilityProperty, ScrollBarVisibility.Disabled);
        VirtualizingPanel.SetIsVirtualizing(list, true);
        var template = new FrameworkElementFactory(typeof(TextBlock));
        template.SetBinding(TextBlock.TextProperty, new System.Windows.Data.Binding("Name"));
        template.SetValue(TextBlock.TextWrappingProperty, TextWrapping.Wrap);
        template.SetValue(FrameworkElement.MarginProperty, new Thickness(4, 6, 3, 6));
        list.ItemTemplate = new DataTemplate { VisualTree = template }; content.Children.Add(list);
        var detail = new Grid { Background = Brushes.White }; Grid.SetColumn(detail, 2); content.Children.Add(detail);
        detail.RowDefinitions.Add(new RowDefinition { Height = GridLength.Auto });
        detail.RowDefinitions.Add(new RowDefinition { Height = GridLength.Auto });
        detail.RowDefinitions.Add(new RowDefinition());
        var header = new Grid { Margin = new Thickness(8, 4, 8, 5) };
        header.ColumnDefinitions.Add(new ColumnDefinition()); header.ColumnDefinitions.Add(new ColumnDefinition { Width = GridLength.Auto });
        var name = new TextBlock { FontSize = 15, FontWeight = FontWeights.SemiBold, TextWrapping = TextWrapping.Wrap, VerticalAlignment = VerticalAlignment.Center }; header.Children.Add(name);
        var address = new TextBlock { FontSize = 11, Margin = new Thickness(8, 0, 0, 0), VerticalAlignment = VerticalAlignment.Center }; Grid.SetColumn(address, 1); header.Children.Add(address); detail.Children.Add(header);
        var map = new StackPanel { Margin = new Thickness(8, 0, 8, 6) }; Grid.SetRow(map, 1); detail.Children.Add(map);
        var tabs = new TabControl { Margin = new Thickness(7, 0, 7, 5), BorderBrush = Brush("#D7DEE8") }; Grid.SetRow(tabs, 2); detail.Children.Add(tabs);
        var registerBody = new StackPanel { Margin = new Thickness(6) };
        var originalTab = new TabItem { Header = "Register Description", Content = new ScrollViewer { Content = registerBody, VerticalScrollBarVisibility = ScrollBarVisibility.Auto, HorizontalScrollBarVisibility = ScrollBarVisibility.Disabled } };
        var fieldLayout = new Grid { Margin = new Thickness(9) }; fieldLayout.RowDefinitions.Add(new RowDefinition { Height = GridLength.Auto }); fieldLayout.RowDefinitions.Add(new RowDefinition());
        var fieldTitle = new TextBlock { FontSize = 13, FontWeight = FontWeights.SemiBold, TextWrapping = TextWrapping.Wrap, Margin = new Thickness(0, 0, 0, 5) }; fieldLayout.Children.Add(fieldTitle);
        var fieldBody = new TextBox { IsReadOnly = true, AcceptsReturn = true, TextWrapping = TextWrapping.Wrap, VerticalScrollBarVisibility = ScrollBarVisibility.Auto, HorizontalScrollBarVisibility = ScrollBarVisibility.Disabled, BorderThickness = new Thickness(0), Padding = new Thickness(0), Margin = new Thickness(0), FontFamily = new FontFamily("Segoe UI"), FontSize = 12, VerticalContentAlignment = VerticalAlignment.Top }; Grid.SetRow(fieldBody, 1); fieldLayout.Children.Add(fieldBody);
        var fieldTab = new TabItem { Header = "Selected Field", Content = fieldLayout }; tabs.Items.Add(originalTab); tabs.Items.Add(fieldTab);
        var hitButtons = new List<(Button Button, RegisterField Field)>();
        RegisterEntry? selectedRegister = null;
        RegisterField? selectedField = null;

        void SelectField(RegisterField field, bool activate)
        {
            selectedField = field;
            var manualField = ManualFor(selectedRegister!)?.Fields.FirstOrDefault(f => f.Lsb == field.Lsb && f.Width == field.Width && f.Name.Equals(field.Name, StringComparison.OrdinalIgnoreCase))
                ?? ManualFor(selectedRegister!)?.Fields.FirstOrDefault(f => f.Lsb == field.Lsb && f.Width == field.Width);
            fieldTitle.Text = $"{(field.Width == 1 ? "Bit " + field.Lsb : "Bits " + (field.Lsb + field.Width - 1) + ":" + field.Lsb)}  {(IsReserved(field.Name) ? "Reserved" : field.Name)}";
            var pieces = new List<string>();
            if (manualField != null)
            {
                var attributes = string.Join("    ", new[] { manualField.Access, manualField.Reset.Length > 0 ? "Reset: " + manualField.Reset : "" }.Where(s => s.Length > 0));
                if (attributes.Length > 0) pieces.Add(attributes);
                if (!string.IsNullOrWhiteSpace(manualField.Description)) pieces.Add(manualField.Description);
                if (!string.IsNullOrWhiteSpace(manualField.Values) && !manualField.Description.Contains(manualField.Values, StringComparison.Ordinal)) pieces.Add(manualField.Values);
            }
            else if (IsReserved(field.Name)) pieces.Add("Reserved.");
            fieldBody.Text = string.Join("\n\n", pieces); fieldBody.ScrollToHome();
            foreach (var (button, target) in hitButtons)
            {
                bool active = target.Lsb == field.Lsb && target.Width == field.Width;
                button.Background = active ? Brush("#DCE8FC") : IsReserved(target.Name) ? Brush("#EFF1F4") : Brushes.White;
                button.Foreground = active ? Brush("#143F83") : Brush("#18263D");
            }
            if (activate) tabs.SelectedItem = fieldTab;
        }

        void BuildBitMap(RegisterEntry register)
        {
            map.Children.Clear(); hitButtons.Clear();
            for (int highest = register.Width - 1; highest >= 0; highest -= 16)
            {
                int low = Math.Max(0, highest - 15); int columns = highest - low + 1;
                var row = new Grid { Margin = new Thickness(0, 0, 0, 3) };
                for (int c = 0; c < columns; c++) row.ColumnDefinitions.Add(new ColumnDefinition());
                row.RowDefinitions.Add(new RowDefinition { Height = new GridLength(18) }); row.RowDefinitions.Add(new RowDefinition { Height = new GridLength(34) }); row.RowDefinitions.Add(new RowDefinition { Height = new GridLength(18) });
                for (int bit = highest; bit >= low; bit--)
                {
                    var field = register.Fields.FirstOrDefault(f => bit >= f.Lsb && bit < f.Lsb + f.Width) ?? new RegisterField("Reserved", bit, 1, "", "", "", "", "");
                    var number = new Button { Content = bit.ToString(), Padding = new Thickness(0), Margin = new Thickness(0), FontSize = 10, BorderThickness = new Thickness(0.5), BorderBrush = Brush("#B9C4D2") };
                    number.Click += (_, _) => SelectField(field, true); Grid.SetColumn(number, highest - bit); row.Children.Add(number); hitButtons.Add((number, field));
                }
                foreach (var field in register.Fields.Where(f => f.Lsb <= highest && f.Lsb + f.Width > low))
                {
                    int hi = Math.Min(highest, field.Lsb + field.Width - 1), lo = Math.Max(low, field.Lsb);
                    int start = highest - hi, span = hi - lo + 1;
                    var text = new TextBlock { Text = IsReserved(field.Name) ? "Reserved" : field.Name, TextWrapping = TextWrapping.Wrap, FontSize = 9, TextAlignment = TextAlignment.Center, VerticalAlignment = VerticalAlignment.Center };
                    var button = new Button { Content = text, Padding = new Thickness(1, 0, 1, 0), Margin = new Thickness(0), BorderThickness = new Thickness(0.5), BorderBrush = Brush("#B9C4D2"), HorizontalContentAlignment = HorizontalAlignment.Stretch };
                    button.Click += (_, _) => SelectField(field, true); Grid.SetRow(button, 1); Grid.SetColumn(button, start); Grid.SetColumnSpan(button, span); row.Children.Add(button); hitButtons.Add((button, field));
                    var mf = ManualFor(register)?.Fields.FirstOrDefault(f => f.Lsb == field.Lsb && f.Width == field.Width);
                    var attrs = new TextBlock { Text = string.Join("-", new[] { mf?.Access ?? "", mf?.Reset ?? "" }.Where(s => s.Length > 0)), FontSize = 8, TextAlignment = TextAlignment.Center, TextTrimming = TextTrimming.CharacterEllipsis, VerticalAlignment = VerticalAlignment.Center };
                    var attrBorder = new Border { Child = attrs, BorderBrush = Brush("#B9C4D2"), BorderThickness = new Thickness(0.5) }; Grid.SetRow(attrBorder, 2); Grid.SetColumn(attrBorder, start); Grid.SetColumnSpan(attrBorder, span); row.Children.Add(attrBorder);
                }
                map.Children.Add(row);
            }
        }

        void ShowRegister()
        {
            if (list.SelectedItem is not RegisterEntry register) return;
            selectedRegister = register; name.Text = register.Name; address.Text = register.Address + "  ·  " + register.Width + " bits";
            registerBody.Children.Clear();
            var doc = ManualFor(register);
            if (doc != null)
            {
                if (doc.Pages.Count > 0)
                    foreach (var imagePage in doc.Pages)
                    {
                        var imagePath = Path.Combine(AppContext.BaseDirectory, imagePage.ImagePath.Replace('/', Path.DirectorySeparatorChar));
                        if (!_manualImages.TryGetValue(imagePath, out var bitmap))
                        {
                            bitmap = new BitmapImage(); bitmap.BeginInit(); bitmap.CacheOption = BitmapCacheOption.OnLoad; bitmap.UriSource = new Uri(imagePath); bitmap.EndInit(); bitmap.Freeze(); _manualImages[imagePath] = bitmap;
                        }
                        registerBody.Children.Add(new Image { Source = bitmap, Stretch = Stretch.Uniform, HorizontalAlignment = HorizontalAlignment.Stretch, Margin = new Thickness(0, 0, 0, 8) });
                    }
                else registerBody.Children.Add(new TextBox { Text = doc.Introduction, IsReadOnly = true, AcceptsReturn = true, TextWrapping = TextWrapping.Wrap, BorderThickness = new Thickness(0), FontFamily = new FontFamily("Segoe UI"), FontSize = 12, Padding = new Thickness(0), Margin = new Thickness(0) });
            }
            BuildBitMap(register);
            var first = register.Fields.FirstOrDefault(f => !IsReserved(f.Name)) ?? register.Fields.FirstOrDefault();
            if (first != null) SelectField(first, false);
        }

        void Refresh()
        {
            var terms = QueryTerms(search.Text);
            var entries = _registers.Where(r => r.ChipId == SelectedChip(chip).Id && (module.SelectedIndex == 0 || r.Peripheral == module.SelectedItem?.ToString()))
                .Select(r => (Register: r, Score: SearchScore(r, terms))).Where(p => p.Score >= 0).OrderByDescending(p => p.Score).ThenBy(p => p.Register.Name).Select(p => p.Register).ToArray();
            list.ItemsSource = entries;
            if (entries.Length > 0)
                list.SelectedItem = terms.Length == 0 ? entries.FirstOrDefault(r => r.Name.EndsWith(".CANBTC") || r.Name.EndsWith(".CAN_BTR")) ?? entries[0] : entries[0];
            else { name.Text = ""; address.Text = ""; map.Children.Clear(); registerBody.Children.Clear(); fieldBody.Clear(); fieldTitle.Text = ""; }
        }
        var debounce = new System.Windows.Threading.DispatcherTimer { Interval = TimeSpan.FromMilliseconds(170) }; debounce.Tick += (_, _) => { debounce.Stop(); Refresh(); };
        list.SelectionChanged += (_, _) => ShowRegister();
        chip.SelectionChanged += (_, _) => Refresh(); module.SelectionChanged += (_, _) => Refresh(); search.TextChanged += (_, _) => { debounce.Stop(); debounce.Start(); }; Refresh();
        bool smokeDone = false;
        _smokeChecks.Add(() =>
        {
            if (smokeDone) return;
            int original = chip.SelectedIndex;
            foreach (var chipIndex in Enumerable.Range(0, Chips.All.Length))
            {
                chip.SelectedIndex = chipIndex; module.SelectedIndex = 0;
                foreach (string query in new[] { "CAN波特率", "通讯", "串口波特率", "TBCTL" })
                { search.Text = query; Refresh(); if (list.Items.Count == 0) throw new InvalidDataException("Register search failed: " + query); }
                search.Text = ""; Refresh();
                var candidates = _registers.Where(r => r.ChipId == Chips.All[chipIndex].Id && r.Width is 16 or 32).GroupBy(r => r.Width).Select(g => g.First()).ToArray();
                foreach (var register in candidates)
                {
                    list.SelectedItem = register;
                    foreach (var (button, field) in hitButtons.Where(p => p.Button.Content is string).ToArray())
                    {
                        button.RaiseEvent(new RoutedEventArgs(ButtonBase.ClickEvent));
                        if (selectedField?.Lsb != field.Lsb || selectedField.Width != field.Width) throw new InvalidDataException("Bit field selection failed.");
                    }
                    if (ManualFor(register) == null) throw new InvalidDataException("Register manual missing: " + register.Name);
                }
            }
            chip.SelectedIndex = original; search.Text = ""; debounce.Stop(); Refresh(); tabs.SelectedItem = originalTab; smokeDone = true;
        });
        return page;
    }

    private ManualRegister? ManualFor(RegisterEntry register) => _manual.RegisterDocuments.TryGetValue(register.ChipId + "|" + register.Name, out var key) && _manual.Documents.TryGetValue(key, out var document) ? document : null;
    private static bool IsReserved(string name) => name.StartsWith("rsvd", StringComparison.OrdinalIgnoreCase) || name.Equals("Reserved", StringComparison.OrdinalIgnoreCase);
    private static string[] QueryTerms(string query)
    {
        query = query.Replace("通讯", "通信").Replace("通訊", "通信").Replace("串口", "SCI ").Replace("脉宽调制", "PWM ");
        query = Regex.Replace(query, @"(?<=[A-Za-z])(?=[\u4e00-\u9fff])|(?<=[\u4e00-\u9fff])(?=[A-Za-z])", " ");
        return query.Split((char[]?)null, StringSplitOptions.RemoveEmptyEntries);
    }
    private int SearchScore(RegisterEntry register, string[] terms)
    {
        int score = 0; var cached = _searchBodies[register];
        foreach (var term in terms)
        {
            if (term.StartsWith("0x", StringComparison.OrdinalIgnoreCase) && uint.TryParse(term[2..], NumberStyles.HexNumber, CultureInfo.InvariantCulture, out var addr) && addr == Convert.ToUInt32(register.Address[2..], 16)) score += 100;
            else if (register.Name.Equals(term, StringComparison.OrdinalIgnoreCase)) score += 100;
            else if (register.Name.Contains(term, StringComparison.OrdinalIgnoreCase)) score += 30;
            else if (cached.Meaning.Contains(term, StringComparison.OrdinalIgnoreCase)) score += 15;
            else if (cached.Body.Contains(term, StringComparison.OrdinalIgnoreCase)) score += 5;
            else if (term.Length > 1 && term.All(char.IsAsciiLetterOrDigit) && IsSubsequence(term, register.Name)) score++;
            else return -1;
        }
        return score;
    }
    private static bool IsSubsequence(string term, string text)
    { int i = 0; foreach (char c in text) if (i < term.Length && char.ToUpperInvariant(c) == char.ToUpperInvariant(term[i])) i++; return i == term.Length; }
}
