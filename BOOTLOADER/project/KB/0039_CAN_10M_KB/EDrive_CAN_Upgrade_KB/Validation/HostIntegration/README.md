# 配套 BOOT / 客户 APP 上位机软件联测

本次执行真实上位机 Core 源码、真实新移植 APP 升级适配器和配套 BOOT 协议核心的联测。**不是上板测试，也没有执行 GUI / USB-CAN 驱动。**

## 结果

- 9 / 9 场景通过：完整升级、BEGIN/DATA/FINISH 应答丢失重试、取消后恢复、错误分区拒绝、APP 进入请求应答未发完不调用停机也不复位、Flash 模型的擦除/编程/校验/提交四类失败。
- 最终默认 GPIO12/13 配置 APP OUT SHA256：`5D2E27AC93EB95F44BE781D87AFC30A2428BEA74272C93058A158FCA4B7C69D4`。
- 镜像入口和起点：`0x087000`，8792 个 16 位字（17584 字节），CRC32：`0x0140DCA0`。
- 正常场景：擦除 1 次、编程 147 次、有效提交 1 次、复位 2 次（APP → BOOT、BOOT → APP），逐字读回全部一致。
- 真实空函数 `Customer_ShutdownForUpgrade()` 被调用 1 次。编译器函数入口探针验证调用发生于完整应答四帧发送完成之后，且升级任务尚未额外屏蔽全局中断；没有替换成成功桩或在交付源码中插入探针。

## 执行链与代码来源

`UpdaterClient.EnterBootAsync` → `CanFraming.Fragment` → 真实 `KB_CAN_UpgradeTask` / `boot_core.c` → 真实空停机函数 → 模拟复位 → 配套 BOOT `boot_core.c` → 实际 `boot_io.c` 提取的原版 CAN 应答分片函数 → `CanReassembler` → `UpdaterClient.UpdateAsync` → 真实新 `.out` 全包发送 → 模型 Flash 逐字读回/CRC/提交 → `ResetAsync`。

上位机源码直接取自当前桌面的：
`C:\Users\17879\Desktop\CHIP\BOOTLOADER\上位机源码\上位机源码\host\Updater.Core`。
11 个 C# 文件逐份 SHA256 记录于 `source_hashes.json`，并核对与前次仿真快照全部一致，详见 `host_source_comparison.json`。未证明某个已发布 EXE 与这套源码的二进制对应关系。

模型直接编译交付的 `kb_can_upgrade.c`、`kb_customer_shutdown.c`、`boot_core.c`，未改正文；APP 与 BOOT 的核心 C 文件、分区头逐字节相同。唯一配置替换是将 RAM 标记地址宏映射到测试数组。Timer/CAN/中断/寄存器声明由桩头提供。完整源哈希在运行前后复核一致。

使用 TI `hex2000.exe` 和上位机原 `TiOutConverter` / `TiBootTable` 解析最终 APP OUT。测试上位机参数使用 `DeviceProfile.F280039C.WithAppStart(0x087000)`，对应 GUI 的 APP 起始地址输入。**实际使用上位机也必须填 `0x087000`；默认 `0x090000` 不适配本工程，软件会在解析或设备分区检查阶段拒绝，未擦除。**

## 模型边界

CAN 硬件寄存器、消息对象、ACK 完成、Timer2、IRQ 和 watchdog reset 均为模型。BOOT Flash 操作回调为 RAM 数组模型；没有调用真实 `flash_port.c` / Flash API，没有验证 ECC、忙时执行、Flash 等待周期、电源或保留 RAM。BOOT 元数据地址 `0x084000` 来自真实分区头，但提交存储只以模型标志表示，不验证真实元数据格式或写入。

复位后的 APP/BOOT 选择由模型状态表示，未执行 BOOT `main.c` 或真实 C28x 启动、跳转和新下载的 APP 指令。10MHz 晶振、100MHz 系统时钟、500k 物理 CAN、板级停机逻辑及硬件在线升级仍需实板确认。

`results.json` / 各场景 JSON、LOG 保存执行结果；`native-build.log`、`host-build.log` 保存构建记录。`run_integration.py` 可复跑，所有输出和工具缓存写入本目录。
