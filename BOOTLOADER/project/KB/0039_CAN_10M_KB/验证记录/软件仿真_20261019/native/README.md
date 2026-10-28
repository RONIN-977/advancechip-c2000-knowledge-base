# 0039 KB 本机 C 协议模型

这不是 C28x 指令集、CCS JTAG 或物理 CAN 仿真。

真实执行的最新桌面源文件：
- BOOT_KB/src/boot_core.c（直接编译链接）
- APP_KB/src/upgrade_entry.c（直接编译链接，包括 dispatch）
- APP_KB/src/boot_io.c 内的 can_wait_tx、boot_send_reply（脚本原样提取）

Flash 擦除/编程/读回 CRC/最后提交标志使用 RAM 模型，模拟四字对齐、地址边界、重复编程防护及失败。CAN 驱动、计时、复位使用适配层；请求经真实 bl_can_feed 分片重组，APP 经真实 upgrade_entry_poll 进入 dispatch，应答经真实 boot_send_reply 分片后再由真实 bl_can_feed 重组。

`source_hashes.json` 记录源文件 SHA256，`compile_command.json` 记录构建命令。原桌面工程未写入。

接口：
- native_reset：回到 APP，清零测试统计和 RAM Flash
- native_exchange：transport 仅接受 2；输入完整协议 wire packet，但模型内部逐 CAN 帧送入；返回完整应答长度
- native_read_word：按 C28x 字地址读取 RAM Flash 模型
- native_fault：0正常；1擦除失败；2编程失败；3CRC验证失败；4提交失败；5发送忙超时；6丢请求第2片；7交换请求第2/3片
- native_stat：0 APP=0/BOOT=1；1擦除次数；2编程次数；3提交次数；4有效标志；5复位次数；6已写字数；7当前manifest长度的Flash CRC；8接收CAN帧数；9发送CAN帧数；10BOOT协议状态

不能验证：10 MHz 晶振起振/PLL、CAN 位时序、电气、邮箱覆盖和真实中断竞争、Flash API RAM 执行及硬件擦写/ECC、复位 RAM 保持、实际启动跳转与5秒窗口。上述仍需板级验证。
