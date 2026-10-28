> 独立工程版目录说明：本文中的 Common 源码、Device 文件现位于各自 Bootloader / Application 内；只有 Common/app_partition.h 仍在型号根目录共享。普通 APP 起点只改该共享文件，旧版多处修改步骤不适用。

# 当前版验证记录

- 用户已确认此前 ROM API 版本的 CAN、SCI 实机升级正常。
- 本版 BOOT/APP 经 TI 编译器重新构建，改名后的两个工程经 CCS 实际导入构建成功。
- CLI 与 CCS BOOT MAP 均通过 ROM API 地址、参数地址、保留 RAM 和 ramfuncs 布局检查。
- SCI FIFO/非 FIFO 接收、错误恢复和发送排空逻辑测试通过。
- APP SCI/CAN 测试通过：无效 CRC/会话请求不得跳转；应答成功后才请求进入 BOOT；发送失败允许重试；模拟 BOOT 初始化后响应 INFO 并保持驻留；再次运行 APP 后可再次请求升级。
- 现有上位机 23 项协议回归通过，输入为改名后新构建的 APP OUT。

本版 APP 采用直接进入 BOOT codestart 的软件跳转，尚需目标板确认。实际外设关闭、C 启动重建及 RAM 标记保留由板上两轮连续升级验证；本机 mock 测试不代表已经验证硬件跳转。

日志位于 output/build.log、ccs-bootloader.log、ccs-application.log、native-tests.log、host-tests.log。
