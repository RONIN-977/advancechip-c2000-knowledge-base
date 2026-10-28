# AVP32F00157 独立工程使用说明

目录结构：

```text
AVP32F00157/
  Common/app_partition.h     唯一共享的地址配置
  Bootloader/
    main.c、bootloader.cmd、Flash 擦写代码
    Common/                 BOOT 自己的通信及升级源码
    Device/                 BOOT 自己的头文件、初始化源码和库
    .project / .cproject / .ccsproject
  Application/
    main.c、application.cmd
    Common/                 APP 自己的通信及返回 BOOT 源码
    Device/                 APP 自己的头文件、初始化源码和库
    .project / .cproject / .ccsproject
  output/                   默认地址下的 OUT、MAP、APP SCI8 TXT
```

1. 完整解压，在 CCS **12.7.1** 的 Import CCS Projects 中选择两个工程。不要勾选 Copy projects into workspace；共享地址文件需要保留上述相对位置。可以只导入其中一个工程，但不要只搬走工程子目录。
2. 默认编译器为 **TI C2000 22.6.1.LTS**（CCS 12.7.1 自带）。选择 Debug，Clean / Rebuild。
3. 地址修改入口保持 `Common/app_partition.h`，默认 `0x088000`，按 `0x400 WORD` 对齐。两个工程树中的 app_partition.h 指向这一份真实文件。
4. 改地址后重新编译 BOOT 和 APP，用 JTAG 下载新 BOOT；上位机填写相同 APP 起点，加载对应新 APP OUT。链接文件、擦除范围、设备返回的地址按共享头文件配置生成，无需手动改 .cmd。
5. 各工程 Common、Device 是实体文件，不再引用另一个工程或外层源码/库。修改 APP 功能使用 Application/main.c 及其 Common；修改 BOOT 使用 Bootloader 内文件。地址以外的公共代码现在是两份独立副本，需同步变更时分别修改。

编译/链接的包含与库搜索路径使用相对路径，便于在当前中文目录及整体搬迁后编译。不得只复制一个工程到另一个 workspace 根目录而丢失外层共享头文件；如需迁移，整体复制型号目录。

命令行也可在本目录运行 `pwsh -File build.ps1`，默认调用 CCS 12.7.1 的 22.6.1 编译器。该脚本重新生成 output；CCS Build 生成的调试文件则在各工程 Debug 内。

本次只整理工程依赖和编译配置；时钟、GPIO、通信协议、升级状态机、Flash API 库及分区默认值均保持原版。CCS 验证记录见 output/CCS12.7.1_验证记录.md。本次没有烧录和实板测试。

