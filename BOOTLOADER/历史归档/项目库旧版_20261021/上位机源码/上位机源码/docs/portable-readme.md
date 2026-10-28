# C2000 统一上位机便携版

适用 Windows 10/11 x64。完整解压后，双击 `Updater.Desktop.exe`。
随包包含 x64 .NET 桌面运行时、x86 CAN 桥接运行时、CANPRO 的 USBCAN 驱动库及 VC80 依赖、TI hex2000 转换工具；无需安装 CCS、Visual Studio、.NET 或 CANPRO。

选择芯片后再选择通信和对应 OUT；切换芯片会清除已加载固件，升级期间设置锁定。只选择固件不会擦写设备，点击“开始升级”才开始。

| 芯片 | 使用方式 | 固件 |
|---|---|---|
| F280039C | 原 CAN / SCI 功能及仿真 | `firmware/application.out`；OUT 自动转换，保留原 TXT/HEX/JSON 导入 |
| AVP32F335 | SCI-A，115200 / 8N1；仅 SCI | `firmware/335/30MHz` 或 `33.3MHz` 内 APP OUT；内部生成 BIN 数据 |

335 首次必须用 JTAG 烧录对应晶振的 **V201 BOOT**。GPIO28/RX、GPIO29/TX、共地；详细步骤见 `335接线与首次烧录.md`。335 APP 正文上限 448 KB 减 4 字节，旧 64 KB BOOT 和旧 APP 链接布局会被拒绝。当前 335 已完成软件/编译验证，实板验收待执行。

CAN 默认使用 `drivers/zlg/x86/usbcan.dll`，不依赖原电脑 C 盘路径。

首次在新电脑连接 CAN 盒，若设备管理器尚未识别设备，请运行 `Install-USB-Driver.cmd` 并允许 Windows 管理员提示。系统 USB 驱动必须向 Windows 注册，不能仅靠复制 DLL 替代。随附驱动来自当前已安装的 ZLG USBCAN 驱动，匹配 USB VID_0471/PID_1200；其他型号或被 Windows 安全策略阻止的旧驱动需使用匹配的厂商驱动。不自动关闭系统安全设置。

窗口可拖动边框/右下角调整大小，也可最大化。最小尺寸 640×480；较小时可滚动查看完整操作区。

分发时请发送整个 ZIP，不要只发 EXE。所有第三方文件保留原样，许可材料见 `licenses`。本机验证不等同于全新电脑的硬件验收。

VC80 使用 Windows 标准并行程序集加载：已有系统运行库时可能优先使用系统版本；未安装时提供包内私有程序集。另随附 `drivers/vcredist_vs2005_sp1_x86.exe` 供出现并行配置错误的电脑修复环境，不在程序启动时自动安装。
