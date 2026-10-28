param([string]$Dotnet, [string]$Packages, [switch]$SkipBuild, [switch]$Offline, [string]$OutputRoot)
$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot
if (-not $SkipBuild) {
    & "$root\build.ps1" -Dotnet $Dotnet -Packages $Packages -Publish -Offline:$Offline
}
$releaseParent = if ($OutputRoot) { $OutputRoot } else { Join-Path $root 'release' }
$releaseRoot = Join-Path $releaseParent ('FAE寄存器工具_' + (Get-Date -Format 'yyyyMMdd_HHmmss'))
$portable = Join-Path $releaseRoot '便携版'
$source = Join-Path $releaseRoot '源码'
New-Item -ItemType Directory -Path $portable,$source -Force | Out-Null
Get-ChildItem -LiteralPath "$root\artifacts\publish" | Copy-Item -Destination $portable -Recurse -Force
Copy-Item -LiteralPath "$root\使用说明.txt" -Destination $portable
if (Test-Path -LiteralPath "$root\docs") { Copy-Item -LiteralPath "$root\docs" -Destination $portable -Recurse }
foreach ($item in @('src','tests','scripts','Data','docs','build.ps1','package.ps1','NuGet.Config','README.md','使用说明.txt','.gitignore')) {
    $itemPath = Join-Path $root $item
    if (Test-Path -LiteralPath $itemPath) {
        if ((Get-Item -LiteralPath $itemPath).PSIsContainer) {
            & robocopy $itemPath (Join-Path $source $item) /E /XD bin obj __pycache__ /NFL /NDL /NJH /NJS /NP | Out-Null
            if ($LASTEXITCODE -gt 7) { throw "Source copy failed: $item" }
        } else { Copy-Item -LiteralPath $itemPath -Destination $source }
    }
}
$hashes = Get-ChildItem -LiteralPath $releaseRoot -File -Recurse | ForEach-Object {
    $relative = $_.FullName.Substring($releaseRoot.Length + 1)
    '{0}  {1}' -f (Get-FileHash -LiteralPath $_.FullName -Algorithm SHA256).Hash, $relative
}
$hashes | Set-Content -LiteralPath "$releaseRoot\文件校验_SHA256.txt" -Encoding utf8
Write-Output $releaseRoot
