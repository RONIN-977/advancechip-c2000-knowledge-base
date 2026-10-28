using Fae.Core;
using System.Numerics;
using System.Text;

int passed = 0;
void Check(bool condition, string label) { if (!condition) throw new Exception("FAIL: " + label); passed++; }
void Near(double actual, double expected, string label) => Check(Math.Abs(actual - expected) < Math.Max(1e-8, Math.Abs(expected) * 1e-10), label);
void Throws(Action action, string label) { try { action(); } catch (ArgumentException) { passed++; return; } throw new Exception("FAIL expected ArgumentException: " + label); }

var exportRoot = args.Length > 0 ? Path.GetFullPath(args[0]) : Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "../../../../../artifacts/ccs"));
Directory.CreateDirectory(exportRoot);
foreach (var chip in Chips.All)
{
    var sys = chip.Id switch { "035" => 60, "335" => 150, "0039" => 120, _ => 100 };
    var clock = chip.CanKind == "eCAN" ? sys / 2.0 : sys;
    foreach (var rate in new[] { 125.0, 250, 500, 1000 })
    {
        var input = new CanInput(chip.Id, chip.Id == "335" ? 30 : 20, sys, clock, rate);
        var timings = CanCalculator.Calculate(input);
        Check(timings.Count > 0, chip.Id + " timing candidates");
        Near(timings[0].ActualKbps, rate, chip.Id + " exact common baud");
        foreach (var t in timings)
        {
            Near(t.ActualKbps, clock * 1000 / t.Prescaler / (1 + t.Tseg1 + t.Tseg2), "CAN formula");
            Check(t.Tseg2 >= t.Sjw && t.Tseg1 >= t.Sjw && t.TotalTq is >= 8 and <= 25, "legal CAN constraints");
        }
        foreach (var driver in chip.DriverLib ? new[] { false, true } : new[] { false })
        {
            var code = CanCalculator.GenerateCode(input, timings[0], driver);
            var kind = driver ? "driverlib" : "bitfield";
            string include = driver ? "driverlib.h" : chip.Id switch { "035" => "DSP2803x_Device.h", "335" => "DSP2833x_Device.h", "0039" => "f28003x_device.h", _ => "f28004x_device.h" };
            File.WriteAllText(Path.Combine(exportRoot, $"can_{chip.Id}_{kind}_{rate:0}.c"), $"#include \"{include}\"\n" + code, Encoding.UTF8);
        }
    }
    var nonExact = CanCalculator.Calculate(new(chip.Id, chip.Id == "335" ? 30 : 20, sys, clock, 333.333));
    Check(Math.Abs(nonExact[0].ErrorPercent) > 0, "nonexact baud reported");
    foreach (var mode in new[] { "up", "down", "updown" })
    {
        foreach (var frequency in new[] { 1000.0, 5000, 10000 })
        {
            var input = new PwmInput(chip.Id, sys, 1, 4, 1, mode, frequency);
            var result = PwmCalculator.Calculate(input);
            Near(result.ActualHz, frequency, "exact PWM common frequency");
            var reverse = PwmCalculator.Calculate(input with { TargetHz = 0, Period = result.Period });
            Near(reverse.ActualHz, frequency, "reverse PWM");
            foreach (var driver in chip.DriverLib ? new[] { false, true } : new[] { false })
            {
                var code = PwmCalculator.GenerateCode(input, result, driver);
                var kind = driver ? "driverlib" : "bitfield";
                string include = driver ? "driverlib.h" : chip.Id switch { "035" => "DSP2803x_Device.h", "335" => "DSP2833x_Device.h", "0039" => "f28003x_device.h", _ => "f28004x_device.h" };
                File.WriteAllText(Path.Combine(exportRoot, $"pwm_{chip.Id}_{kind}_{mode}_{frequency:0}.c"), $"#include \"{include}\"\n" + code, Encoding.UTF8);
            }
        }
    }
}
var baseCan = new CanInput("035", 20, 60, 30, 500);
Throws(() => CanCalculator.Calculate(new("335", 10, 150, 75, 500)), "335 unreachable crystal/sysclock relation");
Near(CanCalculator.Calculate(new("335", 30, 150, 75, 500))[0].ActualKbps, 500, "335 reachable crystal/sysclock relation");
Throws(() => CanCalculator.Calculate(baseCan with { SysClockMhz = 61, CanClockMhz = 30.5 }), "035 TI baseline clock limit");
Throws(() => CanCalculator.Calculate(baseCan with { CanClockMhz = 60 }), "eCAN must divide by2");
Throws(() => CanCalculator.Calculate(baseCan with { TargetKbps = 0 }), "CAN zero rate");
Throws(() => CanCalculator.Calculate(baseCan with { TargetKbps = double.NaN }), "CAN NaN");
Throws(() => CanCalculator.Calculate(baseCan with { Sjw = 5 }), "CAN SJW");
Throws(() => CanCalculator.Calculate(baseCan with { SamplePointPercent = 100 }), "CAN sample point");
var maxBrp = CanCalculator.Calculate(new("0039", 20, 120, 120, 5));
Check(maxBrp[0].Prescaler > 64, "DCAN extension prescaler used");
foreach (var chip in Chips.All)
{
    var sys = chip.Id switch { "035" => 60, "335" => 150, "0039" => 120, _ => 100 };
    var clock = chip.CanKind == "eCAN" ? sys / 2.0 : sys;
    var input = new CanInput(chip.Id, chip.Id == "335" ? 30 : 20, sys, clock, 125, 80, 4);
    var timing = CanCalculator.Calculate(input)[0];
    Check(timing.Tseg2 >= 4, "SJW4 timing constraint");
    foreach (var driver in chip.DriverLib ? new[] { false, true } : new[] { false })
    {
        string include = driver ? "driverlib.h" : chip.Id switch { "035" => "DSP2803x_Device.h", "335" => "DSP2833x_Device.h", "0039" => "f28003x_device.h", _ => "f28004x_device.h" };
        File.WriteAllText(Path.Combine(exportRoot, $"can_{chip.Id}_{(driver ? "driverlib" : "bitfield")}_sjw4.c"), $"#include \"{include}\"\n" + CanCalculator.GenerateCode(input, timing, driver), Encoding.UTF8);
        if (chip.DriverLib)
        {
            var extended = input with { TargetKbps = 5 };
            var extTiming = CanCalculator.Calculate(extended)[0];
            Check(extTiming.Prescaler > 64, "DCAN BRPE exercised");
            File.WriteAllText(Path.Combine(exportRoot, $"can_{chip.Id}_{(driver ? "driverlib" : "bitfield")}_brpe.c"), $"#include \"{include}\"\n" + CanCalculator.GenerateCode(extended, extTiming, driver), Encoding.UTF8);
        }
    }
    if (chip.CanKind == "eCAN")
    {
        Check(CanCalculator.Calculate(input).All(x => x.Prescaler >= 2 && x.Tseg2 >= 2 && x.Tseg1 >= x.Tseg2), "eCAN IPT and BRP constraints");
        Throws(() => CanCalculator.GenerateCode(input, timing with { Prescaler = 1 }, false), "forbidden eCAN BRP1");
        Throws(() => CanCalculator.GenerateCode(input, timing with { Tseg2 = 1 }, false), "forbidden eCAN TSEG2=1");
    }
    Throws(() => CanCalculator.GenerateCode(input, timing with { ActualKbps = timing.ActualKbps + 1 }, false), "stale CAN actual timing rejected");
}
var xtalCan = CanCalculator.Calculate(new("0049", 20, 100, 20, 500));
Near(xtalCan[0].ActualKbps, 500, "DCAN XTAL clock");
var impossibleCan = CanCalculator.Calculate(new("035", 20, 60, 30, 997.7));
Check(Math.Abs(impossibleCan[0].ErrorPercent) > 0.001, "unachievable baud remains explicitly nonzero");
var basePwm = new PwmInput("335", 150, 1, 1, 1, "up", 10000);
Check(PwmCalculator.Calculate(basePwm).Period == 14999, "up counter +1");
Check(PwmCalculator.Calculate(basePwm with { Mode = "updown" }).Period == 7500, "updown factor2");
Throws(() => PwmCalculator.Calculate(basePwm with { ClockDivider = 3 }), "invalid PWM divider");
Throws(() => PwmCalculator.Calculate(basePwm with { Period = -1 }), "invalid PWM period");
Throws(() => PwmCalculator.Calculate(basePwm with { Mode = "updown", Period = 0 }), "updown cannot zero");
Throws(() => PwmCalculator.Calculate(basePwm with { TargetHz = 1 }), "PWM out of range");
Check(Math.Abs(PwmCalculator.Calculate(basePwm with { TargetHz = 12345 }).ErrorPercent) > 0, "nonexact PWM error reported");
Near(AdcCalculator.FromCode(0).PinVolts, 0, "ADC zero");
Near(AdcCalculator.FromCode(2048).PinVolts, 1.65, "ADC half scale");
Near(AdcCalculator.FromCode(4095).PinVolts, 3.3 * 4095 / 4096, "ADC max code");
Check(AdcCalculator.FromVoltage(3.3).Code == 4095, "ADC fullscale clamp");
Near(AdcCalculator.FromCode(2048, 3.3, 11, 0.01, 10, 1.65).CurrentAmps, 0, "ADC current zero offset");
Near(AdcCalculator.FromCode(2048, 3.3, 11).InputVolts, 18.15, "ADC divider");
Throws(() => AdcCalculator.FromCode(4096), "ADC code out of range");
Throws(() => AdcCalculator.FromVoltage(3.4), "ADC volts out of range");
Throws(() => AdcCalculator.FromCode(1, shuntOhms: 0), "ADC zero shunt");
var address = AddressCalculator.FromRange("0x3F0000", "0x3F00FF");
Check(address.Bytes == 512 && address.Words == 256, "C28x word addresses inclusive");
Check(AddressCalculator.FromSize("0x100", "512").End == 0x1FF, "address reverse size");
Check(AddressCalculator.FromRange("0", "0", false).Bytes == 1, "byte single address");
Check(AddressCalculator.FromRange("0", "0", false).Notes.Contains("半个"), "odd byte capacity warning");
Throws(() => AddressCalculator.FromRange("10", "9"), "reverse range");
Throws(() => AddressCalculator.FromSize("0xFFFFFFFF", "4"), "address overflow");
Throws(() => AddressCalculator.FromSize("0", "3"), "odd word capacity");
Check(AddressCalculator.FromRange("0", "0xFFFFFFFF", false).Bytes == BigInteger.One << 32, "32bit address full range no overflow");
foreach (var bits in new[] { 8, 16, 32, 64 })
{
    var mask = (BigInteger.One << bits) - 1;
    Check(ProgrammerCalculator.Evaluate("~0", bits) == mask, "not truncation");
    Check(ProgrammerCalculator.Evaluate("-1+1", bits) == 0, "overflow wraps");
    Check(ProgrammerCalculator.Format(mask, 10, bits, true) == "-1", "signed view");
    Check(ProgrammerCalculator.Evaluate("-2>>1", bits, true) == mask, "arithmetic right shift");
    Check(ProgrammerCalculator.Evaluate("-2>>1", bits, false) == mask / 2, "logical right shift");
    Check(ProgrammerCalculator.Evaluate("1<<" + bits, bits) == 0, "large shifts zero");
    Check(ProgrammerCalculator.ToggleBit(0, bits - 1, bits) == BigInteger.One << (bits - 1), "toggle sign bit");
    foreach (var radix in new[] { 2, 8, 10, 16 })
        Check(ProgrammerCalculator.Evaluate(ProgrammerCalculator.Format(mask, radix, bits), bits, false, radix) == mask, "base roundtrip");
}
Check(ProgrammerCalculator.Evaluate("(2+3)*4|1") == 21, "precedence parentheses");
Check(ProgrammerCalculator.Evaluate("FF & 0F", radix: 16) == 15, "hex input");
Check(ProgrammerCalculator.Evaluate("0x10+0b10+0o10") == 26, "mixed prefixes");
Check(ProgrammerCalculator.Evaluate("-7/2", 8, true) == 253, "signed integer division");
Throws(() => ProgrammerCalculator.Evaluate("1/0"), "division zero");
Throws(() => ProgrammerCalculator.Evaluate("(1+2"), "unmatched parentheses");
Throws(() => ProgrammerCalculator.Evaluate("2", radix: 2), "invalid digit");
Console.WriteLine($"PASS {passed} checks; generated C fixtures: {Directory.GetFiles(exportRoot, "*.c").Length}; {exportRoot}");
