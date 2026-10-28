> 当前维护入口：请先阅读 [当前维护说明.md](当前维护说明.md)。本目录已合并 AVP32F00157，后续只维护此份源码，不重新制作上位机压缩包。以下旧交付说明保留为历史参考。

## UI 优化（2026-10-08）

默认 800×620，真实设备自动检索、进芯型号显示、日志清除及通俗按钮文字。“开始升级”自动完成进入升级和下载，成功后固定发送原有复位／启动请求；“读取设备信息”可测试通讯，有效应答后才记录通讯正常。原核心协议与收发逻辑保持不变，仿真移至后台。使用和验证边界见 [UI优化说明](UI优化说明.md)。

## F28035 扩展（2026-10-05）

新增 **TMS320F28035** 选型，复用现有 SCI/CAN 升级协议。配套外部 10MHz 晶体、60MHz SYSCLK 的 BOOT/APP；SCI GPIO28/RX、GPIO29/TX，115200；eCAN GPIO30/RX、GPIO31/TX，500kbps、节点 1。APP 入口 0x3E8000，末地址 0x3F1FFF（16 位字地址，容量 80KB），GPIO17 每 500ms 翻转。

首次使用 JTAG 烧录配套 BOOT，然后选择 ADP32F035 和实际接口，加载该型号 APP OUT，点击“开始升级”自动进入升级模式。F280039C、AVP32F335、F280049 功能保留。详细烧录和 CCS 说明见同级 UDS_OriCode 下的 [F280035_Bootloader/README.md](../../../../F280035_Bootloader/README.md)。

# F280039C / AVP32F335 / F280049 统一上位机源码

2026-09-30 新增 F280049 / F280049C 的 SCI/CAN 选型，保留原两型号的功能和协议。配套 20 MHz 晶振 Bootloader、APP、CCS 工程及使用说明在 `../../../../F280049_Bootloader`；新增分区 `0x090000–0x09FFEF`，协议身份 `0x28004901`。实际下载前核对所选型号及 APP 链接地址。详见 [280049 使用说明](docs/280049-sci-can.md)。

本包用于上位机二次开发，包含当前已交付版本的完整 C# 源码、测试、协议文档及可用的第三方依赖。不是单纯的运行程序包。

本次增加 AVP32F335 芯片选择、SCI-A 协议、COFF OUT 自动装载及 MD5 升级客户端；F280039C 原 CAN/SCI、仿真、OUT 转换和 x86 桥接保留。335 接线、V201 BOOT 首次烧录及晶振选择见 [335 使用说明](docs/335-sci.md)。配套 MCU 源码和四个 OUT 在知识库 `AVP32F335/335SCI统一升级_V201`；原始 335 资料保持原样。

## 开发环境及启动

Windows 10/11 x64、.NET 10 SDK（本次使用 10.0.300）、PowerShell 7。
可用支持 .NET 10 的 Visual Studio 打开 `F280039C-Updater.sln`，或使用 VS Code/命令行。
普通开发构建的 32 位兼容桥接由 Windows .NET Framework 4.x 的 csc.exe 编译；便携发布改用自带运行时的 .NET 10 x86 桥接。

在解压目录执行：

```powershell
pwsh -File scripts/build-host.ps1 -Test
```

完成后启动 `host/Updater.Desktop/bin/Release/net10.0-windows/Updater.Desktop.exe`。
无需安装 CCS 或 CANPRO：驱动库及 hex2000 已放在 assets，脚本会复制到输出目录。
若直接点击 IDE 运行，请先执行上述脚本，确保依赖已复制到 Release 输出；Debug 调试时将 assets 下的内容复制到对应 Debug 输出目录。

## 工程划分

| 路径 | 作用 |
|---|---|
| host/Updater.Desktop | WPF 界面、Win32 串口、ControlCAN 通信、驱动/转换工具发现 |
| host/Updater.Core | 协议封包、CRC、升级状态流程、TI boot table 解析、OUT 转换、仿真 |
| host/CanBridge32 | 32 位 VCI 驱动桥接，通过标准输入输出与上位机通信 |
| tests/Updater.Tests | 55 项控制台测试（传入 335 固件根目录时），另有 10 项真实 C 协议互通测试；包含 OUT、边界、CRC、MD5、重传和取消 |
| assets | CANPRO 驱动、VC80 依赖、USB 系统驱动、TI hex2000 和许可材料 |
| build/firmware | 测试样本 application.out/.txt 与用于拒绝测试的 bootloader.out |
| docs/protocol.md / docs/335-sci.md | 原 F280039C 协议与新增 335 SCI 协议、接线及地址规则 |

源码为 UTF-8。未包含 MCU 源码、编译器、SDK、开发缓存或 .NET 运行时；build/firmware 是必要的测试样本，不是开发生成垃圾文件。

## 重新生成便携版

```powershell
pwsh -File scripts/publish-portable.ps1
```

首次发布需联网下载微软 x64/x86 自包含运行时；产物在 build/portable/C2000-Updater。运行前先生成两套 335 固件；独立解压本源码包时用 `-Avp335Root` 指定 MCU 包路径。长目录建议用 `-PackageCache` 指定较短的 NuGet 缓存路径。
脚本仅从本包 assets 取得驱动和工具，不需要原开发电脑的 C:\ZLGCANPRO、C:\ti 或 DriverStore 路径。
发送整个便携 ZIP，不能只发 EXE。新电脑首次使用 USB CAN 盒可能需要管理员运行随包驱动安装脚本。

完整测试可执行 `scripts/build-host.ps1 -Test -Avp335Root <335修正版根目录>`；先构建新增 280049 固件，没有 335 参数时运行 53 项（不含两个实际 335 OUT 检查）。新增 280049 专项及真实 C 协议互通测试可执行配套 Bootloader 的 `scripts/verify.ps1`。可用 `-Dotnet <dotnet.exe>` 指定便携 SDK，SDK/编译器不随源码交付。

## 维护入口与兼容约束

- 界面及动作：MainWindow.xaml / MainWindow.xaml.cs。
- 串口和 CAN：HardwareTransports.cs；包内驱动优先级见 CanDriver.cs。
- OUT 自动转换：TiOutConverter.cs；转换工具发现见 TiToolLocator.cs。
- 协议与升级流程：Protocol.cs / Updater.cs；仿真设备：Simulator.cs。
- 分区与身份：Firmware.cs 中 Profile。当前为 F280039C、应用字地址 0x090000–0x0AEFFF、节点默认 1。
- 协议兼容性必须与 Bootloader 同步修改；C28x 地址单位是 16 位字，不是 PC 字节。
- 选择固件仅载入/校验；点击“开始升级”后才打开升级流程。

用户已验收 CAN 烧录、应用跳转、GPIO17 闪灯和直接 OUT 升级。便携版完成本机自检，尚不等于全新系统/所有适配器验收。
第三方二进制保留原样及许可材料，TI 工具仅用于 TI 设备；详见 assets/licenses 和 docs。
