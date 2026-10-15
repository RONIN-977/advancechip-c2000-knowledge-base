> 独立工程版目录说明：本文中的 Common 源码、Device 文件现位于各自 Bootloader / Application 内；只有 Common/app_partition.h 仍在型号根目录共享。普通 APP 起点只改该共享文件，旧版多处修改步骤不适用。

# Flash API 来源与链接要求

库来自用户提供且已验证的 C:/UPS/ADP32F035_CAN/035/ADP32F035_BOOT_CAN/Library/LIB/2803x_FlashAPI_BootROMSymbols.lib。
SHA256：0F01A9C5F2BBF4D5DDCD710B586DCE7354097F2F808FD099E252253AD28656FC。

API 在芯片 ROM 执行，Program=0x3FF8D2、Erase=0x3FF9B3、Verify=0x3FFDD5，与参考工程 MAP 一致。Flash_CPUScaleFactor=0xD04、Flash_CallbackPtr=0xD02。擦写封装函数通过 ramfuncs 搬运到 RAM，启动逐字比对并检查 ROM API 入口。

不要加入 Flash2803x_API_V100.lib，当前工程只使用 ROM 符号库。对 CCS 编译结果可运行 python tools/verify.py Bootloader/Debug/ADP32F035_Bootloader.map。

boot_debug_flash_status：0xFFFC 表示 ROM 入口不符，0xFFFB 表示 CSM 解锁失败，0xFFFD 表示 RAM 搬运失败；其他值由 Flash API 返回。默认未加密器件的 CSM 密钥为 0xFFFF，不修改 Flash 密码。
