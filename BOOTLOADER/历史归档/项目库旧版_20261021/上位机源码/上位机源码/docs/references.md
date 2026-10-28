# 依赖与参考

TI 本地依赖：

- `firmware/vendor/device`：复制自 `C:\UPS\flashapi_ex1_programming\device`，保留原版权信息。
- `firmware/vendor/FAPI_F28003x_EABI_v1.58.10.lib`：复制自用户原工程。
- `firmware/vendor/FlashAPI`：复制自 `C:\C2000ware\C2000Ware_5_05_00_00\libraries\flash_api\f28003x\include\FlashAPI`。

以上依赖继续适用 TI 自身许可；没有修改其源码。便携版按用户要求打包现有 CANPRO 的 USBCAN DLL、当前系统已安装的 USB 驱动、VC80 私有程序集及 CANPRO 自带运行库安装包，并保留可取得的原许可材料。TI hex2000 保留原二进制及编译工具 manifest 许可文件，仅用于 TI 设备；未修改这些第三方二进制。

官方资料：

- [TMS320F280039C 产品与数据手册](https://www.ti.com/product/TMS320F280039C)
- [F28003x Flash API 参考手册 SPRUJ27](https://www.ti.com/lit/pdf/SPRUJ27)
- [TI CCS ProjectSpec 说明](https://software-dl.ti.com/ccs/esd/documents/ccs_projectspecs.html)
- [周立功 CAN 测试软件与接口函数使用手册](https://www.zlg.cn/data/upload/software/Can/CAN_test_um.pdf)

所实现的协议、Flash 分区、元数据格式和项目板卡标识为本项目设计，并非 TI 或周立功规定的升级协议。
