$ErrorActionPreference = 'Stop'
$repoRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$rootPrefix = $repoRoot.TrimEnd([IO.Path]::DirectorySeparatorChar) + [IO.Path]::DirectorySeparatorChar
$catalog = Get-Content -LiteralPath (Join-Path $PSScriptRoot 'large-materials.json') -Raw -Encoding UTF8 | ConvertFrom-Json
$cacheDir = Join-Path $repoRoot 'tmp/large-materials'

foreach ($entry in $catalog.Files) {
    $target = [IO.Path]::GetFullPath((Join-Path $repoRoot $entry.Path))
    if (-not $target.StartsWith($rootPrefix, [StringComparison]::OrdinalIgnoreCase)) { throw '还原路径超出仓库。' }
    if (Test-Path -LiteralPath $target) {
        if ((Get-FileHash -LiteralPath $target -Algorithm SHA256).Hash -ne $entry.SHA256) {
            throw "现有文件哈希不符，保留原文件，请先人工核对：$($entry.Path)"
        }
        Write-Host "已核实：$($entry.Path)"
        continue
    }
    if (-not (Get-Command gh -ErrorAction SilentlyContinue)) { throw '请先安装 GitHub 官方 CLI（gh）并执行 gh auth login。' }
    New-Item -ItemType Directory -Path $cacheDir -Force | Out-Null
    $cached = Join-Path $cacheDir $entry.Asset
    if (-not (Test-Path -LiteralPath $cached)) {
        & gh release download $catalog.Release --repo $catalog.Repository --pattern $entry.Asset --dir $cacheDir
        if ($LASTEXITCODE -ne 0) { throw '私有附件下载失败，请核对 gh 登录账号和仓库权限。' }
    }
    if ((Get-Item -LiteralPath $cached).Length -ne $entry.Bytes -or
        (Get-FileHash -LiteralPath $cached -Algorithm SHA256).Hash -ne $entry.SHA256) { throw '下载文件大小或 SHA256 不符，停止还原。' }
    New-Item -ItemType Directory -Path (Split-Path -Parent $target) -Force | Out-Null
    Copy-Item -LiteralPath $cached -Destination $target
    if ((Get-FileHash -LiteralPath $target -Algorithm SHA256).Hash -ne $entry.SHA256) { throw '复制后 SHA256 不符。' }
    Write-Host "已还原并核实：$($entry.Path)"
}
Write-Host '超限原始资料包已取齐；下载缓存位于仓库 tmp/large-materials。'
