param([switch]$Refresh)
$ErrorActionPreference = 'Stop'
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
Set-Location -LiteralPath $repoRoot
$manifestPath = '99_整理记录/20261015_GitHub迁移/文件校验清单.csv'
if ((Test-Path -LiteralPath $manifestPath) -and -not $Refresh) { throw '清单已存在；确认重新生成时使用 -Refresh。' }
$paths = @(git -c core.quotepath=false ls-files --cached --others --exclude-standard)
if ($LASTEXITCODE -ne 0) { throw '无法读取 Git 文件范围。' }
$paths = @($paths | Sort-Object -Unique | Where-Object { $_ -ne $manifestPath })
$largePaths = [System.Collections.Generic.List[string]]::new()
$totalBytes = [long]0
$index = 0
$rows = foreach ($path in $paths) {
    $item = Get-Item -LiteralPath $path -Force
    if ($item.Length -ge 50MB) { $largePaths.Add($path) }
    $totalBytes += $item.Length
    $index++
    if ($index % 10000 -eq 0) { Write-Host "已校验 $index / $($paths.Count)" }
    [pscustomobject]@{Path=$path;Bytes=$item.Length;SHA256=(Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash}
}
$rows | Export-Csv -LiteralPath $manifestPath -NoTypeInformation -Encoding utf8
# Retain large binary files without risking GitHub's ordinary Git object limit.
foreach ($path in $largePaths) {
    $escapedPath = $path.Replace('\','\\').Replace('"','\"')
    Add-Content -LiteralPath '.gitattributes' -Value ('"' + $escapedPath + '" filter=lfs diff=lfs merge=lfs -text') -Encoding utf8
}
# .gitattributes was updated after hashing; keep the manifest consistent.
$rows | Where-Object Path -eq '.gitattributes' | ForEach-Object {
    $_.Bytes = (Get-Item -LiteralPath '.gitattributes').Length
    $_.SHA256 = (Get-FileHash -LiteralPath '.gitattributes' -Algorithm SHA256).Hash
}
$rows | Export-Csv -LiteralPath $manifestPath -NoTypeInformation -Encoding utf8
Write-Host "完成：$($paths.Count) 个文件，$([math]::Round($totalBytes/1GB,3)) GiB；50 MiB 以上文件 $($largePaths.Count) 个。"
