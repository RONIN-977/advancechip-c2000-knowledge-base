using System.Globalization;
using System.Numerics;

namespace Fae.Core;

public record AdcResult(int Code, double PinVolts, double InputVolts, double CurrentAmps, string Formula);
public static class AdcCalculator
{
    public static AdcResult FromCode(int code, double reference = 3.3, double dividerRatio = 1, double shuntOhms = 0.01, double gain = 1, double offsetVolts = 0, int bits = 12)
    {
        Validate(reference, dividerRatio, shuntOhms, gain, offsetVolts, bits);
        var levels = 1 << bits;
        if (code < 0 || code >= levels) throw new ArgumentException($"{bits}位ADC原始码须为0～{levels - 1}。");
        var pin = code * reference / levels;
        return new(code, pin, pin * dividerRatio, (pin - offsetVolts) / (gain * shuntOhms),
            $"理想量化换算：Vpin=Code×Vref/2^{bits}；Vin=Vpin×分压倍率；I=(Vpin−偏置)/(增益×采样电阻)。不含ADC增益/偏移、参考电压及电阻误差；满量程输入饱和至{levels - 1}。");
    }

    public static AdcResult FromVoltage(double pinVolts, double reference = 3.3, double dividerRatio = 1, double shuntOhms = 0.01, double gain = 1, double offsetVolts = 0, int bits = 12)
    {
        Validate(reference, dividerRatio, shuntOhms, gain, offsetVolts, bits);
        if (!double.IsFinite(pinVolts) || pinVolts < 0 || pinVolts > reference) throw new ArgumentException("ADC引脚电压超出0～Vref有效范围。");
        var code = Math.Clamp((int)Math.Round(pinVolts / reference * (1 << bits), MidpointRounding.AwayFromZero), 0, (1 << bits) - 1);
        return FromCode(code, reference, dividerRatio, shuntOhms, gain, offsetVolts, bits);
    }

    private static void Validate(double reference, double divider, double resistance, double gain, double offset, int bits)
    {
        if (bits != 12) throw new ArgumentException("第一版四个型号仅提供12位、单端理想换算。");
        if (new[] { reference, divider, resistance, gain }.Any(x => !double.IsFinite(x) || x <= 0) || !double.IsFinite(offset))
            throw new ArgumentException("参考电压、分压倍率、采样电阻、增益须为正数，偏置须为有效数值。");
    }
}

public record AddressResult(BigInteger Start, BigInteger End, BigInteger Bytes, BigInteger Words, double Kilobytes, double Kwords, string Notes);
public static class AddressCalculator
{
    public static BigInteger Parse(string value)
    {
        var s = value.Trim().Replace("_", "");
        BigInteger result;
        if (s.StartsWith("0x", StringComparison.OrdinalIgnoreCase))
        {
            if (s.Length <= 2 || !s[2..].All(Uri.IsHexDigit)) throw new ArgumentException("十六进制格式不正确，请使用0x前缀。");
            result = BigInteger.Parse("0" + s[2..], NumberStyles.AllowHexSpecifier, CultureInfo.InvariantCulture);
        }
        else if (!BigInteger.TryParse(s, NumberStyles.Integer, CultureInfo.InvariantCulture, out result))
            throw new ArgumentException("请输入十进制整数或0x开头的十六进制数。");
        if (result < 0) throw new ArgumentException("地址和容量不能为负数。");
        return result;
    }

    public static AddressResult FromRange(string start, string end, bool wordAddressed = true)
    {
        var a = Parse(start); var b = Parse(end);
        if (a > b) throw new ArgumentException("结束地址不能小于起始地址。");
        return Build(a, b, wordAddressed);
    }

    public static AddressResult FromSize(string start, string bytes, bool wordAddressed = true)
    {
        var a = Parse(start); var size = Parse(bytes);
        if (size <= 0) throw new ArgumentException("容量必须大于0字节。");
        if (wordAddressed && !size.IsEven) throw new ArgumentException("16位字寻址要求容量为偶数字节，不能包含半个地址单元。");
        return Build(a, a + size / (wordAddressed ? 2 : 1) - 1, wordAddressed);
    }

    private static AddressResult Build(BigInteger a, BigInteger b, bool wordAddressed)
    {
        if (a > uint.MaxValue || b > uint.MaxValue) throw new ArgumentException("地址超出32位地址范围。");
        var bytes = (b - a + 1) * (wordAddressed ? 2 : 1);
        var notes = "区间包含首尾；1 KB=1024字节，1 Kword=1024个16位字。";
        if (!bytes.IsEven) notes += " 当前容量包含半个16位字；Words仅显示完整字数，精确字数为字节数/2。";
        return new(a, b, bytes, bytes / 2, (double)bytes / 1024, (double)bytes / 2048, notes);
    }
}
