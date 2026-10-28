"""Independent address audit against TI DriverLib address/offset definitions.

Does not import the extraction script and never changes register data. Only
literal constants and explicit instance mappings are matched; gaps are reported.
"""
import argparse
import collections
import hashlib
import json
import pathlib
import re
import sys
from datetime import datetime, timezone

ROOT = pathlib.Path(__file__).resolve().parents[1]
FAMILIES = {"0039": "f28003x", "0049": "f28004x"}
MODULES = {"ADC": "adc", "CAN": "can", "PWM": "epwm", "GPIO": "gpio",
           "SCI": "sci", "SPI": "spi", "I2C": "i2c", "定时器": "cputimer",
           "中断": "pie", "看门狗": "wwd", "时钟": "sysctl"}


def instance_base(instance):
    patterns = [
        (r"Adc([abc])Regs", lambda m: "ADC" + m[1].upper() + "_BASE"),
        (r"Adc([abc])ResultRegs", lambda m: "ADC" + m[1].upper() + "RESULT_BASE"),
        (r"Can([ab])Regs", lambda m: "CAN" + m[1].upper() + "_BASE"),
        (r"EPwm(\d+)Regs", lambda m: "EPWM" + m[1] + "_BASE"),
        (r"Sci([ab])Regs", lambda m: "SCI" + m[1].upper() + "_BASE"),
        (r"Spi([ab])Regs", lambda m: "SPI" + m[1].upper() + "_BASE"),
        (r"I2c([ab])Regs", lambda m: "I2C" + m[1].upper() + "_BASE"),
        (r"CpuTimer(\d+)Regs", lambda m: "CPUTIMER" + m[1] + "_BASE"),
    ]
    for pattern, convert in patterns:
        m = re.fullmatch(pattern, instance)
        if m:
            return convert(m)
    return {"GpioCtrlRegs": "GPIOCTRL_BASE", "GpioDataRegs": "GPIODATA_BASE",
            "GpioDataReadRegs": "GPIODATAREAD_BASE", "PieCtrlRegs": "PIECTRL_BASE",
            "WdRegs": "WD_BASE", "ClkCfgRegs": "CLKCFG_BASE", "CpuSysRegs": "CPUSYS_BASE",
            "DevCfgRegs": "DEVCFG_BASE", "SyncSocRegs": "SYNCSOC_BASE",
            "NmiIntruptRegs": "NMI_BASE", "XintRegs": "XINT_BASE"}.get(instance)


def offset_names(module, name):
    prefix = module.upper() + "_O_"
    names = [prefix + name]
    if module in ("adc", "can", "sci", "spi", "i2c", "pie"):
        token = module.upper()
        if name.startswith(token):
            names.append(prefix + name[len(token):].lstrip("_"))
    return names


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--c2000ware", default=r"C:\ti\c2000\C2000Ware_6_00_01_00")
    parser.add_argument("--data", type=pathlib.Path, default=ROOT / "Data/registers.json")
    parser.add_argument("--output", type=pathlib.Path, default=ROOT / "artifacts/address-audit.json")
    args = parser.parse_args()
    ware = pathlib.Path(args.c2000ware)
    cache, sources = {}, {}

    def header(family, module):
        key = (family, module)
        if key in cache:
            return cache[key]
        filename = "hw_" + module + ".h"
        candidates = [ROOT / "docs/sources" / family / "driverlib-inc" / filename,
                      ROOT / "docs/sources" / family / filename,
                      ware / "driverlib" / family / "driverlib/inc" / filename]
        path = next((p for p in candidates if p.exists()), None)
        values = {}
        if path:
            text = path.read_text(encoding="utf-8", errors="replace")
            for name, literal in re.findall(r"^\s*#define\s+(\w+)\s+((?:0x[0-9A-Fa-f]+|\d+))[uUlL]*\b", text, re.M):
                values[name] = int(literal, 0)
            sources[str(path)] = hashlib.sha256(path.read_bytes()).hexdigest()
        cache[key] = (values, path)
        return cache[key]

    pack = json.loads(args.data.read_text(encoding="utf-8-sig"))
    matched, errors, unmatched = [], [], []
    summary = collections.defaultdict(collections.Counter)
    for register in pack["Registers"]:
        chip = register["ChipId"]
        if chip not in FAMILIES:
            continue
        family, peripheral = FAMILIES[chip], register["Peripheral"]
        key = chip + "/" + peripheral
        module = MODULES.get(peripheral)
        item = {"ChipId": chip, "Name": register["Name"], "Actual": register["Address"]}
        if not module or "." not in register["Name"]:
            item["Reason"] = "No explicit module mapping"
            unmatched.append(item); summary[key]["Unmatched"] += 1
            continue
        instance, name = register["Name"].split(".", 1)
        base_name = instance_base(instance)
        bases, base_path = header(family, "memmap")
        offsets, offset_path = header(family, module)
        candidate_names = offset_names(module, name)
        offset_name = next((n for n in candidate_names if n in offsets), None)
        if base_name not in bases or not offset_name:
            item["Reason"] = "Base or exact register offset missing; no guessed offset"
            item["BaseDefine"] = base_name
            item["OffsetCandidates"] = candidate_names
            unmatched.append(item); summary[key]["Unmatched"] += 1
            continue
        expected = bases[base_name] + offsets[offset_name]
        item.update(Expected=f"0x{expected:06X}", BaseDefine=base_name,
                    OffsetDefine=offset_name, Offset=f"0x{offsets[offset_name]:X}",
                    OffsetSource=str(offset_path), BaseSource=str(base_path))
        if int(register["Address"], 16) != expected:
            errors.append(item); summary[key]["Errors"] += 1
        else:
            matched.append(item); summary[key]["Matched"] += 1
    report = {
        "GeneratedOn": datetime.now(timezone.utc).isoformat(),
        "DataVersion": pack["Version"], "DataSha256": hashlib.sha256(args.data.read_bytes()).hexdigest(),
        "ComparedChips": list(FAMILIES), "Matched": len(matched), "Errors": len(errors),
        "Unmatched": len(unmatched), "Summary": dict(summary), "ErrorDetails": errors,
        "UnmatchedDetails": unmatched, "SourcesSha256": sources,
        "Method": "Exact instance base plus exact DriverLib _O_ constant. No inferred matches. C28x hardware address space; byte_peripheral CAN offsets are used verbatim.",
        "Limitations": "035/335 have no matching DriverLib hardware map here and are excluded. Unmatched entries are not verified by this audit. Address correctness alone does not verify access/reset/field semantics.",
    }
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, ensure_ascii=False, indent=2), encoding="utf-8")
    print(f"Address audit: {len(matched)} matched; {len(errors)} errors; {len(unmatched)} unmatched; {args.output}")
    for item in errors[:15]:
        print(f"{item['ChipId']} {item['Name']}: {item['Actual']} != {item['Expected']}")
    return 1 if errors else 0


if __name__ == "__main__":
    sys.exit(main())
