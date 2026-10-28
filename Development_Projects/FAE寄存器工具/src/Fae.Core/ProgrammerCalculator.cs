using System.Globalization;
using System.Numerics;

namespace Fae.Core;

/// <summary>定宽整数表达式。溢出按位宽截断；signed控制除法、比较解释及右移补符号。</summary>
public static class ProgrammerCalculator
{
    public static BigInteger Evaluate(string expression, int bits = 32, bool signed = false, int radix = 10)
    {
        Check(bits, radix);
        return new Parser(expression, bits, signed, radix).Parse();
    }

    public static BigInteger ToggleBit(BigInteger value, int bit, int bits = 32)
    {
        Check(bits, 10);
        if (bit < 0 || bit >= bits) throw new ArgumentException("位号超出当前位宽。");
        return Normalize(value ^ (BigInteger.One << bit), bits);
    }

    public static string Format(BigInteger value, int radix, int bits = 32, bool signed = false)
    {
        Check(bits, radix);
        value = Normalize(value, bits);
        if (radix == 10) return (signed ? AsSigned(value, bits) : value).ToString(CultureInfo.InvariantCulture);
        if (value.IsZero) return "0";
        var result = "";
        while (value > 0)
        {
            value = BigInteger.DivRem(value, radix, out var digit);
            result = "0123456789ABCDEF"[(int)digit] + result;
        }
        return result;
    }

    public static BigInteger Normalize(BigInteger value, int bits) => value & ((BigInteger.One << bits) - 1);
    public static BigInteger AsSigned(BigInteger value, int bits)
    {
        value = Normalize(value, bits);
        return value.TestBit(bits - 1) ? value - (BigInteger.One << bits) : value;
    }
    public static bool TestBit(this BigInteger value, int bit) => (value & (BigInteger.One << bit)) != 0;
    private static void Check(int bits, int radix)
    {
        if (bits is not (8 or 16 or 32 or 64)) throw new ArgumentException("位宽须为8、16、32或64。");
        if (radix is not (2 or 8 or 10 or 16)) throw new ArgumentException("进制须为2、8、10或16。");
    }

    private sealed class Parser(string source, int bits, bool signed, int radix)
    {
        private readonly string text = source.Replace("×", "*").Replace("÷", "/").Replace("−", "-");
        private int pos;
        public BigInteger Parse()
        {
            if (string.IsNullOrWhiteSpace(text) || text.Length > 4096) throw new ArgumentException("请输入不超过4096字符的整数表达式。");
            var value = Expression(0);
            Space();
            if (pos != text.Length) throw new ArgumentException($"无法识别第{pos + 1}个字符附近的表达式。");
            return Normalize(value, bits);
        }
        private void Space() { while (pos < text.Length && char.IsWhiteSpace(text[pos])) pos++; }
        private BigInteger Expression(int minPriority)
        {
            var left = Unary();
            while (true)
            {
                Space();
                var (op, priority) = Operator();
                if (priority < minPriority) break;
                pos += op.Length;
                var right = Expression(priority + 1);
                left = Apply(op, left, right);
            }
            return left;
        }
        private (string, int) Operator()
        {
            if (pos >= text.Length) return ("", -1);
            if (text.AsSpan(pos).StartsWith("<<")) return ("<<", 4);
            if (text.AsSpan(pos).StartsWith(">>")) return (">>", 4);
            return text[pos] switch { '|' => ("|", 1), '^' => ("^", 2), '&' => ("&", 3), '+' => ("+", 5), '-' => ("-", 5), '*' => ("*", 6), '/' => ("/", 6), '%' => ("%", 6), _ => ("", -1) };
        }
        private BigInteger Unary()
        {
            Space();
            if (pos >= text.Length) throw new ArgumentException("表达式缺少操作数。");
            if (text[pos] is '+' or '-' or '~')
            {
                var op = text[pos++]; var value = Unary();
                return Normalize(op == '-' ? -value : op == '~' ? ~value : value, bits);
            }
            if (text[pos] == '(')
            {
                pos++; var value = Expression(0); Space();
                if (pos >= text.Length || text[pos++] != ')') throw new ArgumentException("括号不匹配。");
                return value;
            }
            var numberBase = radix;
            if (pos + 1 < text.Length && text[pos] == '0')
            {
                var prefix = char.ToLowerInvariant(text[pos + 1]);
                if (prefix is 'x' or 'b' or 'o') { numberBase = prefix == 'x' ? 16 : prefix == 'b' ? 2 : 8; pos += 2; }
            }
            var start = pos;
            var result = BigInteger.Zero;
            while (pos < text.Length)
            {
                var c = char.ToUpperInvariant(text[pos]);
                if (c == '_') { pos++; continue; }
                var digit = "0123456789ABCDEF".IndexOf(c);
                if (digit < 0) break;
                if (digit >= numberBase) throw new ArgumentException($"字符{c}不属于{numberBase}进制。");
                result = result * numberBase + digit;
                pos++;
            }
            if (pos == start || text[start..pos].All(x => x == '_')) throw new ArgumentException("表达式中存在无效数字或操作符。");
            return Normalize(result, bits);
        }
        private BigInteger Apply(string op, BigInteger left, BigInteger right)
        {
            var a = signed ? AsSigned(left, bits) : left;
            var b = signed ? AsSigned(right, bits) : right;
            if ((op is "/" or "%") && b.IsZero) throw new ArgumentException("不能除以0。");
            if (op is "<<" or ">>")
            {
                if (signed && b < 0) throw new ArgumentException("移位数量不能为负数。");
                if (right >= bits) return op == ">>" && signed && a < 0 ? Normalize(-1, bits) : BigInteger.Zero;
                return Normalize(op == "<<" ? left << (int)right : a >> (int)right, bits);
            }
            return Normalize(op switch
            {
                "+" => a + b, "-" => a - b, "*" => a * b, "/" => a / b, "%" => a % b,
                "&" => left & right, "|" => left | right, "^" => left ^ right,
                _ => throw new ArgumentException("不支持的运算符。")
            }, bits);
        }
    }
}
