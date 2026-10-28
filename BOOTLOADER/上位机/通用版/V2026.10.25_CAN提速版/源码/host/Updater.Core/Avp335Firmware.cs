using System.Buffers.Binary;
using System.Security.Cryptography;
using System.Text;

namespace Updater.Core;

public static class Avp335Profile
{
    public const uint AppStart = 0x300000;
    public const uint AppEnd = 0x337ffe; // exclusive; the last two words are metadata
    public const uint FlagAddress = AppEnd;
    public const uint BootVersion = 201;
    public const int PacketBytes = 128;
    public const int MaxBytes = (int)((AppEnd - AppStart) * 2);
    public static uint ValidateStart(uint address)
    {
        if (address < AppStart || address >= AppEnd - 1 || (address & 3) != 0)
            throw new InvalidDataException("335 APP 起始地址须为 0x300000–0x337FFC 内的 4 字对齐地址（16 位字地址）。");
        return address;
    }
}

public sealed record Avp335FirmwareImage(byte[] Bytes)
{
    public bool SimulationOnly { get; init; }
    public uint StartWordAddress { get; init; } = Avp335Profile.AppStart;
    public byte[] Digest => MD5.HashData(Bytes);
    public void Validate()
    {
        Avp335Profile.ValidateStart(StartWordAddress);
        if (Bytes is null || Bytes.Length < 4 || (Bytes.Length & 1) != 0 || Bytes.Length > (Avp335Profile.AppEnd - StartWordAddress) * 2)
            throw new InvalidDataException("335 固件必须包含 codestart、长度为偶数，且不得超过 448 KB 减 4 字节。");
    }
    public static Avp335FirmwareImage Demo(uint start = Avp335Profile.AppStart) => new(Enumerable.Range(0, (int)Math.Min(4100U, (Avp335Profile.AppEnd - Avp335Profile.ValidateStart(start))*2)).Select(i => (byte)(i * 31)).ToArray()) { SimulationOnly = true, StartWordAddress = start };
}

/// <summary>Reads the C28x COFF v2 output used by the supplied 335 CCS project. No converter process is needed.</summary>
public static class Avp335OutLoader
{
    public static Avp335FirmwareImage Load(string path, uint start = Avp335Profile.AppStart)
    {
        if (!Path.GetExtension(path).Equals(".out", StringComparison.OrdinalIgnoreCase))
            throw new InvalidDataException("335 请直接选择配套 APP 工程编译的 .out 文件。");
        var file = new FileInfo(path);
        if (!file.Exists) throw new FileNotFoundException("未找到 335 OUT 文件。", path);
        if (file.Length == 0 || file.Length > 128L * 1024 * 1024) throw new InvalidDataException("OUT 文件为空或过大。");
        return Parse(File.ReadAllBytes(path), start);
    }
    public static Avp335FirmwareImage Parse(byte[] file, uint start = Avp335Profile.AppStart)
    {
        Avp335Profile.ValidateStart(start);
        void Range(long offset, long length)
        {
            if (offset < 0 || length < 0 || offset > file.Length || length > file.Length - offset)
                throw new InvalidDataException("335 COFF 文件被截断或字段越界。");
        }
        ushort U16(int offset) { Range(offset, 2); return BinaryPrimitives.ReadUInt16LittleEndian(file.AsSpan(offset)); }
        uint U32(int offset) { Range(offset, 4); return BinaryPrimitives.ReadUInt32LittleEndian(file.AsSpan(offset)); }
        Range(0, 22);
        if (U16(0) != 0x00c2 || U16(20) != 0x009d || (U16(18) & 2) == 0)
            throw new InvalidDataException("需要 335 CCS 工程生成的 C28x COFF v2 可执行 OUT；不接受 ELF、HEX 或其他架构。");
        int count = U16(2), optional = U16(16);
        if (optional < 28) throw new InvalidDataException("COFF 可执行文件缺少可选头。");
        Range(22, optional); Range(22L + optional, count * 48L);
        long strings = U32(8) + U32(12) * 18L;
        string Name(int header)
        {
            int offset = header, limit = header + 8;
            if (U32(header) == 0)
            {
                Range(strings, 4);
                uint size = U32(checked((int)strings)), name = U32(header + 4);
                if (size < 4 || name < 4 || name >= size) throw new InvalidDataException("COFF 段名字符串表无效。");
                Range(strings, size); offset = checked((int)(strings + name)); limit = checked((int)(strings + size));
            }
            int end = offset;
            while (end < limit && file[end] != 0) end++;
            if (end == limit && limit != header + 8) throw new InvalidDataException("COFF 段名未结束。");
            return Encoding.ASCII.GetString(file, offset, end - offset);
        }
        byte[] image = Enumerable.Repeat((byte)0xff, checked((int)((Avp335Profile.AppEnd-start)*2))).ToArray();
        bool[] seen = new bool[image.Length / 2];
        bool codestart = false, flag = false;
        int last = 0;
        for (int i = 0; i < count; i++)
        {
            int header = 22 + optional + i * 48;
            uint words = U32(header + 16), raw = U32(header + 20), attributes = U32(header + 40);
            // DSECT, NOLOAD, COPY/debug, and BSS are not loadable Flash contents.
            if (words == 0 || (attributes & 0x93) != 0 || (attributes & 0x60) == 0) continue;
            if (raw == 0) throw new InvalidDataException("335 COFF 初始化段缺少装载数据。");
            string name = Name(header);
            // C28x COFF uses s_vaddr for LOAD and s_paddr for RUN; ramfuncs may have different addresses.
            uint address = U32(header + 12);
            Range(raw, words * 2L);
            if (name == "BOOTFLAG")
            {
                if (flag || address != Avp335Profile.FlagAddress || words != 1 || U16(checked((int)raw)) != 0x00aa)
                    throw new InvalidDataException("335 有效标志布局不匹配，请使用 V201 / 448 KB APP 链接文件重新编译。");
                flag = true; continue;
            }
            if (address < start || address >= Avp335Profile.AppEnd || words > Avp335Profile.AppEnd - address)
                throw new InvalidDataException($"段 {name} 的装载地址 0x{address:X6} 超出 335 APP 区；拒绝 BOOT、CSM、RAM、OTP 数据。");
            int offset = checked((int)(address - start));
            for (int word = 0; word < words; word++)
            {
                if (seen[offset + word]) throw new InvalidDataException("335 OUT 包含重叠装载段。");
                seen[offset + word] = true;
            }
            Array.Copy(file, raw, image, offset * 2L, words * 2L);
            last = Math.Max(last, checked((int)((offset + words) * 2)));
            if (name == "codestart" && address == start && words >= 2) codestart = true;
        }
        if (!codestart || !flag || !seen[0] || !seen[1])
            throw new InvalidDataException($"OUT 缺少 0x{start:X6} codestart 或 V201 BOOTFLAG；请按所选分区重新链接 APP。");
        Array.Resize(ref image, last);
        var result = new Avp335FirmwareImage(image) { StartWordAddress = start }; result.Validate(); return result;
    }
}
