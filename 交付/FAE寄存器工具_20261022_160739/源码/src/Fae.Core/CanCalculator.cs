using System.Globalization;

namespace Fae.Core;

public static class CanCalculator
{
    public static IReadOnlyList<CanTiming> Calculate(CanInput input)
    {
        Validate(input);
        var ecan = Chips.Get(input.ChipId).CanKind == "eCAN";
        var maxPrescaler = ecan ? 256 : 1024;
        var minPrescaler = ecan ? 2 : 1;
        var candidates = new List<CanTiming>();
        for (var t1 = 2; t1 <= 16; t1++)
        for (var t2 = 1; t2 <= 8; t2++)
        {
            var tq = 1 + t1 + t2;
            if (tq < 8 || input.Sjw > t2 || input.Sjw > t1 || (ecan && (t2 < 2 || t1 < t2))) continue;
            var ideal = input.CanClockMhz * 1000 / (input.TargetKbps * tq);
            // For fixed TSEG1/2 only the neighboring integer prescalers can win.
            foreach (var p in new[] { (int)Math.Clamp(Math.Floor(ideal), minPrescaler, maxPrescaler), (int)Math.Clamp(Math.Ceiling(ideal), minPrescaler, maxPrescaler) }.Distinct())
            {
                var actual = input.CanClockMhz * 1000 / (p * tq);
                candidates.Add(new(p, t1, t2, input.Sjw, actual, (actual / input.TargetKbps - 1) * 100, 100.0 * (1 + t1) / tq));
            }
        }
        return candidates.OrderBy(x => Math.Round(Math.Abs(x.ErrorPercent), 9))
            .ThenBy(x => Math.Abs(x.SamplePointPercent - input.SamplePointPercent))
            .ThenByDescending(x => x.TotalTq).ThenBy(x => x.Prescaler).Take(40).ToArray();
    }

    public static void Validate(CanInput i)
    {
        var chip = Chips.Get(i.ChipId);
        if (!double.IsFinite(i.CrystalMhz) || i.CrystalMhz <= 0 || !double.IsFinite(i.SysClockMhz) || i.SysClockMhz <= 0 || i.SysClockMhz > chip.MaxClockMhz)
            throw new ArgumentException($"晶振和主频须为正数；当前型号采用TI基线，主频上限为 {chip.MaxClockMhz} MHz。");
        if (!double.IsFinite(i.CanClockMhz) || i.CanClockMhz <= 0 || !double.IsFinite(i.TargetKbps) || i.TargetKbps <= 0 || i.TargetKbps > 1000)
            throw new ArgumentException("经典 CAN 目标波特率须在 0～1000 kbit/s（不含0），CAN时钟须为正数。");
        if (!double.IsFinite(i.SamplePointPercent) || i.SamplePointPercent <= 0 || i.SamplePointPercent >= 100 || i.Sjw < 1 || i.Sjw > 4)
            throw new ArgumentException("采样点须在0～100%之间，SJW须为1～4 TQ。");
        bool Same(double a, double b) => Math.Abs(a - b) < 0.000001;
        if (chip.CanKind == "eCAN")
        {
            // TI PLL tables: SPRUI10A p67/69; SPRUI07 p67/72.
            // Check algebraic reachability only; this does not configure or qualify the PLL.
            bool reachable = new[] { 1, 2, 4 }.Any(d => Same(i.SysClockMhz, i.CrystalMhz / d));
            foreach (int divider in chip.Id == "335" ? new[] { 2, 4 } : new[] { 1, 2, 4 })
                for (int multiplier = 1; multiplier <= (chip.Id == "335" ? 10 : 12); multiplier++)
                    if (Same(i.SysClockMhz, i.CrystalMhz * multiplier / divider) &&
                        (chip.Id != "035" || i.CrystalMhz * multiplier >= 50)) reachable = true;
            if (!reachable)
                throw new ArgumentException("所填晶振无法通过该TI基线PLL倍频/分频关系得到所填主频。335的150 MHz常用30 MHz晶振；请核对实际工程时钟，工具不修改PLL。");
        }
        if (chip.CanKind == "eCAN" && !Same(i.CanClockMhz, i.SysClockMhz / 2))
            throw new ArgumentException("035/335 的 eCAN 模块时钟必须为 SYSCLK/2。");
        if (chip.CanKind == "DCAN" && !Same(i.CanClockMhz, i.SysClockMhz) && !Same(i.CanClockMhz, i.CrystalMhz))
            throw new ArgumentException("DCAN 时钟请选择 SYSCLK 或外部晶振；需与工程 CAN 时钟源选择一致。");
    }

