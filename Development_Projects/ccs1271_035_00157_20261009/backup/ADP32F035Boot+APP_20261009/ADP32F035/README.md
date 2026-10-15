> 当前维护方式：请先阅读 [单头文件修改说明](单头文件修改说明.md)。只修改 Common/app_partition.h 的 APP_START_ADDRESS，链接文件自动跟随。以下历史分区地址和手动修改说明不再作为修改入口。

# ADP32F035 BOOT / APP 最新工程

2026-10-09：已优化可配置分区擦除，入口为 [分区配置与擦除优化说明](docs/分区配置与擦除优化_20261009.md)。默认分区保持不变。

此目录为唯一维护入口，工程名和输出名已统一为 ADP32F035。用户已确认上一版 ROM API 的 CAN/SCI 升级成功；本版沿用该配置，改进 APP 自动返回 BOOT 的路径。

## CCS 导入

完整解压目录，在 CCS 导入现有工程，选择 ADP32F035_Bootloader、ADP32F035_Application。不要勾选 Copy projects into workspace，共享源码和库使用相对路径。工程包含 .project/.cproject/.ccsproject。采用 C2000 COFF ABI；选择本机已安装的兼容编译器。CCS 的 TMS320F28035 是兼容调试器件配置，硬件型号为 ADP32F035。

首次烧录 output/ADP32F035_Bootloader.out；CCS 源码调试请 Clean/Rebuild 后使用对应 Debug OUT。命令行可执行 ./build.ps1，编译器目录由 -CompilerRoot 指定。output 内是本次重新构建的正常协议版本。

## 升级与 APP 自动返回

1. 当前上位机选择 ADP32F035，选择实际 SCI/CAN；默认 APP 起点为 0x3E8000。
2. 加载 output/ADP32F035_Application.out，点击“开始升级”，自动进入 BOOT、下载并请求启动。
3. GPIO17 每 500ms 翻转。APP 运行时仍可直接再次点击“开始升级”，无需手动复位。
4. APP 收到 ENTER_BOOT（命令7），完整 ACK 发送后返回 BOOT；APP 不执行擦写，也不回复 BOOT 的 INFO 查询。
5. 修改分区时同步 partition_config.h、链接脚本及上位机地址；详见本次优化说明。

返回路径不再依赖看门狗复位后的 RAM 保留。BOOT→APP 和 APP→BOOT 均经过各自 codestart/_c_int00，重新建立 C 运行环境。保留标记占用 RAM 0x3F8–0x3FB，两个链接文件均避开该区域。后续增加 PWM/其他中断外设时，应在跳转前补齐其关闭流程。

## 固定配置

- 外部 10MHz 晶体，SYSCLK 60MHz。
- SCI：GPIO28 RX / GPIO29 TX，115200、8N1；发送沿用 TI 例程逐字节等待 FIFO 空，修复已验证的发送丢字节问题。
- CAN：GPIO30 RX / GPIO31 TX，500kbit/s，节点1，标准 ID 0x601/0x581。
- APP：字地址 0x3E8000–0x3F1FFF；元数据位于 0x3F2000；BOOT 位于 0x3F4000 起。
- Flash：只链接用户参考 CAN 工程的 2803x_FlashAPI_BootROMSymbols.lib，调用芯片 ROM API。擦写封装函数搬运到 RAM；启动检查 ROM 入口和 RAM 内容。

## 文件结构

Bootloader 与 Application 是两个 CCS 工程；Common 是共享通讯、协议和跳转；Device 是 TI 初始化与头文件；output 包含唯一一套最新 OUT/MAP/TXT；tests 包含本机逻辑测试；tools 包含链接校验和打包工具。

Application/main.c 是应用开发入口。保留 update_service_poll() 的持续调用，不要长时间阻塞主循环。Common/boot_io.c 的 boot_enter_boot() 负责直接返回 BOOT。

## 验证范围

本版完成 TI 编译、ROM 入口与保留 RAM 布局检查，以及 SCI/CAN 的 APP 命令校验、应答后跳转、发送失败重试、返回 BOOT 的 INFO 及重复进入升级逻辑测试。CCS 构建日志见 output。用户已确认此前 CAN/SCI Flash 升级成功；本版直接 APP→BOOT 跳转仍需实机验证，逻辑模拟不能替代芯片执行。

推荐实机验证：升级 APP → LED 翻转 → 进入升级 → 读取设备 → 再升级 → LED 再次翻转。SCI、CAN 分别完成两轮。
