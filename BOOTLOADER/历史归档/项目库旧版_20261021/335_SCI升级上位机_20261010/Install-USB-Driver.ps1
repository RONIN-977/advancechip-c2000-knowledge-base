$ErrorActionPreference = 'Stop'
$inf = Join-Path $PSScriptRoot 'drivers\zlg-usb\usbcan.inf'
if (!(Test-Path -LiteralPath $inf)) { throw "USB driver not found: $inf" }
# Runs only when explicitly launched by the user, never on application startup.
$result = Start-Process -FilePath "$env:SystemRoot\System32\pnputil.exe" -ArgumentList @('/add-driver', ('"' + $inf + '"'), '/install') -Verb RunAs -WindowStyle Hidden -Wait -PassThru
if ($result.ExitCode -notin @(0,3010)) { throw "USB driver installation failed: $($result.ExitCode). Check Windows driver signature compatibility." }
Write-Host 'USB driver installed. Reconnect the CAN adapter. Exit code:' $result.ExitCode
