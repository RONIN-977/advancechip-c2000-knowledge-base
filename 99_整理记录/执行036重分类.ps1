$ErrorActionPreference = 'Stop'
$root = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$prefix = $root.TrimEnd('\') + '\'
$plan = Get-Content -LiteralPath (Join-Path $PSScriptRoot '036_路径迁移.json') -Raw -Encoding UTF8 | ConvertFrom-Json

function InWorkspace([string]$relativePath) {
    $full = [IO.Path]::GetFullPath((Join-Path $root ($relativePath -replace '/', '\')))
    if (-not $full.StartsWith($prefix, [StringComparison]::OrdinalIgnoreCase)) {
        throw "路径越界：$relativePath"
    }
    return $full
}

$moves = @($plan | Sort-Object { ($_.from -split '/').Count } -Descending | ForEach-Object {
    [pscustomobject]@{ From = $_.from; To = $_.to; Source = (InWorkspace $_.from); Target = (InWorkspace $_.to) }
})

foreach ($m in $moves) {
    if (-not (Test-Path -LiteralPath $m.Source)) { throw "源不存在：$($m.From)" }
    if (Test-Path -LiteralPath $m.Target) { throw "目标已存在：$($m.To)" }
}

foreach ($m in $moves) {
    $parent = Split-Path -Path $m.Target -Parent
    if (-not (Test-Path -LiteralPath $parent)) { New-Item -Path $parent -ItemType Directory -Force | Out-Null }
    Move-Item -LiteralPath $m.Source -Destination $m.Target
    if ((Test-Path -LiteralPath $m.Source) -or -not (Test-Path -LiteralPath $m.Target)) {
        throw "移动后校验失败：$($m.From)"
    }
}

$moves | Select-Object From,To | ConvertTo-Json -Depth 3 | Set-Content -LiteralPath (Join-Path $PSScriptRoot '036_移动执行.json') -Encoding UTF8
Write-Output "moved=$($moves.Count)"
