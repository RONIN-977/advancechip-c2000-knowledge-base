$ErrorActionPreference = 'Stop'
# Runs only when the customer explicitly launches the installation CMD.
$installer = Join-Path $PSScriptRoot 'drivers\chuangxin\usb\Setup-V1.40.exe'
if (!(Test-Path -LiteralPath $installer -PathType Leaf)) {
    throw "Chuangxin USB driver installer not found: $installer"
}
Write-Host 'Close CAN applications before continuing. Approve the Windows administrator prompt and follow the original installer.'
$result = Start-Process -FilePath $installer -WorkingDirectory (Split-Path -Parent $installer) -Verb RunAs -Wait -PassThru
if ($result.ExitCode -notin @(0, 3010)) {
    throw "Chuangxin driver installer exited with code $($result.ExitCode). Installation may be incomplete."
}
Write-Host 'Installer finished. Reconnect the CAN adapter, then refresh devices in the updater.'
if ($result.ExitCode -eq 3010) { Write-Host 'Windows restart is required.' }