using System.Windows;
using System.Windows.Controls;
using Fae.Core;

namespace Fae.Desktop;
public partial class MainWindow
{
    private UIElement AdcPage()
    {
        var page = Page(3); var input = Card(page, "采样与模拟链参数"); var row = Row(input); var chip = ChipChoice(row, "adc"); var direction = Choice(row, "计算方向", ["原始码 → 电压 / 电流", "ADC 引脚电压 → 原始码", "分压前电压 → 原始码", "电流 → 原始码"], 0, 285); var value = Input(row, "输入值 / code、V 或 A", "2048", 200);
        row = Row(input); var bits = Input(row, "ADC 分辨率 / bit", "12", 160); bits.IsReadOnly = true; var reference = Input(row, "参考电压 / V", SelectedChip(chip).Id == "335" ? "3.0" : "3.3", 170); var divider = Input(row, "分压倍率 Vin / Vpin", "1", 215);
        row = Row(input); var shunt = Input(row, "采样电阻 / Ω", "0.01", 185); var gain = Input(row, "放大倍数", "10", 170); var offset = Input(row, "电流零点引脚电压 / V", "1.65", 230);
        Note(input, "原始码须为已完成对齐的12位数值（0～4095）。分压倍率 =（上电阻 + 下电阻）/ 下电阻。电流 =（ADC 引脚电压 − 零点电压）/（采样电阻 × 放大倍数）。参考电压及模拟链参数请按实际板卡填写。");
        var resultCard = Card(page, "换算结果"); var values = new Grid(); values.ColumnDefinitions.Add(new ColumnDefinition()); values.ColumnDefinitions.Add(new ColumnDefinition());
        var code = Metric(values, "原始码", 0, 0); var pin = Metric(values, "ADC 引脚电压", 1, 0); var voltage = Metric(values, "分压前输入电压", 0, 1); var current = Metric(values, "电流", 1, 1); resultCard.Children.Add(values); var formula = Note(resultCard, "");
        var detail = Note(resultCard, ""); string? copyText = null;
        void Calculate() => Guard(() =>
        {
            double vref = Number(reference), ratio = Number(divider), r = Number(shunt), amplification = Number(gain), zero = Number(offset);
            double entered = Number(value);
            var result = direction.SelectedIndex == 0 ? AdcCalculator.FromCode(Integer(value), vref, ratio, r, amplification, zero, 12) : AdcCalculator.FromVoltage(direction.SelectedIndex switch { 1 => entered, 2 => entered / ratio, _ => entered * r * amplification + zero }, vref, ratio, r, amplification, zero, 12);
            code.Text = result.Code.ToString(); pin.Text = result.PinVolts.ToString("F6") + " V"; voltage.Text = result.InputVolts.ToString("F6") + " V"; current.Text = result.CurrentAmps.ToString("F6") + " A";
            formula.Text = result.Formula; detail.Text = $"{SelectedChip(chip).Name} / {SelectedChip(chip).TiModel} · 12 位单端原始码，范围 0～4095。\n电压反算显示最近码值对应的量化电压；不包含参考源误差、ADC 偏移、增益误差或板级校准。" + (SelectedChip(chip).Id == "335" ? "\n335：AdcRegs.ADCRESULTn（PF2，0x7108～0x7117）左对齐，输入前需 >>4；AdcMirror（0xB00～0xB0F）右对齐，无需移位。来源：SPRUI07，第490页。" : "");
            copyText = $"原始码 {result.Code}\nADC引脚 {result.PinVolts:G12} V\n输入电压 {result.InputVolts:G12} V\n电流 {result.CurrentAmps:G12} A\n{result.Formula}"; _status.Text = "ADC 换算完成";
        });
        var actions = Row(input); ActionButton(actions, "计算换算结果", Calculate, true); ActionButton(actions, "复制结果", () => Copy(copyText ?? ""));
        void Invalidate() { copyText = null; foreach (var t in new[] { code, pin, voltage, current }) t.Text = "—"; formula.Text = "参数已修改，请重新计算。"; }
        foreach (var t in new[] { value, reference, divider, shunt, gain, offset }) t.TextChanged += (_, _) => Invalidate(); chip.SelectionChanged += (_, _) => { reference.Text = SelectedChip(chip).Id == "335" ? "3.0" : "3.3"; Invalidate(); }; direction.SelectionChanged += (_, _) => Invalidate();
        Calculate(); _smokeChecks.Add(Calculate); return Scroll(page);
    }
    private static TextBlock Metric(Grid grid, string title, int column, int row)
    {
        while (grid.RowDefinitions.Count <= row) grid.RowDefinitions.Add(new RowDefinition());
        var panel = new StackPanel { Margin = new Thickness(0, 0, 16, 18) }; panel.Children.Add(new TextBlock { Text = title, FontSize = 12, Foreground = Brush("#64748B") });
        var text = new TextBlock { Text = "—", FontSize = 27, FontWeight = FontWeights.SemiBold, Margin = new Thickness(0, 7, 0, 0), Foreground = Brush("#1D4ED8"), TextWrapping = TextWrapping.Wrap }; panel.Children.Add(text); Grid.SetColumn(panel, column); Grid.SetRow(panel, row); grid.Children.Add(panel); return text;
    }
    private UIElement AddressPage()
    {
        var page = Page(4); var input = Card(page, "地址与空间"); var row = Row(input); var direction = Choice(row, "计算方向", ["起止地址 → 容量", "起始地址 + 容量 → 末地址"], 0, 280); var unit = Choice(row, "寻址单位", ["C28x：1 地址 = 16 位字", "字节：1 地址 = 1 byte"], 0, 270);
        row = Row(input); var start = Input(row, "起始地址（0x 前缀为十六进制）", "0x3E8000", 310); var end = Input(row, "末地址（包含该地址）", "0x3EFFFF", 270); var bytes = Input(row, "容量 / 字节", "65536", 200); bytes.IsEnabled = false;
        Note(input, "不带前缀按十进制解析；十六进制请写 0x。区间始终包含首尾，KB = 1024 byte，Kword = 1024 个 16 位字。");
        var output = Card(page, "空间计算结果"); var result = new TextBlock { FontSize = 20, LineHeight = 35, TextWrapping = TextWrapping.Wrap }; output.Children.Add(result); var notes = Note(output, ""); string? copyText = null;
        void Calculate() => Guard(() =>
        {
            var r = direction.SelectedIndex == 0 ? AddressCalculator.FromRange(start.Text.Trim(), end.Text.Trim(), unit.SelectedIndex == 0) : AddressCalculator.FromSize(start.Text.Trim(), bytes.Text.Trim(), unit.SelectedIndex == 0);
            result.Text = $"起始地址  0x{r.Start:X}\n末地址      0x{r.End:X}（包含）\n容量          {r.Bytes:N0} byte  /  {r.Kilobytes:F6} KB\n16 位字     {r.Words:N0} word  /  {r.Kwords:F6} Kword";
            notes.Text = r.Notes; copyText = result.Text + "\n" + r.Notes; _status.Text = "地址计算完成";
        });
        var actions = Row(input); ActionButton(actions, "计算空间", Calculate, true); ActionButton(actions, "复制结果", () => Copy(copyText ?? ""));
        void Invalidate() { result.Text = "参数已修改，请重新计算。"; copyText = null; notes.Text = ""; }
        direction.SelectionChanged += (_, _) => { end.IsEnabled = direction.SelectedIndex == 0; bytes.IsEnabled = direction.SelectedIndex == 1; Invalidate(); }; unit.SelectionChanged += (_, _) => Invalidate(); foreach (var t in new[] { start, end, bytes }) t.TextChanged += (_, _) => Invalidate();
        Calculate(); _smokeChecks.Add(Calculate); return Scroll(page);
    }
}
