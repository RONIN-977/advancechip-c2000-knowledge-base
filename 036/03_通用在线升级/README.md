# 036 在线升级工程入口

用户确认 [LIN 新 BOOT](<LIN_新版/036LINBOOT+APP_VF_20260615/036LINBOOT+APP_VF/ADP32F03x_LINBoot036>) 与 [CAN 新 BOOT](<CAN_新版/CANBootloaderDemo/CANBootloaderDemo/ADP32F036_CANBOOT036_V1.02>) 都是高、低压方案通用的 BOOT。新包内分别带 VF 应用示例与配套上位机；历史区保留旧 LIN APP、AutoACDrive CAN APP 和旧工具，供移植参考。旧 LIN/CAN BOOT 已按用户确认删除。

通用指 BOOT 适用范围，不代表历史 APP 已与新版 BOOT 配对验证。集成时逐项核对 Flash 分区、APP 入口、帧格式、校验、链接文件、上位机版本和目标板外设；不得混配旧 BOOT/APP 习惯参数。保留工程还未同步进芯 [V1.1 核查修改指南](<../00_规范与勘误/手工同步清单.md>)，其中多处仍为 Flash `3/3/5`，必须先手工修订并实测。
