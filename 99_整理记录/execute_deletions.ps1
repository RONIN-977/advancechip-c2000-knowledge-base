param([switch]$Execute)
$ErrorActionPreference = 'Stop'
$workspace = [IO.Path]::GetFullPath((Split-Path -Parent $PSScriptRoot))
$insidePrefix = $workspace.TrimEnd('\') + '\'
$plan = Get-Content -LiteralPath (Join-Path $PSScriptRoot '删除计划.json') -Raw -Encoding utf8 | ConvertFrom-Json
$logPath = Join-Path $PSScriptRoot '已删除资料.jsonl'
function Resolve-SafeFile([string]$relative) {
    if ([IO.Path]::IsPathRooted($relative)) { throw "不能删除绝对路径: $relative" }
    $absolute=[IO.Path]::GetFullPath((Join-Path $workspace $relative))
    if (-not $absolute.StartsWith($insidePrefix,[StringComparison]::OrdinalIgnoreCase)) { throw "路径越界: $absolute" }
    return $absolute
}
foreach ($item in $plan) {
    $target=Resolve-SafeFile $item.path
    if (-not (Test-Path -LiteralPath $target -PathType Leaf)) { throw "文件不存在: $target" }
    if ((Get-FileHash -LiteralPath $target -Algorithm SHA256).Hash.ToLowerInvariant() -ne $item.sha256) { throw "文件已变化: $target" }
}
if (-not $Execute) { Write-Output "删除计划检查通过，共 $($plan.Count) 个文件。"; return }
if (Test-Path -LiteralPath $logPath) { throw '已有删除记录；拒绝重复执行。' }
foreach ($item in $plan) {
    $target=Resolve-SafeFile $item.path
    $item | ConvertTo-Json -Compress | Add-Content -LiteralPath $logPath -Encoding utf8
    Remove-Item -LiteralPath $target -Force
    if (Test-Path -LiteralPath $target) { throw "删除失败: $target" }
}
Write-Output "已按记录删除 $($plan.Count) 个重复、临时或空文件。"
