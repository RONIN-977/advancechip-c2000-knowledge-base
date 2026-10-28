using System.Globalization;
using System.Text.RegularExpressions;

namespace Updater.Core;

public static class TiBootTable
{
    // hex2000 --boot --sci8 --ascii --outfile=application.txt application.out
    // 16-bit words are little endian on the SCI wire; 32-bit addresses consist
    // of the HIGH 16-bit word followed by the LOW 16-bit word (TI boot format).
    public static FirmwareImage Load(string path, uint version = 1, DeviceProfile? target = null)
    {
        if (new FileInfo(path).Length > 2_000_000) throw new InvalidDataException("TI SCI8 文件过大。");
        return Parse(File.ReadAllText(path), version, target);
    }
    public static FirmwareImage Parse(string text, uint version = 1, DeviceProfile? target = null)
    {
        target ??= DeviceProfile.F280039C;
        if (DeviceProfile.For(target.DeviceId).WithAppStart(target.AppStart) != target)
            throw new InvalidDataException("应用分区参数不合法。");
        text = text.Trim();
        if (text.StartsWith('\u0002')) text = text[1..];
        text = text.Trim();
        if (text.EndsWith('\u0003')) text = text[..^1];
        var tokens = Regex.Split(text.Trim(), @"\s+");
        if (tokens.Length > 400000) throw new InvalidDataException("TI boot table 过大。");
        byte[] bytes = new byte[tokens.Length];
        for (int i = 0; i < tokens.Length; i++)
            if (tokens[i].Length != 2 || !byte.TryParse(tokens[i], NumberStyles.AllowHexSpecifier, CultureInfo.InvariantCulture, out bytes[i]))
                throw new InvalidDataException("需要 hex2000 导出的 SCI8 ASCII boot table（不是 Intel HEX 或原始 .out）。");
        int position = 0;
        ushort Word()
        {
            if (position + 2 > bytes.Length) throw new InvalidDataException("TI boot table 被截断。");
            ushort value = (ushort)(bytes[position] | (bytes[position + 1] << 8)); position += 2; return value;
        }
        uint Address() { uint high = Word(); return (high << 16) | Word(); }
        if (Word() != 0x08aa) throw new InvalidDataException("SCI8 Key 必须为 0x08AA。");
        for (int i = 0; i < 8; i++) _ = Word();
        uint entry = Address();
        if (entry != target.AppStart) throw new InvalidDataException($"入口 0x{entry:X6} 不匹配应用入口 0x{target.AppStart:X6}；请先修改应用链接文件重新编译。");
        ushort[] words = Enumerable.Repeat((ushort)0xffff, checked((int)(target.AppEnd - target.AppStart))).ToArray();
        bool[] seen = new bool[words.Length];
        int last = 0;
        for (;;)
        {
            ushort count = Word();
            if (count == 0) break;
            uint address = Address();
            if (address < target.AppStart || address >= target.AppEnd || count > target.AppEnd - address)
                throw new InvalidDataException($"段地址 0x{address:X6} / {count} 字超出应用分区；拒绝写入 Bootloader、RAM、OTP 或元数据。");
            int offset = checked((int)(address - target.AppStart));
            for (int i = 0; i < count; i++)
            {
                if (seen[offset + i]) throw new InvalidDataException("TI boot table 包含重叠地址。");
                words[offset + i] = Word(); seen[offset + i] = true;
            }
            last = Math.Max(last, offset + count);
        }
        if (position != bytes.Length || last == 0 || !seen[0] || !seen[1]) throw new InvalidDataException("TI boot table 结束标记、内容或 codestart 不完整。");
        // Merge all sections before padding. Never program an ECC granule twice.
        Array.Resize(ref words, (last + 3) & ~3);
        var result = new FirmwareImage(target.DeviceId, target.BoardId, target.Cpu, target.AppStart, entry, version, words) { ConfiguredTarget = target };
        result.Validate();
        return result;
    }
}
