using System.Numerics;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using Fae.Core;

namespace Fae.Desktop;
public partial class MainWindow
{
    private UIElement ProgrammerPage()
    {
        var page = Page(5); var card = Card(page); var controls = Row(card); var width = Choice(controls, "字长", ["64 位 · QWORD", "32 位 · DWORD", "16 位 · WORD", "8 位 · BYTE"], 1, 210); var radix = Choice(controls, "输入进制", ["HEX · 十六进制", "DEC · 十进制", "OCT · 八进制", "BIN · 二进制"], 1, 220); var signed = new CheckBox { Content = "有符号（DEC 与算术右移）" }; controls.Children.Add(signed);
        var editor = new Grid { Margin = new Thickness(0, 3, 0, 0) }; editor.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(1, GridUnitType.Star) }); editor.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(1.2, GridUnitType.Star) }); card.Children.Add(editor);
        var display = new StackPanel { Margin = new Thickness(0, 0, 18, 0) }; editor.Children.Add(display);
        var expression = new TextBox { Text = "0", FontFamily = new System.Windows.Media.FontFamily("Cascadia Mono, Consolas"), FontSize = 29, MinHeight = 65, Margin = new Thickness(0, 4, 0, 12), HorizontalContentAlignment = HorizontalAlignment.Right, AcceptsReturn = false };
        display.Children.Add(expression);
        var repr = new Grid(); repr.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(60) }); repr.ColumnDefinitions.Add(new ColumnDefinition()); var reprBoxes = new List<TextBox>(); int[] bases = [16, 10, 8, 2];
        for (int i = 0; i < bases.Length; i++)
        {
            repr.RowDefinitions.Add(new RowDefinition()); var label = new TextBlock { Text = new[] { "HEX", "DEC", "OCT", "BIN" }[i], Foreground = Brush("#64748B"), FontWeight = FontWeights.SemiBold, VerticalAlignment = VerticalAlignment.Center };
            Grid.SetRow(label, i); repr.Children.Add(label); var text = new TextBox { IsReadOnly = true, FontFamily = new System.Windows.Media.FontFamily("Cascadia Mono, Consolas"), Background = System.Windows.Media.Brushes.Transparent, BorderThickness = new Thickness(0), Padding = new Thickness(2, 5, 2, 5), Margin = new Thickness(0), MinHeight = 28, TextWrapping = TextWrapping.Wrap }; Grid.SetRow(text, i); Grid.SetColumn(text, 1); repr.Children.Add(text); reprBoxes.Add(text);
        }
        display.Children.Add(repr); var resultNote = Note(display, "整数定宽运算；结果按当前位宽截断。有符号只影响十进制解释、除法和右移。"); resultNote.FontSize = 12; resultNote.LineHeight = 20;
        var keyGrid = new Grid { Margin = new Thickness(0, 4, 0, 0), VerticalAlignment = VerticalAlignment.Top }; for (int i = 0; i < 6; i++) keyGrid.ColumnDefinitions.Add(new ColumnDefinition());
        string[][] keys = [["A", "B", "C", "(", ")", "⌫"], ["D", "E", "F", "AND", "OR", "XOR"], ["7", "8", "9", "NOT", "<<", ">>"], ["4", "5", "6", "×", "÷", "%"], ["1", "2", "3", "+", "−", "C"], ["0", "00", "±", "复制", "CE", "="]];
        var digitButtons = new List<(Button Button, int Value)>();
        var bitsPanel = new WrapPanel { Margin = new Thickness(0, 3, 0, 0) }; var bitCard = Card(page, "位视图 · 点击切换 0 / 1"); bitCard.Children.Add(bitsPanel);
        BigInteger current = 0; bool evaluated = false; bool changing = false;
        int Bits() => new[] { 64, 32, 16, 8 }[Math.Max(0, width.SelectedIndex)]; int Radix() => bases[Math.Max(0, radix.SelectedIndex)];
        void UpdateRepresentations()
        {
            for (int i = 0; i < bases.Length; i++) reprBoxes[i].Text = ProgrammerCalculator.Format(current, bases[i], Bits(), signed.IsChecked == true);
            bitsPanel.Children.Clear(); BigInteger masked = current & ((BigInteger.One << Bits()) - 1);
            for (int bit = Bits() - 1; bit >= 0; bit--)
            {
                int index = bit; bool one = (masked & (BigInteger.One << bit)) != 0;
                var panel = new StackPanel(); panel.Children.Add(new TextBlock { Text = bit.ToString(), FontSize = 10, HorizontalAlignment = HorizontalAlignment.Center, Foreground = Brush("#7E8BA0") }); panel.Children.Add(new TextBlock { Text = one ? "1" : "0", FontSize = 16, HorizontalAlignment = HorizontalAlignment.Center, FontFamily = new System.Windows.Media.FontFamily("Consolas") });
                var b = new Button { Content = panel, Width = 37, Height = 53, Padding = new Thickness(1), Margin = new Thickness(0, 1, bit % 8 == 0 ? 10 : 3, 5), Background = one ? Brush("#DCE9FF") : Brush("#F2F5FA"), Foreground = one ? Brush("#1D4ED8") : Brush("#7C8AA2") };
                b.Click += (_, _) => Guard(() => { if (!TryUpdate()) throw new ArgumentException("表达式尚未完成，请补全后切换位。"); current = ProgrammerCalculator.ToggleBit(current, index, Bits()); SetExpression(ProgrammerCalculator.Format(current, Radix(), Bits(), signed.IsChecked == true)); evaluated = true; UpdateRepresentations(); }); bitsPanel.Children.Add(b);
            }
        }
        void SetExpression(string text) { changing = true; expression.Text = text; expression.CaretIndex = expression.Text.Length; changing = false; }
        bool TryUpdate()
        {
            try
            {
                current = ProgrammerCalculator.Evaluate(expression.Text, Bits(), signed.IsChecked == true, Radix()); UpdateRepresentations();
                resultNote.Text = $"{Bits()} 位 · {(signed.IsChecked == true ? "有符号" : "无符号")} · 四种进制与位视图同步显示；有符号右移补符号位，其他右移补0。"; return true;
            }
            catch (Exception ex) { resultNote.Text = "表达式尚未完成或无效：" + ex.Message; return false; }
        }
        void Evaluate()
        {
            Guard(() => { current = ProgrammerCalculator.Evaluate(expression.Text, Bits(), signed.IsChecked == true, Radix()); UpdateRepresentations(); SetExpression(ProgrammerCalculator.Format(current, Radix(), Bits(), signed.IsChecked == true)); evaluated = true; resultNote.Text = $"{Bits()} 位 · {(signed.IsChecked == true ? "有符号" : "无符号")} · 超出位宽保留低位；有符号右移补符号位，其他右移补0；移位量大于等于位宽时全移出。"; _status.Text = "程序员整数计算完成"; });
        }
        void Insert(string text)
        {
            expression.Focus(); bool digit = text.Length > 0 && text.All(char.IsAsciiLetterOrDigit);
            if ((evaluated || expression.Text == "0") && digit) SetExpression(""); evaluated = false;
            int caret = expression.SelectionStart; expression.SelectedText = text; expression.CaretIndex = caret + text.Length;
        }
        void Press(string key, bool clear = false)
        {
            if (key == "=") { Evaluate(); return; }
            if (clear || key == "CE") { SetExpression("0"); current = 0; evaluated = false; UpdateRepresentations(); return; }
            if (key == "复制") { if (TryUpdate()) Copy(ProgrammerCalculator.Format(current, Radix(), Bits(), signed.IsChecked == true)); else _status.Text = "表达式未完成，未复制旧结果。"; return; }
            if (key == "⌫") { int p = expression.SelectionStart; if (expression.SelectionLength > 0) expression.SelectedText = ""; else if (p > 0) { expression.Text = expression.Text.Remove(p - 1, 1); expression.CaretIndex = p - 1; } if (expression.Text.Length == 0) SetExpression("0"); evaluated = false; return; }
            if (key == "±") { SetExpression("-(" + expression.Text + ")"); Evaluate(); return; }
            Insert(key switch { "AND" => " & ", "OR" => " | ", "XOR" => " ^ ", "NOT" => "~", "×" => " * ", "÷" => " / ", "−" => " - ", "+" => " + ", "<<" => " << ", ">>" => " >> ", "%" => " % ", _ => key });
        }
        for (int r = 0; r < keys.Length; r++)
        {
            keyGrid.RowDefinitions.Add(new RowDefinition()); for (int c = 0; c < 6; c++)
            {
                string key = keys[r][c]; bool clear = r == 4 && c == 5;
                var b = new Button { Content = key, FontSize = 18, Height = 43, Padding = new Thickness(5), Margin = new Thickness(0, 0, 5, 5), Background = key == "=" ? Brush("#2563EB") : Brush("#EEF3FA"), Foreground = key == "=" ? System.Windows.Media.Brushes.White : Brush("#24364F") };
                b.Click += (_, _) => Press(key, clear); Grid.SetRow(b, r); Grid.SetColumn(b, c); keyGrid.Children.Add(b);
                if (r <= 1 && c <= 2 || key.All(char.IsAsciiDigit)) { int n = key.Length == 1 && key[0] >= 'A' ? key[0] - 'A' + 10 : int.Parse(key); digitButtons.Add((b, n)); }
            }
        }
        Grid.SetColumn(keyGrid, 1); editor.Children.Add(keyGrid);
        void ModeChanged()
        {
            Guard(() => { current = ProgrammerCalculator.Evaluate(current.ToString(System.Globalization.CultureInfo.InvariantCulture), Bits(), signed.IsChecked == true, 10); SetExpression(ProgrammerCalculator.Format(current, Radix(), Bits(), signed.IsChecked == true)); UpdateRepresentations(); foreach (var b in digitButtons) b.Button.IsEnabled = b.Value < Radix(); evaluated = true; });
        }
        width.SelectionChanged += (_, _) => ModeChanged(); radix.SelectionChanged += (_, _) => ModeChanged(); signed.Checked += (_, _) => ModeChanged(); signed.Unchecked += (_, _) => ModeChanged();
        expression.TextChanged += (_, _) => { if (!changing) { evaluated = false; TryUpdate(); } };
        expression.PreviewKeyDown += (_, e) => { if (e.Key == Key.Enter) { Evaluate(); e.Handled = true; } else if (e.Key == Key.Escape) { Press("CE"); e.Handled = true; } };
        ModeChanged();
        _smokeChecks.Add(() =>
        {
            radix.SelectedIndex = 1; SetExpression("(15 << 4) | 3"); Evaluate(); if (current != 243) throw new InvalidOperationException("计算器表达式自检失败。");
            radix.SelectedIndex = 0; if (reprBoxes[0].Text.TrimStart('0') != "F3") throw new InvalidOperationException("计算器HEX自检失败。");
            SetExpression("FF"); if (!TryUpdate() || current != 255) throw new InvalidOperationException("计算器输入FF即时解析失败。"); radix.SelectedIndex = 1; if (expression.Text != "255") throw new InvalidOperationException("计算器HEX切DEC失败。");
            width.SelectedIndex = 3; signed.IsChecked = true; if (reprBoxes[1].Text != "-1") throw new InvalidOperationException("计算器符号切换失败。"); signed.IsChecked = false;
            current = ProgrammerCalculator.ToggleBit(current, 0, Bits()); UpdateRepresentations(); if ((current & 1) != 0) throw new InvalidOperationException("计算器位切换自检失败。");
            width.SelectedIndex = 1; radix.SelectedIndex = 1; SetExpression("0"); Evaluate();
        });
        return Scroll(page);
    }
}
