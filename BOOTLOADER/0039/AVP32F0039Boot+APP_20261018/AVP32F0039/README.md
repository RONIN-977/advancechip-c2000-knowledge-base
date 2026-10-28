# AVP32F0039 BOOT APP 独立工程

BOOT 与 APP 已完全分开，各自有独立头文件与库，不依赖 Common。
可以单独复制、导入和编译任一工程。
分区地址在各自 include/app_partition.h 修改，两份配置需要手动保持一致。
详见 [独立工程修改说明](独立工程修改说明.md)。
