# Local dependency sources

- Base CCS project/device code/driverlib: user supplied Desktop/V12/sci_ex3_echoback. Original files left unchanged.
- SysConfig product data: C2000Ware 5.04.00.00. All .meta trees plus .metadata/sdk.json are bundled under vendor/c2000ware. The SDK manifest is unmodified; no external C2000Ware installation is required to generate this SCI configuration.
- TI license and software manifest are in vendor/c2000ware/license.txt and C2000Ware_5.04.00.00_manifest.html. Source copyright notices retained.
- CCS 12.7.1, C2000 compiler 22.6.1.LTS and SysConfig 1.23.1 remain host development-tool prerequisites. SysConfig 1.20 is not suitable for this SDK migrationProducts manifest field.
- .project/.cproject use project-relative paths. vendor and the bundled driverlib implementation are excluded from automatic source compilation; driverlib is linked from device/driverlib/ccs/Debug/driverlib.lib.
- Four original configurations retained: CPU1_RAM, CPU1_FLASH, CPU1_LAUNCHXL_RAM, CPU1_LAUNCHXL_FLASH. The two non-LaunchPad configurations preserve the original 20 MHz crystal device setup; LaunchPad macros select their original board clock setup.
- Generating/loading this project does not prove hardware operation or full-temperature autobaud behavior.
