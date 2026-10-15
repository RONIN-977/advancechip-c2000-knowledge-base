param([switch]$Refresh)
$ErrorActionPreference = 'Stop'
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
Set-Location -LiteralPath $repoRoot
$manifestPath = '99_整理记录/20261015_GitHub迁移/文件校验清单.csv'
if ((Test-Path -LiteralPath $manifestPath) -and -not $Refresh) { throw '清单已存在；确认重新生成时使用 -Refresh。' }
$paths = @(git -c core.quotepath=false ls-files --cached --others --exclude-standard)
if ($LASTEXITCODE -ne 0) { throw '无法读取 Git 文件范围。' }
$catalog = Get-Content -LiteralPath 'tools/large-materials.json' -Raw -Encoding UTF8 | ConvertFrom-Json
$paths += @($catalog.Files.Path)
$uniquePaths = [Collections.Generic.HashSet[string]]::new([StringComparer]::Ordinal)
foreach ($path in $paths) { if ($path -ne $manifestPath) { [void]$uniquePaths.Add($path) } }
$paths = @($uniquePaths | Sort-Object)
$totalBytes = [long]0
$index = 0
$rows = foreach ($path in $paths) {
    $item = Get-Item -LiteralPath $path -Force
    $totalBytes += $item.Length
    $index++
    if ($index % 10000 -eq 0) { Write-Host "已校验 $index / $($paths.Count)" }
    [pscustomobject]@{Path=$path;Bytes=$item.Length;SHA256=(Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash}
}
$rows | Export-Csv -LiteralPath $manifestPath -NoTypeInformation -Encoding utf8
Write-Host "完成：$($paths.Count) 个文件，$([math]::Round($totalBytes/1GB,3)) GiB。包含私有 Release 还原的原始包；清单不递归记录自身哈希。"
