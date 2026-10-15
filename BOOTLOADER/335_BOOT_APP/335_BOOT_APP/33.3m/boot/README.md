# AVP32F335_BOOT_33_3m

固定晶振 33.300 MHz，BOOT 工程。Main/Library 包含工程源码，公共地址配置位于 ../../UpgradeLayout.h；复制或交付时请保留整个 335_BOOT_APP 目录。

CCS 导入当前文件夹，工程名与其余三套不同；默认 Debug 配置已设置对应晶振宏。本机已验证 TI C2000 20.2.0.LTS 命令行编译，以及 CCS 12.7.1 工程导入和完整构建（0 错误）；实板待测。

输出在 `Debug/AVP32F335_BOOT_33.3MHz.out`，同目录含 MAP 和编译日志。

命令行重新编译：`python build.py --compiler C:\ti\ti-cgt-c2000_20.2.0.LTS`。

首次用 JTAG 烧录本 BOOT V201；随后由统一上位机升级同晶振 APP。

CCS 操作：Project → Import CCS Projects，选择上级 `335固件` 并搜索子文件夹；勾选本工程，保持 `Copy projects into workspace` 不勾选，导入后使用 Debug 配置执行 Rebuild Project。

2026-09-29：修正 SCI 首次发送中断启动顺序；本目录 OUT 已重编译，请使用本次新的 BOOT/APP。

2026-09-29：修正 CsmUnlock 成功值误判导致 0x70 返回 1；当前 OUT 已重新生成，请重新烧录本 BOOT。

本次最新 OUT 同时包含 CSM 成功值修正、Flash API 看门狗回调状态保存修正；完整核验见交付根目录 `完整流程核验.md`。

2026-10-09：APP 分区集中配置在 ../../UpgradeLayout.h；必须连同整个 335_BOOT_APP 目录保留公共头文件，不能仅复制单个 boot/app 文件夹。BOOT、APP 及 APP 链接文件均使用同一配置。
