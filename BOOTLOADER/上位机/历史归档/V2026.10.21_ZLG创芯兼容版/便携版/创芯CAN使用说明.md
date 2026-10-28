# CAN盒使用与本地驱动安装

本版保留周立功ZLG CAN盒，新增创芯科技CAN盒。0039、0049、035、00157原有CAN升级共用本适配；335仍使用SCI。BOOT、APP、升级协议和分区不因更换CAN盒而改变。

## 客户怎样使用

1. 保持整个上位机文件夹完整，不要单独复制EXE或DLL。
2. 如果电脑没有安装对应CAN盒的USB驱动，先关闭占用CAN盒的软件，再双击 `安装ZLG驱动.cmd` 或 `安装创芯驱动.cmd`。按Windows管理员授权及原厂安装界面完成安装。安装文件就在本地，不需要联网下载。
3. 按安装工具提示重新插拔USB，必要时重启电脑。驱动安装成功以Windows识别到设备、上位机可以识别和通信为准；安装程序正常退出并不单独证明硬件已可用。
4. 上位机选择CAN并刷新设备。只有一个盒子时自动选中，多个盒子时从列表选择需要的设备及通道；对应DLL由程序自动绑定。
5. 波特率、节点地址、升级文件及APP起始地址仍按原设备要求设置，不因盒子品牌而改变。

## 本地文件

- `drivers/zlg/`、`drivers/zlg-usb/`：原ZLG接口库及USB驱动，保持原有内容。
- `drivers/chuangxin/x64/ControlCAN.dll`：创芯64位接口库。
- `drivers/chuangxin/x86/ControlCAN.dll`：创芯32位接口库，供32位桥接使用。
- `drivers/chuangxin/usb/Setup-V1.40.exe`：原厂USB驱动安装工具。
- `drivers/chuangxin/usb/USB驱动安装与卸载说明书.pdf`：原厂驱动安装说明。
- `安装ZLG驱动.cmd`：调用原ZLG安装实现 `Install-USB-Driver.ps1`。
- `安装创芯驱动.cmd`：调用 `Install-Chuangxin-USB-Driver.ps1`，启动本地创芯原厂安装工具。

接口DLL负责让上位机调用CAN盒；USB驱动负责让Windows识别硬件，两者不同。上位机启动或刷新时不会自动执行USB驱动安装。

## 来源与验证边界

创芯文件来自用户提供的 `CAN分析仪资料20250618`；两种位数的DLL和安装EXE按原文件复制，未修改。当前支持资料中VCI兼容接口的经典CAN功能，不增加CAN FD升级协议。文件哈希见 `CAN驱动文件SHA256.csv`。

本次未在开发电脑执行驱动安装，亦未借安装脚本更改Windows驱动设置。实际型号的USB驱动安装、双品牌设备识别和实板完整升级，需要对应硬件验证；软件检查通过不能替代实盒结果。