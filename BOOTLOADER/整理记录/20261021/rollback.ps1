$ErrorActionPreference = 'Stop'
$prefix = 'C:\Users\17879\Desktop\CHIP\BOOTLOADER\'
$journal = Join-Path $PSScriptRoot 'move-journal.json'
if (!(Test-Path -LiteralPath $journal)) { throw 'Move journal missing.' }
$steps = @(Get-Content -LiteralPath $journal -Raw -Encoding UTF8 | ConvertFrom-Json)
[array]::Reverse($steps)
if (Get-Process -Name ccstudio,eclipsec -ErrorAction SilentlyContinue) { throw 'Close CCS before rollback.' }
foreach ($step in $steps) {
    foreach ($path in @($step.source,$step.target)) {
        if (![IO.Path]::GetFullPath($path).StartsWith($prefix,[StringComparison]::OrdinalIgnoreCase)) { throw 'Unexpected rollback path' }
    }
    if (!(Test-Path -LiteralPath $step.target)) { throw "Rollback data missing: $($step.target)" }
    $entry = Get-Item -LiteralPath $step.source -Force -ErrorAction SilentlyContinue
    if ($entry) {
        if ($entry.LinkType -ne 'Junction') { throw "Refuse overwrite original: $($step.source)" }
        # Remove only the junction entry, never recursively delete the target.
        Remove-Item -LiteralPath $step.source -Force
    }
    New-Item -ItemType Directory -Path (Split-Path -Parent $step.source) -Force | Out-Null
    Move-Item -LiteralPath $step.target -Destination $step.source
    Write-Output ('Restored: ' + $step.source)
}
