from pathlib import Path
import re
work=Path(__file__).resolve().parent
for model in ('ADP32F035','AVP32F00157'):
    root=work/'staging'/(model+'Boot+APP_20261009')/model
    p=root/'README.md'; text=p.read_text(encoding='utf-8')
    notice='> 2026-10-09 独立工程版：BOOT、APP 的 Common 源码、Device 头文件/初始化源码和所需库均在各自目录内；外层 Common 只保留唯一的 app_partition.h。工程使用 CCS 12.7.1 / C2000 22.6.1.LTS。详见 [独立工程与CCS12.7.1说明](独立工程与CCS12.7.1说明.md)。\n\n'
    text=notice+text
    if model=='ADP32F035':
        text=text.replace('共享源码和库使用相对路径','每个工程使用自己的源码和库，只通过相对路径共用地址头文件')
        text=text.replace('采用 C2000 COFF ABI；选择本机已安装的兼容编译器','采用 C2000 COFF ABI；默认选择 22.6.1.LTS 编译器')
        text=text.replace('修改分区时同步 partition_config.h、链接脚本及上位机地址；详见本次优化说明','修改 APP 起点只改 Common/app_partition.h 并重新编译 BOOT、APP，上位机填相同起点；详见单头文件修改说明')
        text=text.replace('Common 是共享通讯、协议和跳转；Device 是 TI 初始化与头文件','外层 Common 只保留共享地址文件；每个工程自己的 Common 是通讯、协议和跳转源码，自己的 Device 是 TI 初始化、头文件及所需库')
        text=text.replace('Common/boot_io.c 的','Application/Common/boot_io.c 的')
    else:
        text=text.replace('- Common：两工程共用的协议、SCI/CAN、计时和跳转代码。','- Common：只保留共享地址配置 app_partition.h。')
        text=text.replace('- Device：随包 TI 头文件、原版初始化文件、driverlib 和 Flash API。','- 各工程内 Common / Device：各自的协议源码、TI 头文件、初始化文件、driverlib 和 Flash API。')
        text=text.replace('公共源码使用 PARENT-1-PROJECT_LOC 链接。本次验证环境为 CCS 21、TI C2000 编译器 25.11.1.LTS','只有地址头文件使用 PARENT-1-PROJECT_LOC 链接，其余源码和库均在工程内。本次验证环境为 CCS 12.7.1、TI C2000 编译器 22.6.1.LTS')
        text=text.replace('链接进来的 Common、Device 文件中','工程内 Common、Device 文件中')
        text=text.replace('C:/ti/ccs2100/ccs/tools/compiler/ti-cgt-c2000_25.11.1.LTS','C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS')
    p.write_text(text,encoding='utf-8')
    p=root/'单头文件修改说明.md'; text=p.read_text(encoding='utf-8')
    text=text.replace('完整解压工程，保持 BOOT、APP 和 Common 的相对位置。','完整解压工程，保持 Bootloader、Application 和外层 Common 的相对位置。源码和库已放进各自工程，外层 Common 只有共享地址文件。')
    text=text.replace('高级改动仍可通过 partition_config.h 调整，但可能要求主机同步结束地址。','高级分区改动须同时核对 Bootloader/Common/partition_config.h 和 Application/Common/partition_config.h，并可能要求主机同步结束地址。普通 APP 起点修改仍只改共享 app_partition.h。')
    text+='\n当前独立工程版使用 CCS 12.7.1 / C2000 22.6.1.LTS；导入后选 Debug，执行 Clean / Rebuild。详见《独立工程与CCS12.7.1说明》。\n'
    p.write_text(text,encoding='utf-8')
    for p in (root/'docs').glob('*.md'):
        text=p.read_text(encoding='utf-8')
        text='> 独立工程版目录说明：本文中的 Common 源码、Device 文件现位于各自 Bootloader / Application 内；只有 Common/app_partition.h 仍在型号根目录共享。普通 APP 起点只改该共享文件，旧版多处修改步骤不适用。\n\n'+text
        p.write_text(text,encoding='utf-8')
    p=root/'tools/package.py'; text=p.read_text(encoding='utf-8')
    text=text.replace("'README.md','单头文件修改说明.md'", "'README.md','独立工程与CCS12.7.1说明.md','单头文件修改说明.md'")
    p.write_text(text,encoding='utf-8')
    p=root/'独立工程与CCS12.7.1说明.md'
    start,alignment=('0x3E8000','0x2000') if model=='ADP32F035' else ('0x088000','0x400')
    p.write_text(f'''# {model} 独立工程使用说明

目录结构：

```text
{model}/
  Common/app_partition.h     唯一共享的地址配置
  Bootloader/
    main.c、bootloader.cmd、Flash 擦写代码
    Common/                 BOOT 自己的通信及升级源码
    Device/                 BOOT 自己的头文件、初始化源码和库
    .project / .cproject / .ccsproject
  Application/
    main.c、application.cmd
    Common/                 APP 自己的通信及返回 BOOT 源码
    Device/                 APP 自己的头文件、初始化源码和库
    .project / .cproject / .ccsproject
  output/                   默认地址下的 OUT、MAP、APP SCI8 TXT
```

1. 完整解压，在 CCS **12.7.1** 的 Import CCS Projects 中选择两个工程。不要勾选 Copy projects into workspace；共享地址文件需要保留上述相对位置。可以只导入其中一个工程，但不要只搬走工程子目录。
2. 默认编译器为 **TI C2000 22.6.1.LTS**（CCS 12.7.1 自带）。选择 Debug，Clean / Rebuild。
3. 地址修改入口保持 `Common/app_partition.h`，默认 `{start}`，按 `{alignment} WORD` 对齐。两个工程树中的 app_partition.h 指向这一份真实文件。
4. 改地址后重新编译 BOOT 和 APP，用 JTAG 下载新 BOOT；上位机填写相同 APP 起点，加载对应新 APP OUT。链接文件、擦除范围、设备返回的地址按共享头文件配置生成，无需手动改 .cmd。
5. 各工程 Common、Device 是实体文件，不再引用另一个工程或外层源码/库。修改 APP 功能使用 Application/main.c 及其 Common；修改 BOOT 使用 Bootloader 内文件。地址以外的公共代码现在是两份独立副本，需同步变更时分别修改。

编译/链接的包含与库搜索路径使用相对路径，便于在当前中文目录及整体搬迁后编译。不得只复制一个工程到另一个 workspace 根目录而丢失外层共享头文件；如需迁移，整体复制型号目录。

命令行也可在本目录运行 `powershell -File build.ps1`，默认调用 CCS 12.7.1 的 22.6.1 编译器。该脚本重新生成 output；CCS Build 生成的调试文件则在各工程 Debug 内。

本次只整理工程依赖和编译配置；时钟、GPIO、通信协议、升级状态机、Flash API 库及分区默认值均保持原版。CCS 验证记录见 output/CCS12.7.1_验证记录.md。本次没有烧录和实板测试。
''',encoding='utf-8')
print('Updated current instructions and packaging paths')
