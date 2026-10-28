# 协议 v1

这是本项目自定义驻留 Bootloader 协议，与 TI ROM Boot、CANopen 和 ISO-TP 不兼容。上位机与设备必须成套使用。

## 逻辑帧

| 偏移（8 位字节） | 长度 | 含义 |
|---:|---:|---|
| 0 | 2 | A5 5A |
| 2 | 1 | 版本 1 |
| 3 | 1 | 命令；应答置位 0x80 |
| 4 | 4 | session，LE |
| 8 | 4 | sequence，LE |
| 12 | 2 | payload 字节数，LE，0–128 |
| 14 | N | payload |
| 14+N | 4 | CRC32，LE；覆盖偏移 2 至 payload 末尾 |

CRC 为 CRC-32/ISO-HDLC，反射多项式 EDB88320、初值 FFFFFFFF、最终异或 FFFFFFFF；`123456789` 的结果为 CBF43926。

除 TI SCI8 输入文件的地址编码外，本协议多字节整数统一小端。DSP 接收时，一个 `uint16_t` 元素承载一个 8 位 wire octet，禁止将 C28x 结构体原样发送。

## 命令

| 值 | 命令 | 请求 |
|---:|---|---|
| 1 | INFO | 空，session=sequence=0 |
| 2 | BEGIN | 32 字节清单；非零随机 session，sequence=1 |
| 3 | DATA | 4 字节字地址 + 4–60 个 16 位字；数量是 4 的倍数 |
| 4 | FINISH | 空；仅在全部数据写入后提交 |
| 5 | ABORT | 空；同一 session，允许 sequence 大于最后成功序号 |
| 6 | RESET | 空；仅有效镜像状态允许复位 |
| 7 | ENTER_BOOT | 空，session=sequence=0；业务应用响应并通过 RAM 标记复位 |

BEGIN 的八个 LE uint32 字段依次是 DeviceId、BoardId、Cpu、StartWordAddress、WordCount、EntryWordAddress、ImageCRC32、Version。

普通 ACK payload：status(u8)、state(u8)、nextWordAddress(u32)，共 6 字节。INFO 在其后附加 DeviceId(u32)、BoardId(u32)、Cpu(u16)、AppStart(u32)、AppEndExclusive(u32)，共 24 字节。

status：0 OK，1 BadCommand，2 BadLength，3 BadImage，4 BadState，5 BadSequence，6 FlashError，7 VerifyError，8 Busy。

state：0 Idle，1 Receiving，2 Valid，3 Failed。除 ABORT 外数据序号严格递增；上一完整请求完全相同可重放其缓存应答。过期请求、同序号不同内容不重新执行 Flash 操作。

## SCI

8N1，无流控，直接传输逻辑帧。接收支持字节流切片、噪声和 CRC 错误后重同步。半帧超过 200 ms 丢弃。上位机必须等待一条完整应答后再发下一请求。

## 经典 CAN

标准 11 位 ID：请求 `0x600 + node`、应答 `0x580 + node`，node=1–127。每帧 3–8 字节：

| 字节 | 含义 |
|---|---|
| 0 | transferId，u8，每次分包发送递增 |
| 1 | bit7=首帧；bit0–6=分片序号，从 0 开始 |
| 2–7 | 最多 6 字节逻辑帧内容 |

末帧 DLC 为实际长度，无额外 padding。所有分片保持同一 transferId。错序、丢帧、非法 DLC 或间隔超过 200 ms 丢弃当前重组。包 CRC 覆盖整个逻辑帧，完整重组之后才允许执行写入。上位机分片间隔默认 3 ms，避免单接收邮箱被覆盖。

BEGIN 应答超时 120 s，其余普通请求 3 s；最多额外两次重传。设备 Flash 单次 FSM 等待超过 5 s 从 RAM 触发复位；会话 30 s 无有效请求结束。中途断连不支持续传，重新 BEGIN 会从头擦写。

## 元数据

Bank0 扇区15。前 20 个 16 位字保存 magic、DeviceId、BoardId、Cpu、起点、字数、入口、镜像 CRC、版本、头 CRC，均 uint32 小端。最后独立 4 字位于 +20，固定 A55A 5AA5 C33C 3CC3。先编程头，再编程最后一组有效标记；任何启动都验证记录、范围、镜像 CRC 和存储 ECC。

此协议假设一个上位机控制一个设备会话。多节点可以使用不同 CAN ID，但不包含发现、地址分配或多设备同时升级调度。
