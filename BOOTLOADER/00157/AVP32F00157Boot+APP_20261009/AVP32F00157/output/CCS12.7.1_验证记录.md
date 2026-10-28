# 2026-10-09 独立工程版验证

- 实际交付目录：`C:\Users\17879\Desktop\CHIP\BOOTLOADER\AVP32F00157Boot+APP_20261009\AVP32F00157`。
- 使用本机 `C:/ti/ccs1271/ccs/eclipse/eclipsec.exe`，CCS 12.7.1；C2000 编译器 22.6.1.LTS。
- 两型号共 4 个工程实际导入全新 CCS 工作区，Debug full build：4/4 通过，0 错误，0 警告。原路径中文目录直接编译通过。
- 本型号默认地址 `0x088000`；output OUT/MAP 从本次原路径 CCS Debug 构建复制，APP SCI8 TXT 用同版本 hex2000 导出。
- 本型号整体复制到独立验证目录，只修改共享头文件一行至 `0x08A000`，BOOT/APP 均通过同编译器构建；入口地址、Flash 边界及 RAM 函数/API 位置校验通过。交付版本已保留默认地址。
- 原版 Common、Device 中所有源码/头文件/库以及各工程根目录 C/H/ASM/CMD 与备份逐文件按字节比对一致；只调整目录、工程引用、编译器版本、构建/测试/校验/打包路径和说明。
- 每个工程仅一个外部链接：`PARENT-1-PROJECT_LOC/Common/app_partition.h`；其余源码和库为工程目录内实体文件。
- 验证旧有链接检查脚本，035 ROM API 库哈希及绝对入口保持一致，00157 Flash API/RAM 搬运范围通过。
- 本次没有重新运行历史上位机协议仿真或进行烧录、SCI/CAN 实板升级测试。

详细全量编译日志：`ccs1271-full-build-20261009.log`。
原始工程和同名 ZIP 备份：`C:\Users\17879\Documents\ChatGPT\进芯替代TI C2000系列芯片信息\Development_Projects\ccs1271_035_00157_20261009\backup`。