    public static string GenerateCode(CanInput input, CanTiming timing, bool driverLib)
    {
        Validate(input);
        var chip = Chips.Get(input.ChipId);
        var max = chip.CanKind == "eCAN" ? 256 : 1024;
        if (timing.Prescaler < (chip.CanKind == "eCAN" ? 2 : 1) || timing.Prescaler > max || timing.Tseg1 is < 2 or > 16 || timing.Tseg2 is < 1 or > 8 || timing.Sjw != input.Sjw || timing.Sjw > timing.Tseg2 || timing.TotalTq < 8 || (chip.CanKind == "eCAN" && (timing.Tseg2 < 2 || timing.Tseg1 < timing.Tseg2)))
            throw new ArgumentException("CAN 时序超出所选芯片支持范围。");
        var actualKbps = input.CanClockMhz * 1000 / timing.Prescaler / timing.TotalTq;
        var samplePoint = 100.0 * (1 + timing.Tseg1) / timing.TotalTq;
        if (Math.Abs(actualKbps - timing.ActualKbps) > 1e-6 || Math.Abs(samplePoint - timing.SamplePointPercent) > 1e-6)
            throw new ArgumentException("CAN时序结果与输入时钟不一致，请重新计算。");
        if (driverLib && !chip.DriverLib) return "// 此型号无受支持的官方 DriverLib 模板，请选择寄存器位域。";
        var p = timing.Prescaler - 1;
        var t1 = timing.Tseg1 - 1;
        var t2 = timing.Tseg2 - 1;
        var sjw = timing.Sjw - 1;
        var header = FormattableString.Invariant($"// {chip.Name} / TI {chip.TiModel}；经典CAN，仅时序配置\n// XTAL={input.CrystalMhz:G9} MHz; SYSCLK={input.SysClockMhz:G9} MHz; CANCLK={input.CanClockMhz:G9} MHz\n// CANCLK须已在工程中正确选择；不修改PLL、GPIO、邮箱、中断或外设时钟使能。\n// 实际={timing.ActualKbps:G9} kbit/s; 采样点={timing.SamplePointPercent:F3}%; SJW={timing.Sjw} TQ\n// 以下字段/API参数均为编码值；物理分频/时间段数减1。运行中修改将中断CAN通信。\n");
        if (chip.CanKind == "eCAN")
            return header + $$"""
// 依赖匹配型号的位域总头文件；ECanaRegs；CANMC/CANBTC的32位shadow写入。
void FAE_configCanBitTiming(void)
{
    union CANMC_REG mc;
    union CANBTC_REG btc;
    unsigned long savedCcr;
    EALLOW;
    mc.all = ECanaRegs.CANMC.all;
    savedCcr = mc.bit.CCR;
    mc.bit.CCR = 1;
    ECanaRegs.CANMC.all = mc.all;
    while (ECanaRegs.CANES.bit.CCE != 1) { }
    btc.all = 0UL; /* CANBTC保留位按TRM写0 */
    btc.bit.BRPREG = {{p}};
    btc.bit.TSEG1REG = {{t1}};
    btc.bit.TSEG2REG = {{t2}};
    btc.bit.SJWREG = {{sjw}};
    btc.bit.SAM = 0; /* 单次采样 */
    ECanaRegs.CANBTC.all = btc.all;
    mc.all = ECanaRegs.CANMC.all;
    mc.bit.CCR = savedCcr;
    ECanaRegs.CANMC.all = mc.all;
    if (savedCcr == 0) { while (ECanaRegs.CANES.bit.CCE != 0) { } }
    EDIS;
}
""";
        if (driverLib)
            return header + $$"""
// 依赖匹配型号 C2000Ware driverlib.h、driverlib.lib。
// CAN_setBitTiming内部设置INIT/CCE并恢复原INIT状态；不启动/初始化其余外设。
void FAE_configCanBitTiming(void)
{
    CAN_setBitTiming(CANA_BASE, {{p & 63}}U, {{p >> 6}}U, {{t1}}U, {{t2}}U, {{sjw}}U);
}
""";
        return header + $$"""
// 依赖匹配型号位域总头文件，CanaRegs；保留原控制寄存器与INIT状态。
void FAE_configCanBitTiming(void)
{
    unsigned long savedCtl;
    union CAN_BTR_REG btr;
    savedCtl = CanaRegs.CAN_CTL.all;
    CanaRegs.CAN_CTL.all = savedCtl | 0x41UL; /* INIT=1, CCE=1 */
    btr.all = 0UL;
    btr.bit.BRP = {{p & 63}};
    btr.bit.BRPE = {{p >> 6}};
    btr.bit.TSEG1 = {{t1}};
    btr.bit.TSEG2 = {{t2}};
    btr.bit.SJW = {{sjw}};
    CanaRegs.CAN_BTR.all = btr.all;
    CanaRegs.CAN_CTL.all = savedCtl & ~0x40UL; /* 清CCE并恢复INIT */
}
""";
    }
}
