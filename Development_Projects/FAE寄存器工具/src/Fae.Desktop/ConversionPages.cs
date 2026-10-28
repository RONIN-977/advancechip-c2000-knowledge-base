using System.Windows;
using System.Windows.Controls;
using Fae.Core;

namespace Fae.Desktop;
public partial class MainWindow
{
    private UIElement AdcPage()
    {
        var page = CompactPage(3); var columns = new Grid(); columns.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(0.95, GridUnitType.Star) }); columns.ColumnDefinitions.Add(new ColumnDefinition()); Grid.SetRow(columns, 1); page.Children.Add(columns);
        var left = new StackPanel { Margin = new Thickness(0, 0, 8, 0) }; columns.Children.Add(left); var input = Card(left, "采样参数"); var row = Row(input); var chip = ChipChoice(row, "adc"); var bits = Input(row, "分辨率 / bit", "12", 100); bits.IsReadOnly = true;
        var direction = Choice(input, "计算方向", ["原始码 → 电压 / 电流", "ADC 引脚电压 → 原始码", "分压前电压 → 原始码", "电流 → 原始码"], 0, 260);
        row = Row(input); var value = Input(row, "输入值 / code、V、A", "2048", 135); var reference = Input(row, "参考电压 / V", SelectedChip(chip).Id == "335" ? "3.0" : "3.3", 125);
        row = Row(input); var divider = Input(row, "分压倍率 Vin/Vpin", "1", 135); var shunt = Input(row, "采样电阻 / Ω", "0.01", 125);
        row = Row(input); var gain = Input(row, "放大倍数", "10", 135); var offset = Input(row, "零点电压 / V", "1.65", 125);
        Note(input, "输入已对齐的12位原始码（0～4095）。\n分压倍率 =（上电阻 + 下电阻）/ 下电阻。");
        var actions = Row(input);
        var output = new Grid { Margin = new Thickness(10) }; output.RowDefinitions.Add(new RowDefinition { Height = GridLength.Auto }); output.RowDefinitions.Add(new RowDefinition());
        var values = new Grid(); values.ColumnDefinitions.Add(new ColumnDefinition()); values.ColumnDefinitions.Add(new ColumnDefinition()); var code = Metric(values, "原始码", 0, 0); var pin = Metric(values, "ADC 引脚电压", 1, 0); var voltage = Metric(values, "分压前输入电压", 0, 1); var current = Metric(values, "电流", 1, 1); output.Children.Add(values); var explanations = new StackPanel(); var formula = Note(explanations, ""); var detail = Note(explanations, ""); var scroller = Scroll(explanations); Grid.SetRow(scroller, 1); output.Children.Add(scroller);
        var surface = new Border { Child = output, Background = System.Windows.Media.Brushes.White, CornerRadius = new CornerRadius(5), BorderThickness = new Thickness(1), BorderBrush = Brush("#E0E7F1") }; Grid.SetColumn(surface, 1); columns.Children.Add(surface); string? copyText = null;
        void Calculate() => Guard(() =>
        {
            double vref = Number(reference), ratio = Number(divider), r = Number(shunt), amplification = Number(gain), zero = Number(offset), entered = Number(value);
            var result = direction.SelectedIndex == 0 ? AdcCalculator.FromCode(Integer(value), vref, ratio, r, amplification, zero, 12) : AdcCalculator.FromVoltage(direction.SelectedIndex switch { 1 => entered, 2 => entered / ratio, _ => entered * r * amplification + zero }, vref, ratio, r, amplification, zero, 12);
            code.Text = result.Code.ToString(); pin.Text = result.PinVolts.ToString("F6") + " V"; voltage.Text = result.InputVolts.ToString("F6") + " V"; current.Text = result.CurrentAmps.ToString("F6") + " A"; formula.Text = result.Formula;
            detail.Text = SelectedChip(chip).Id == "335" ? "335：AdcRegs.ADCRESULTn（0x7108～0x7117）左对齐，先右移4位；AdcMirror（0xB00～0xB0F）右对齐。" : "12位单端原始码。电压反算采用最近码值对应的量化电压。";
            copyText = $"原始码 {result.Code}\nADC引脚 {result.PinVolts:G12} V\n输入电压 {result.InputVolts:G12} V\n电流 {result.CurrentAmps:G12} A\n{result.Formula}"; _status.Text = "ADC 换算完成";
        });
        ActionButton(actions, "计算", Calculate, true).Tag = "layout-required"; ActionButton(actions, "复制结果", () => Copy(copyText ?? "")).Tag = "layout-required";
        void Invalidate() { copyText = null; foreach (var t in new[] { code, pin, voltage, current }) t.Text = "—"; formula.Text = "参数已修改，请重新计算。"; }
        foreach (var t in new[] { value, reference, divider, shunt, gain, offset }) t.TextChanged += (_, _) => Invalidate(); chip.SelectionChanged += (_, _) => { reference.Text = SelectedChip(chip).Id == "335" ? "3.0" : "3.3"; Invalidate(); }; direction.SelectionChanged += (_, _) => Invalidate(); Calculate(); _smokeChecks.Add(Calculate); return page;
    }
    private static TextBlock Metric(Grid grid, string title, int column, int row)
    {
        while (grid.RowDefinitions.Count <= row) grid.RowDefinitions.Add(new RowDefinition { Height = GridLength.Auto }); var panel = new StackPanel { Margin = new Thickness(0, 0, 7, 14) }; panel.Children.Add(new TextBlock { Text = title, FontSize = 11, Foreground = Brush("#64748B") }); var text = new TextBlock { Text = "—", FontSize = 19, FontWeight = FontWeights.SemiBold, Margin = new Thickness(0, 5, 0, 0), Foreground = Brush("#1D4ED8"), TextWrapping = TextWrapping.Wrap }; panel.Children.Add(text); Grid.SetColumn(panel, column); Grid.SetRow(panel, row); grid.Children.Add(panel); return text;
    }
    private UIElement AddressPage()
    {
        var page = CompactPage(4); var columns = new Grid(); columns.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(0.9, GridUnitType.Star) }); columns.ColumnDefinitions.Add(new ColumnDefinition()); Grid.SetRow(columns, 1); page.Children.Add(columns); var left = new StackPanel { Margin = new Thickness(0, 0, 8, 0) }; columns.Children.Add(left); var input = Card(left, "地址参数"); var direction = Choice(input, "计算方向", ["起止地址 → 容量", "起始地址 + 容量 → 末地址"], 0, 270); var unit = Choice(input, "寻址单位", ["C28x：1 地址 = 16 位字", "字节：1 地址 = 1 byte"], 0, 270); var start = Input(input, "起始地址", "0x3E8000", 270); var end = Input(input, "末地址（包含）", "0x3EFFFF", 270); var bytes = Input(input, "容量 / 字节", "65536", 270); bytes.IsEnabled = false; Note(input, "十六进制写0x前缀，否则按十进制。\n区间包含首尾；KB = 1024 byte。"); var actions = Row(input);
        var output = new Grid { Margin = new Thickness(12) }; output.RowDefinitions.Add(new RowDefinition { Height = GridLength.Auto }); output.RowDefinitions.Add(new RowDefinition()); var result = new TextBlock { FontSize = 15, LineHeight = 29, TextWrapping = TextWrapping.Wrap }; output.Children.Add(result); var explanations = new StackPanel(); var notes = Note(explanations, ""); var scroller = Scroll(explanations); Grid.SetRow(scroller, 1); output.Children.Add(scroller); var surface = new Border { Child = output, Background = System.Windows.Media.Brushes.White, BorderBrush = Brush("#E0E7F1"), BorderThickness = new Thickness(1), CornerRadius = new CornerRadius(5) }; Grid.SetColumn(surface, 1); columns.Children.Add(surface); string? copyText = null;
        void Calculate() => Guard(() => { var r = direction.SelectedIndex == 0 ? AddressCalculator.FromRange(start.Text.Trim(), end.Text.Trim(), unit.SelectedIndex == 0) : AddressCalculator.FromSize(start.Text.Trim(), bytes.Text.Trim(), unit.SelectedIndex == 0); result.Text = $"起始  0x{r.Start:X}\n末址  0x{r.End:X}（包含）\n\n{r.Bytes:N0} byte\n{r.Kilobytes:F6} KB\n\n{r.Words:N0} word\n{r.Kwords:F6} Kword"; notes.Text = r.Notes; copyText = result.Text + "\n" + r.Notes; _status.Text = "地址计算完成"; });
        ActionButton(actions, "计算", Calculate, true).Tag = "layout-required"; ActionButton(actions, "复制结果", () => Copy(copyText ?? "")).Tag = "layout-required";
        void Invalidate() { result.Text = "参数已修改，请重新计算。"; copyText = null; notes.Text = ""; }
        direction.SelectionChanged += (_, _) => { end.IsEnabled = direction.SelectedIndex == 0; bytes.IsEnabled = direction.SelectedIndex == 1; Invalidate(); }; unit.SelectionChanged += (_, _) => Invalidate(); foreach (var t in new[] { start, end, bytes }) t.TextChanged += (_, _) => Invalidate(); Calculate(); _smokeChecks.Add(Calculate); return page;
    }
}
