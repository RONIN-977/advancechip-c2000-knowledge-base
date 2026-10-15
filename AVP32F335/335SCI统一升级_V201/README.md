# AVP32F335 SCI-A V201 / 448 KB

修正版保留原始包于 `../335sciboot`，本目录包括 BOOT/APP 源码、共用协议/MD5/时钟配置、两套编译结果和验证记录。

| 目录 | 内容 |
|---|---|
| firmware/AVP32F335_Bootloader | A 扇区 V201 BOOT，SCI-A，APP 入口 0x300000 |
| firmware/AVP32F335_APP | H～B APP，末尾保留有效标志，包含进入 BOOT 的 SCI 处理 |
| firmware/common | 共用 SCI、MD5、分区和晶振配置，须与两个工程一起保留 |
| build/30MHz | 30.000 MHz：BOOT 60 MHz / APP 150 MHz |
| build/33.3MHz | 33.300 MHz：BOOT 58.275 MHz / APP 149.85 MHz |
| tests / verification | C 固件算法测试、MAP 审计、便携版自检和界面截图 |

直接运行 `python build-firmware.py` 编译两套；可通过 `--compiler` 指定 TI C2000 编译器。本机已使用 20.2.0.LTS / COFF / FPU32 编译。CCS 项目提供 `30MHz` 和 `33.3MHz` 配置，自动传入对应 `BOARD_OSC_HZ`；CCS IDE 导入未执行，命令行构建已验证。

首次只通过 JTAG 烧对应晶振的 BOOT，然后用统一上位机升级 APP。引脚 GPIO28/RX、GPIO29/TX、共地；115200/8N1。配套 APP OUT 可以直接导入，不能导入旧 0x307FFE 标志的 APP。BOOT 201 版本门禁用于拒绝原 64 KB BOOT；它不检测实际晶振，必须人工选择匹配版本。

详细接线、首次烧录及客户 APP 移植说明见统一上位机 `docs/335-sci.md`（交付便携包中为 `335接线与首次烧录.md`）。APP 正文容量为 458748 字节，不含 0x337FFE～0x337FFF 两个保留字。

软件验证和硬件待验收项见 [验证记录](验证记录.md)。测试 C 代码使用 ARM 仿真执行，替换了 Flash API 和硬件寄存器；它不验证 AVP335 外设、电气或真实 Flash 时序。

```powershell
python -m pip install unicorn==2.1.4 pyelftools==0.32
python tests/run-firmware-tests.py
python tests/verify-maps.py
```

原厂 Library 保留原许可证。新增文件使用 UTF-8；原 Main/Library 的中文注释保留 GBK。

2026-09-29：SCI 非 FIFO 首字节中断启动顺序已修正，四套 OUT 已重新生成。复现及验证可运行 `python tests/run-sci-runtime-tests.py`；实板完整应答待用户复测，详细记录见验证记录。

2026-09-29 CSM 修正：0x70 错误码 1 的初始化误判已修复，两种晶振 BOOT OUT 已重编译。CsmUnlock 成功返回 1，Flash API 成功返回 0。请重烧新的对应 BOOT；APP 可继续使用。通信已获用户确认，完整升级待复测。

## 后续完整流程核验（2026-09-29）

在 CSM 判断修正后，继续修正 RAM 看门狗回调的状态保存：PUSH ST1 / POP ST1 保持 Flash API 调用方 EALLOW/INTM，避免原 EDIS 干扰后续 Flash 控制写。两套 BOOT 已再次编译并同步交付。42 项主机测试、8 组实际 C 回归、C#→实际 C 联测及 SCI 收发/ACK 排空后跳转测试通过；两种真实 APP OUT、最大容量、丢 ACK、取消与重试均覆盖。完整结果见 `完整流程核验.md` 和交付 `SCI验证`；Flash/寄存器用模型、跳转被截获，实板完整升级仍待复测。

## 用户实板反馈及客户精简交付（2026-09-29）

用户在 CSM 成功值及 Flash 回调状态修正后反馈“现在测试没问题”。按当前对话板卡条件，记录为 30.000 MHz 335 实板升级通过；没有单独取得 33.300 MHz、专项断电恢复等项目的确认，不扩大为全部验收项通过。之前的“待复测”条目为历史状态。

2026-09-29 的分发目录当时为知识库根目录 `最终交付_0039c_335`，现归档至 `历史归档/20261008_BOOTLOADER替换/旧交付/最终交付_0039c_335`。当前交付入口为根目录 `BOOTLOADER`，已按 2026-10-08 用户桌面目录完整同步；以下客户目录结构与验收说明属于当时版本。只把 `客户文件` 整个文件夹发给客户，`源码` 为内部保留。客户侧按 `0039c/boot、app` 和 `335/30m、33.3m/boot、app` 放六个 OUT；固件 SHA256 与已提供版本一致。自包含单文件上位机配套 x86 CAN 桥接、驱动及 OUT 转换工具，客户不需安装 .NET/SDK。首次 BOOT 仍由 CCS/JTAG 或已验证烧录工具写入，上位机随后升级 APP。

客户目录共 34 个文件，约 104.4 MiB。已将整包复制到另一路径，在无效 DOTNET_ROOT、PATH 仅保留 Windows System32、全新运行时缓存的条件下完成：自包含启动、0039c OUT 转换、两种真实 335 APP OUT 导入、335 界面仿真、两芯片窗口离屏渲染、x86 CAN 导出/握手和全包 SHA256 核验。没有启动真实 CAN/串口设备，也没有全新 Windows 虚拟机验收。

当前 0039c 原始上位机源码包未包含这对 BOOT/APP 的 MCU 源工程；内部源码目录已明确标注，未混入其他旧协议例程。客户使用所需配套 OUT 已齐全。
