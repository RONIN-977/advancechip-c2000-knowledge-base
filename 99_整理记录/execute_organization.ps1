param([switch]$Execute)
$ErrorActionPreference = 'Stop'
$workspace = [IO.Path]::GetFullPath((Split-Path -Parent $PSScriptRoot))
$insidePrefix = $workspace.TrimEnd('\') + '\'
$planPath = Join-Path $PSScriptRoot '分类移动计划.json'
$logPath = Join-Path $PSScriptRoot '已执行移动.jsonl'
$plan = Get-Content -LiteralPath $planPath -Raw -Encoding utf8 | ConvertFrom-Json

function Resolve-SafePath([string]$relative) {
    if ([IO.Path]::IsPathRooted($relative)) { throw "计划不能使用绝对路径: $relative" }
    $absolute = [IO.Path]::GetFullPath((Join-Path $workspace $relative))
    if (-not $absolute.StartsWith($insidePrefix, [StringComparison]::OrdinalIgnoreCase)) { throw "路径越界: $absolute" }
    return $absolute
}

# Validate the complete plan before creating directories or moving files.
foreach ($item in $plan) {
    $src = Resolve-SafePath $item.from
    $dst = Resolve-SafePath $item.to
    if (-not (Test-Path -LiteralPath $src -PathType Leaf)) { throw "源文件不存在: $src" }
    if (Test-Path -LiteralPath $dst) { throw "拒绝覆盖: $dst" }
    if ((Get-FileHash -LiteralPath $src -Algorithm SHA256).Hash.ToLowerInvariant() -ne $item.sha256) { throw "原文件已变化: $src" }
}
if (-not $Execute) { Write-Output "计划检查通过，共 $($plan.Count) 个独立资料文件；加 -Execute 执行。"; return }
if (Test-Path -LiteralPath $logPath) { throw '已有执行记录；请先检查，避免重复执行。' }
foreach ($item in $plan) {
    $src = Resolve-SafePath $item.from
    $dst = Resolve-SafePath $item.to
    $parent = Split-Path -Parent $dst
    [IO.Directory]::CreateDirectory($parent) | Out-Null
    Move-Item -LiteralPath $src -Destination $dst -ErrorAction Stop
    $item | ConvertTo-Json -Compress | Add-Content -LiteralPath $logPath -Encoding utf8
    if ((Get-FileHash -LiteralPath $dst -Algorithm SHA256).Hash.ToLowerInvariant() -ne $item.sha256) { throw "移动后校验失败: $dst" }
}
Write-Output "已移动并逐个校验 $($plan.Count) 个资料文件；工程目录、压缩包及原文件内容未改动。"
