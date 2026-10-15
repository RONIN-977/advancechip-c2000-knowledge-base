> 当前维护方式：请先阅读 [单头文件修改说明](单头文件修改说明.md)。只修改 Common/app_partition.h 的 APP_START_ADDRESS，链接文件自动跟随。以下历史分区地址和手动修改说明不再作为修改入口。

# AVP32F00157 CAN / SCI 在线升级工程

基于用户提供的 AVP32F00157 与 TMS320F2800157 兼容关系开发，使用 C2000Ware 5.05.00.00 的 F280015x driverlib、原版 Device_init 和 TI Flash API。包含可导入 CCS 的 BOOT / APP 工程、源码及带调试符号的 OUT。2026-10-08 已完成编译、链接检查和软件模拟测试；尚未在 AVP32F00157 实板上验证。

## 默认配置

| 项目 | 配置 |
| --- | --- |
| 外部时钟 | 20 MHz 晶体，TI USE_PLL_SRC_XTAL 配置 |
| SYSCLK / LSPCLK | 120 MHz / 30 MHz |
| SCI-A | GPIO28 = RX，GPIO29 = TX；115200，8N1 |
| CAN-A | GPIO5 = RX，GPIO4 = TX；500 kbit/s |
| CAN 标准帧 ID | 主机发往设备 0x601；设备回复 0x581；节点 1 |
| APP 指示 | GPIO35 每 500 ms 翻转，完整周期 1 s |

SCI 使用 3.3 V TTL 电平、收发交叉连接并共地。CAN 经板上收发器连接总线，按实际总线安装终端电阻。GPIO35 为电平翻转输出，LED 是否点亮取决于板上电路。时钟采用晶体振荡器模式；若板上实际为有源时钟输入，需要按 TI 文档修改振荡源配置。

## 文件和 CCS 导入

- Bootloader / Application：独立 CCS 工程，含 .project、.cproject、.ccsproject。
- Common：两工程共用的协议、SCI/CAN、计时和跳转代码。
- Device：随包 TI 头文件、原版初始化文件、driverlib 和 Flash API。
- output：BOOT/APP OUT、MAP、APP SCI8 文本和编译、自检记录。
- 上位机唯一源码：UDS_OriCode/UI优化版（源码+交付）_20261008/UI优化版（源码+交付）_20261008/源码；保留优化版 UI 和原有型号。
- 对应运行程序：该源码同级的“上位机本低部署版/统一升级上位机.exe”（沿用现有目录名称）。
- docs/维护与验证.md：分区、Flash、维护点和调试说明。

本目录只维护 MCU 工程，不再复制上位机源码或生成上位机压缩包。tools/package.py 仅用于打包 MCU 工程。上位机后续改动统一在上述现有源码中完成。

CCS 中选择 Import CCS Projects，搜索 AVP32F00157，选择两个工程。保持整套相对目录关系，不勾选把单个工程复制进 workspace；公共源码使用 PARENT-1-PROJECT_LOC 链接。本次验证环境为 CCS 21、TI C2000 编译器 25.11.1.LTS，EABI / FPU32。无需运行 SysConfig，无需替换国产 Flash API。

Debug / Release 是自动生成的构建输出，CCS 排除其作为输入源码是正常行为。用户源码在工程根目录及链接进来的 Common、Device 文件中。

命令行重新构建：

```powershell
pwsh -File build.ps1 -CompilerRoot 'C:/ti/ccs2100/ccs/tools/compiler/ti-cgt-c2000_25.11.1.LTS'
```

## 首次下载与连续升级

1. 通过 CCS / JTAG 下载 output/AVP32F00157_Bootloader.out，设置板卡从 Flash 启动。调试下载后点击 Resume。空白 APP 时 BOOT 持续等待升级。
2. 运行上述现有源码配套的“上位机本低部署版/统一升级上位机.exe”，选择 AVP32F00157 和实际 SCI 或 CAN 设备。其他历史部署目录的 EXE 不代表本次版本。
3. SCI 选择对应 COM、115200；CAN 选择对应适配器、500 kbit/s、节点 1。设备在 BOOT 时可读取设备信息。
4. 加载 output/AVP32F00157_Application.out，开始升级。主机负责 TI OUT 转换，运行目录中的 tools、bridge、drivers 等文件夹须一起保留。
5. 升级完成并运行 APP 后，GPIO35 每 500 ms 翻转。
6. 再次升级时，选择固件后直接点击“开始升级”。当前上位机自动发送 ENTER_BOOT；APP 从 SCI 或 CAN 收到请求，发送完整 ACK，写入 RAM 请求标记并跳转 BOOT 启动入口，主机继续下载，不需要人工复位。

APP 仅提供返回 BOOT 的升级入口，不处理 BOOT 的 INFO / BEGIN / DATA 命令。在 APP 状态直接“读取设备信息”可能超时；使用“开始升级”会自动请求切换。当前 UI 没有独立的“进入升级”按钮。二次开发须持续调用 update_service_poll()，不可用 500 ms 阻塞延时代替本例非阻塞闪灯。

正常上电时，已有完整有效 APP 则等待约 5 s 后跳转；APP 请求返回 BOOT 时留在 BOOT 等待升级。升级先擦元数据，完成镜像校验后再写最终标记，中断升级后可重新发送完整固件。

JTAG 单独下载 APP 不会生成升级元数据。验证 BOOT 自动启动与完整在线流程时，请通过上位机下载 APP。

## 验证范围

两套 CCS 工程已实际导入和编译；通过命令行构建、Flash API RAM 位置/搬运范围检查、APP 返回 BOOT 模拟测试、23 项新型号协议/固件转换回归和发布版 SCI/CAN 界面模拟升级。记录见 output。

下一步实板验证：SCI 首次升级 → GPIO35 翻转 → SCI 返回 BOOT 再升级；用 CAN 重复相同流程，并检查断电重启及中断升级恢复。硬件结果以实测为准。
