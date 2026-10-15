using System.Buffers.Binary;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace Updater.Core;

// Provisional CPU1 partition, C28x 16-bit WORD addresses. Change with the board profile and linker together.
public static class Profile
{
    public const uint DeviceId = 0x280039c1;
    public const uint BoardId = 0x55505301; // Project board profile UPS revision 1.
    public const uint AppStart = 0x090000;
    public const uint AppEnd = 0x0af000; // exclusive; bank 1 and bank 2 sectors 0-14
    public const ushort Cpu = 1;
}

public sealed record DeviceProfile(string Name, uint DeviceId, uint BoardId, ushort Cpu, uint AppStart, uint AppEnd)
{
    public DeviceProfile WithAppStart(uint address)
    {
        uint minimum = DeviceId == 0x28003501 ? 0x3e8000U : 0x080000U;
        if (address < minimum || address >= AppEnd || (address & 3) != 0)
            throw new InvalidDataException($"APP 起始地址须为 0x{minimum:X6}–0x{AppEnd - 4:X6} 内的 4 字对齐地址（16 位字地址）。");
        return this with { AppStart = address };
    }
    public static readonly DeviceProfile F280039C = new("F280039C", Profile.DeviceId, Profile.BoardId, Profile.Cpu, Profile.AppStart, Profile.AppEnd);
    public static readonly DeviceProfile F280049 = new("F280049 / F280049C", 0x28004901, 0x55505301, 1, 0x090000, 0x09fff0);
    public static readonly DeviceProfile F280035 = new("TMS320F28035", 0x28003501, 0x55505301, 1, 0x3e8000, 0x3f2000);
    public static readonly DeviceProfile F2800157 = new("AVP32F00157", 0x28015701, 0x55505301, 1, 0x088000, 0x09fff0);
    public static DeviceProfile For(uint deviceId) => deviceId switch
    {
        Profile.DeviceId => F280039C,
        0x28004901 => F280049,
        0x28003501 => F280035,
        0x28015701 => F2800157,
        _ => throw new InvalidDataException("不支持的固件型号。")
    };
}

public sealed record FirmwareImage(uint DeviceId, uint BoardId, ushort Cpu, uint StartWordAddress,
    uint EntryWordAddress, uint Version, ushort[] Words)
{
    public bool SimulationOnly { get; init; }
    [JsonIgnore]
    public DeviceProfile? ConfiguredTarget { get; init; }
    [JsonIgnore]
    public byte[] Bytes
    {
        get
        {
            byte[] data = new byte[checked(Words.Length * 2)];
            for (int i = 0; i < Words.Length; i++) BinaryPrimitives.WriteUInt16LittleEndian(data.AsSpan(2 * i), Words[i]);
            return data;
        }
    }
    [JsonIgnore]
    public DeviceProfile Target => ConfiguredTarget ?? DeviceProfile.For(DeviceId);
    public void Validate(DeviceProfile? expected = null)
    {
        var target = Target;
        var standard = DeviceProfile.For(DeviceId);
        if (standard.WithAppStart(target.AppStart) != target)
            throw new InvalidDataException("自定义分区只能修改 APP 起始地址，型号、板号及分区结束地址必须保持一致。");
        if ((expected is not null && target != expected) || BoardId != target.BoardId || Cpu != target.Cpu)
            throw new InvalidDataException("固件型号、板卡或 CPU 与所选配置不匹配。");
        if (Words is null || Words.Length == 0 || Words.Length % 4 != 0 || StartWordAddress != target.AppStart ||
            (ulong)StartWordAddress + (ulong)Words.Length > target.AppEnd)
            throw new InvalidDataException("固件必须从应用区起点开始，长度为 4 个 16 位字的倍数，且不得越界。");
        if (EntryWordAddress != target.AppStart)
            throw new InvalidDataException("入口必须是应用区起点的 codestart（跳向 C 运行库初始化）。");
    }
    public byte[] Manifest()
    {
        Validate();
        byte[] data = new byte[32];
        uint[] fields = [DeviceId, BoardId, Cpu, StartWordAddress, (uint)Words.Length, EntryWordAddress, Crc32.Compute(Bytes), Version];
        for (int i = 0; i < fields.Length; i++) BinaryPrimitives.WriteUInt32LittleEndian(data.AsSpan(4 * i), fields[i]);
        return data;
    }
    public static FirmwareImage Load(string path, DeviceProfile? target = null)
    {
        if (!Path.GetExtension(path).Equals(".json", StringComparison.OrdinalIgnoreCase)) return TiBootTable.Load(path, target: target);
        if (new FileInfo(path).Length > 4_000_000) throw new InvalidDataException("固件文件过大。");
        var image = JsonSerializer.Deserialize<FirmwareImage>(File.ReadAllText(path)) ?? throw new InvalidDataException("固件为空。");
        image = image with { ConfiguredTarget = target };
        image.Validate(target);
        return image;
    }
    public void Save(string path) => File.WriteAllText(path, JsonSerializer.Serialize(this, new JsonSerializerOptions { WriteIndented = true }));
    public static FirmwareImage Demo(DeviceProfile? target = null)
    {
        target ??= DeviceProfile.F280039C;
        return new(target.DeviceId, target.BoardId, target.Cpu, target.AppStart,
            target.AppStart, 1, Enumerable.Range(0, (int)Math.Min(4096U, target.AppEnd-target.AppStart)).Select(i => (ushort)((i * 31) ^ 0xa55a)).ToArray()) { SimulationOnly = true, ConfiguredTarget = target };
    }
}
