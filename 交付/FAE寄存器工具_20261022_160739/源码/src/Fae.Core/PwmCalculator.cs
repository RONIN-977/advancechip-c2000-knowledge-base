namespace Fae.Core;

public static class PwmCalculator
{
    public static readonly int[] HighSpeedDividers = [1, 2, 4, 6, 8, 10, 12, 14];
    public static readonly int[] ClockDividers = [1, 2, 4, 8, 16, 32, 64, 128];
    public static string NormalizeMode(string mode) => mode.Trim().ToLowerInvariant() switch
    {
        "up" or "向上" or "向上计数" => "up",
        "down" or "向下" or "向下计数" => "down",
        "updown" or "up-down" or "上下" or "上下计数" => "updown",
        _ => throw new ArgumentException("计数模式请选择向上、向下或上下计数。")
    };

    public static PwmResult Calculate(PwmInput input)
    {
        var chip = Chips.Get(input.ChipId);
        if (!double.IsFinite(input.SysClockMhz) || input.SysClockMhz <= 0 || input.SysClockMhz > chip.MaxClockMhz)
            throw new ArgumentException($"主频须大于0且不超过{chip.MaxClockMhz} MHz。");
        if (!HighSpeedDividers.Contains(input.HighSpeedDivider) || !ClockDividers.Contains(input.ClockDivider))
            throw new ArgumentException("PWM分频值不受硬件支持。");
        if (input.PeripheralDivider is not (1 or 2) || (!chip.DriverLib && input.PeripheralDivider != 1))
            throw new ArgumentException("035/335 的外设分频固定1；0039/0049可选择1或2，须匹配工程EPWMCLK配置。");
        var tb = input.SysClockMhz * 1e6 / input.PeripheralDivider / input.HighSpeedDivider / input.ClockDivider;
        var mode = NormalizeMode(input.Mode);
        if (!input.Period.HasValue && (!double.IsFinite(input.TargetHz) || input.TargetHz <= 0))
            throw new ArgumentException("目标PWM频率须为正数。");
        var ideal = input.Period ?? (mode == "updown" ? tb / (2 * input.TargetHz) : tb / input.TargetHz - 1);
        if (!double.IsFinite(ideal) || ideal < (mode == "updown" ? 1 : 0) || ideal > 65535)
            throw new ArgumentException("目标对应的TBPRD超出16位范围，请调整分频或频率。");
        var period = (int)Math.Round(ideal, MidpointRounding.AwayFromZero);
        var actual = mode == "updown" ? tb / (2 * period) : tb / (period + 1);
        return new(period, tb, actual, input.Period.HasValue ? 0 : (actual / input.TargetHz - 1) * 100);
    }

    public static string GenerateCode(PwmInput input, PwmResult result, bool driverLib)
    {
        var expected = Calculate(input);
        if (expected.Period != result.Period) throw new ArgumentException("周期结果与输入不一致，请重新计算。");
        var chip = Chips.Get(input.ChipId);
        if (driverLib && !chip.DriverLib) return "// 此型号无受支持的官方 DriverLib 模板，请选择寄存器位域。";
        var mode = NormalizeMode(input.Mode);
        var modeValue = mode == "up" ? 0 : mode == "down" ? 1 : 2;
        var header = FormattableString.Invariant($"// {chip.Name} / TI {chip.TiModel}；仅PWM时基参数\n// SYSCLK={input.SysClockMhz:G9} MHz; EPWMCLK=SYSCLK/{input.PeripheralDivider}; TBCLK={result.TbClockHz:G12} Hz\n// EPWMCLK分频须已在工程配置；不更改系统时钟、GPIO、动作限定、死区、中断。\n// 实际频率={result.ActualHz:G12} Hz；TBPRD={result.Period}。请在时基停止/初始化阶段调用。\n");
        if (driverLib)
            return header + $$"""
// 依赖匹配型号的driverlib.h和driverlib.lib；示例实例EPWM1。
void FAE_configPwmTimeBase(void)
{
    EPWM_setClockPrescaler(EPWM1_BASE, EPWM_CLOCK_DIVIDER_{{input.ClockDivider}}, EPWM_HSCLOCK_DIVIDER_{{input.HighSpeedDivider}});
    EPWM_setTimeBasePeriod(EPWM1_BASE, {{result.Period}}U);
    EPWM_setTimeBaseCounterMode(EPWM1_BASE, EPWM_COUNTER_MODE_{{(mode == "up" ? "UP" : mode == "down" ? "DOWN" : "UP_DOWN")}});
}
""";
        return header + $$"""
// 依赖匹配型号的位域总头文件；示例实例EPwm1Regs。
void FAE_configPwmTimeBase(void)
{
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = {{Array.IndexOf(HighSpeedDividers, input.HighSpeedDivider)}};
    EPwm1Regs.TBCTL.bit.CLKDIV = {{Array.IndexOf(ClockDividers, input.ClockDivider)}};
    EPwm1Regs.TBPRD = {{result.Period}};
    EPwm1Regs.TBCTL.bit.CTRMODE = {{modeValue}};
}
""";
    }
}
