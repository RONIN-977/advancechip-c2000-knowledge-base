namespace Fae.Core;

public record Chip(string Id, string Name, string TiModel, string CanKind, double MaxClockMhz, bool DriverLib);
public static class Chips
{
    public static readonly Chip[] All = [new("035", "ADP32F035", "TMS320F28035", "eCAN", 60, false), new("335", "AVP32F335", "TMS320F28335", "eCAN", 150, false), new("0039", "AVP32F0039", "TMS320F280039C", "DCAN", 120, true), new("0049", "AVP32F0049", "TMS320F280049C", "DCAN", 100, true)];
    public static Chip Get(string id) => All.First(x => x.Id == id);
}
public record CanInput(string ChipId, double CrystalMhz, double SysClockMhz, double CanClockMhz, double TargetKbps, double SamplePointPercent = 80, int Sjw = 1);
public record CanTiming(int Prescaler, int Tseg1, int Tseg2, int Sjw, double ActualKbps, double ErrorPercent, double SamplePointPercent)
{
    public int TotalTq => 1 + Tseg1 + Tseg2;
}
public record PwmInput(string ChipId, double SysClockMhz, int PeripheralDivider, int HighSpeedDivider, int ClockDivider, string Mode, double TargetHz, int? Period = null);
public record PwmResult(int Period, double TbClockHz, double ActualHz, double ErrorPercent);
public record RegisterField(string Name, int Lsb, int Width, string Access, string Reset, string Description, string Values, string Notes);
public record RegisterEntry(string ChipId, string Peripheral, string Name, string Address, int Width, string Description, string Keywords, string Source, string Notes, List<RegisterField> Fields);
public record RegisterPack(string Version, string GeneratedOn, List<RegisterEntry> Registers);
