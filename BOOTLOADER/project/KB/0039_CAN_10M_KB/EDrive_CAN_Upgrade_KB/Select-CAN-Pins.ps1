param([ValidateSet(12,30)][int]$Pair=12)
$ErrorActionPreference='Stop'
$files=@('APP_KB/CAN_Upgrade/kb_can_board.h','BOOT_KB/include/kb_can_board.h')
foreach($relative in $files) {
    $path=Join-Path $PSScriptRoot $relative
    $text=[IO.File]::ReadAllText($path)
    $text=[regex]::Replace($text,'#define KB_CAN_PIN_PAIR (12|30)U',"#define KB_CAN_PIN_PAIR $($Pair)U")
    [IO.File]::WriteAllText($path,$text,[Text.UTF8Encoding]::new($false))
}
Write-Host "CAN pins selected: $Pair / $($Pair+1). Rebuild BOTH projects."
