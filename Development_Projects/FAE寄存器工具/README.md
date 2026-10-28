# FAE 寄存器与嵌入式计算工具

Windows 10/11 x64 离线便携工具。四个显示型号为 ADP32F035、AVP32F335、AVP32F0039、AVP32F0049，分别以 TI F28035、F28335、F280039C、F280049C 的寄存器和接口资料为基线。资料来源、缺项和进芯差异须结合程序中的说明及资料覆盖清单使用。

## 运行

打开交付包的“便携版”，双击 `FAE寄存器工具.exe`。移动时复制整个文件夹，不要只复制 EXE；不用安装 .NET、不用联网。设置存放于当前用户的 `%LOCALAPPDATA%\FaeRegisterTool`。便携版与源码分别交付。

六个独立入口：寄存器查询、CAN 波特率、PWM 频率、ADC 换算、地址计算、程序员计算器。寄存器资料见 `Data/registers.json`，原始 TI 手册与来源文件见 `docs/sources`。

## 构建

开发需要 Windows 和 .NET 10 SDK。使用 `powershell -ExecutionPolicy Bypass -File .\build.ps1 -Publish` 构建、自测并发布；使用 `powershell -ExecutionPolicy Bypass -File .\package.ps1` 生成含便携版、源码和 SHA256 清单的独立交付目录。

可用 `-Dotnet 'SDK路径\dotnet.exe' -Packages 'NuGet缓存路径'` 指定构建工具和缓存。这些是开发依赖，运行便携版不需要。默认优先使用本仓库已存在的 `tmp/dotnet-sdk` 和 `tmp/nuget`，不将它们的绝对路径写入程序。新电脑没有缓存时，发布步骤从 NuGet 官方获取自包含运行时。

缓存和SDK齐全时可加 `-Offline`，构建过程只使用本地依赖，不查询网络包源或漏洞数据库。离线构建不表示进行过在线漏洞审计。

代码结构：`src/Fae.Core` 为独立计算引擎与模板；`src/Fae.Desktop` 为 WPF 界面；`tests/Fae.Tests` 为无需测试框架包的自动测试；`scripts` 为资料生成和模板验证；`docs` 保存覆盖清单、来源和验证记录。

仅在重建寄存器资料时需要 Python 3 与 `scripts/requirements-data.txt` 中的 pypdf；正常构建EXE及运行工具不依赖Python。资料校验：`python scripts/verify-data.py`；地址交叉审计：`python scripts/audit-addresses.py`。CCS片段编译：`powershell -File scripts/verify-ccs.ps1 -C2000Ware <固定版本目录> -Compiler <cl2000.exe路径>`，不会修改客户工程。

## 验证边界

数值计算测试、TI CCS 编译、Windows 程序启动和实板测试分别记录，不互相替代。工具不连接芯片、不烧录、不改写客户工程。默认代码为配置片段，接入工程前需满足显示的时钟及头文件前提。

TI 资料和头文件保留原版权与许可，仅作为来源核对和用户内部使用材料；本项目不公开发布，不宣称 TI 基线已经完成进芯实板验证。
