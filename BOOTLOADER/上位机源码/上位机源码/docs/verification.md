# 软件验证记录

日期：2026-09-28。
CCS 工程依赖整理验证更新：2026-09-29。
便携发布及窗口适配验证更新：2026-09-29。

| 项目 | 结果 |
|---|---|
| .NET 10.0.300 / WPF Release 构建 | 通过，0 警告、0 错误 |
| 主机协议/仿真/OUT 导入测试 | 22 项通过 |
| OUT 自动转换 | 使用本机 TI hex2000；自动发现工具后完成 CAN 仿真升级；转换镜像与原 TXT 逐字相同；中文/空格路径、错误文件、Bootloader 拒绝、缺失工具和取消检查通过 |
| TI C2000 25.11.1.LTS C28x EABI 编译与链接 | Bootloader、应用均生成 .out |
| CAN 引脚更新 | RX GPIO30 / TX GPIO31，使用 GPIO_30_CANA_RX / GPIO_31_CANA_TX；Bootloader 和示例应用重新编译、链接及 hex 导出通过 |
| 固件排版及注释 | 11 个自编写 C/H 文件通过 clang-format 18.1.8 格式检查；TI vendor 源码保持原样 |
| Bootloader 行为保持 | 整理前后 ELF 可加载段 codestart、.text、.cinit、.const、.TI.ramfunc 逐字节一致 |
| GPIO17 LED 应用 | 用户已验收应用跳转成功、指示灯闪烁正常 |
| hex2000 --boot --sci8 --ascii | 导出成功，实际文件已通过主机导入测试 |
| Bootloader / 应用入口 | map 确认为 0x080000 / 0x090000 |
| Flash API / 擦写函数 RAM RUN 地址 | map 已核查，位于 RAMLS 范围 |
| 两份 ProjectSpec XML | 使用本机 TI ProjectSpec XSD 校验通过 |
| CCS 精简依赖导入 | 按 ProjectSpec 逐文件复制到独立目录并编译链接通过；Bootloader 6 个源文件/2 个库，Application 6 个源文件/1 个库；无整目录排除或 driverlib 源码重复导入 |
| WPF 布局 | 已离屏渲染并检查文字对比度、布局 |
| 便携版 | win-x64 上位机及 win-x86 桥接自带 .NET 10 运行时；设置无效 DOTNET_ROOT 后直接启动 EXE，自检确认 CLR、CAN DLL、hex2000 来自包内 |
| 窗口缩放 | 显式 CanResizeWithGrip，最小 640×480，工作区适配和垂直滚动；1080×800 与 640×480 离屏布局检查 |
| CANPRO 依赖 | 原 5.1.0.1 DLL、VC80 CRT/MFC 私有程序集、备用运行库安装程序随包；Windows 可优先加载已安装的 VC80 系统版本，尚未在全新 Windows 环境验收 |
| USB 系统驱动 | 随附本机 DriverStore 的 usbcan INF/SYS/CAT 等文件与手动安装脚本；未执行安装或更改系统驱动 |
| CANPRO x86 驱动关联 | 自动填入 C:\ZLGCANPRO\ZCANPRO\kerneldlls\usbcan.dll，UI 渲染确认 |
| x86 桥接与原厂驱动 | .NET Framework x86 编译成功；5.1.0.1 驱动加载、7 个 VCI 导出解析、二进制握手和 EOF 退出通过；未打开设备 |
| CCS12 IDE 导入/22.6.1 编译 | 未执行，本机可用 IDE 为 CCS21 |
| 实际 COM / USBCAN 收发 | 未执行 |
| 用户实板反馈 | F280039C 开发板 + CAN 盒烧录、应用跳转、GPIO17 闪灯已由用户验收；不等同于断电或全部接口验收 |
| Flash 擦写、上电启动、RAM 标记复位进入升级 | 待实板验证 |
| 随机断电与总线故障恢复 | 待实板验证 |

C 核心由 TI 编译器编译；22 项自动测试针对 C# 协议、设备模型和实际 hex2000 转换，实板结果单独记录为用户验收反馈。输出 map 中将 `code_start` 用作入口而非 `_c_int00` 的提示符合本工程启动设计。

## 统一版本新增验证（2026-09-29）

原 F280039C 22 项回归与新增 335 20 项主机测试全部通过，共 42 项。335 共用 C 协议/MD5 的 7 组算法测试、四个 TI C2000 COFF OUT/MAP、便携版 x64/x86 运行时和驱动自检、四个布局渲染通过；335 实板尚未执行。详细证据、编译器版本与待测表见知识库 AVP32F335/335SCI统一升级_V201/验证记录.md。本次源码未修改原 F280039C 协议、分区和 CAN 通信代码。
