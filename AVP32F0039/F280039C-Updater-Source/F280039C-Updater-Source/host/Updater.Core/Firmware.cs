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

public sealed record FirmwareImage(uint DeviceId, uint BoardId, ushort Cpu, uint StartWordAddress,
    uint EntryWordAddress, uint Version, ushort[] Words)
{
    public bool SimulationOnly { get; init; }
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
    public void Validate()
    {
        if (DeviceId != Profile.DeviceId || BoardId != Profile.BoardId || Cpu != Profile.Cpu)
            throw new InvalidDataException("固件型号、板卡或 CPU 与 F280039C / UPS-1 配置不匹配。");
        if (Words is null || Words.Length == 0 || Words.Length % 4 != 0 || StartWordAddress != Profile.AppStart ||
            (ulong)StartWordAddress + (ulong)Words.Length > Profile.AppEnd)
            throw new InvalidDataException("固件必须从应用区起点开始，长度为 4 个 16 位字的倍数，且不得越界。");
        if (EntryWordAddress != Profile.AppStart)
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
    public static FirmwareImage Load(string path)
    {
        if (!Path.GetExtension(path).Equals(".json", StringComparison.OrdinalIgnoreCase)) return TiBootTable.Load(path);
        if (new FileInfo(path).Length > 4_000_000) throw new InvalidDataException("固件文件过大。");
        var image = JsonSerializer.Deserialize<FirmwareImage>(File.ReadAllText(path)) ?? throw new InvalidDataException("固件为空。");
        image.Validate();
        return image;
    }
    public void Save(string path) => File.WriteAllText(path, JsonSerializer.Serialize(this, new JsonSerializerOptions { WriteIndented = true }));
    public static FirmwareImage Demo() => new(Profile.DeviceId, Profile.BoardId, 1, Profile.AppStart,
        Profile.AppStart, 1, Enumerable.Range(0, 4096).Select(i => (ushort)((i * 31) ^ 0xa55a)).ToArray()) { SimulationOnly = true };
}
