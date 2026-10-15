$ErrorActionPreference = 'Stop'
$root = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$prefix = $root.TrimEnd('\') + '\'
$folders = @(
    '036/01_器件资料/ADM32F036A2',
    '036/01_器件资料/ADM32F036A3',
    '036/01_器件资料/ADP32F036',
    '036/04_基础例程/ADP32F036_Example_20241029',
    '036/04_基础例程/历史差异例程'
)
$moves = New-Object 'System.Collections.Generic.List[object]'
foreach ($relative in $folders) {
    $sourceFolder = [IO.Path]::GetFullPath((Join-Path $root ($relative -replace '/', '\')))
    if (-not $sourceFolder.StartsWith($prefix, [StringComparison]::OrdinalIgnoreCase)) { throw "源目录越界：$relative" }
    $label = Split-Path -Path $sourceFolder -Leaf
    if ($label -eq '历史差异例程') { $label = '历史差异例程' }
    $destFolder = [IO.Path]::GetFullPath((Join-Path $root "036/99_原始发布包/来自$label"))
    if (-not $destFolder.StartsWith($prefix, [StringComparison]::OrdinalIgnoreCase)) { throw "目标目录越界：$destFolder" }
    foreach ($file in (Get-ChildItem -LiteralPath $sourceFolder -File | Where-Object { $_.Extension -in @('.zip','.rar','.7z') })) {
        $dest = Join-Path $destFolder $file.Name
        if (Test-Path -LiteralPath $dest) { throw "目标已存在：$dest" }
        $moves.Add([pscustomobject]@{ From=$file.FullName.Substring($root.Length + 1).Replace('\','/'); To=$dest.Substring($root.Length + 1).Replace('\','/'); Source=$file.FullName; Target=$dest; SHA256=(Get-FileHash -LiteralPath $file.FullName -Algorithm SHA256).Hash })
    }
}
foreach ($m in $moves) {
    $parent = Split-Path -Path $m.Target -Parent
    if (-not (Test-Path -LiteralPath $parent)) { New-Item -Path $parent -ItemType Directory -Force | Out-Null }
    Move-Item -LiteralPath $m.Source -Destination $m.Target
    if ((Test-Path -LiteralPath $m.Source) -or -not (Test-Path -LiteralPath $m.Target)) { throw "移动校验失败：$($m.From)" }
}
$moves | Select-Object From,To,SHA256 | ConvertTo-Json -Depth 3 | Set-Content -LiteralPath (Join-Path $PSScriptRoot '036_压缩包归档.json') -Encoding UTF8
Write-Output "archived_packages=$($moves.Count)"
