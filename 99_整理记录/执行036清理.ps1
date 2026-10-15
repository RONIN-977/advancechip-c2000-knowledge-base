$ErrorActionPreference = 'Stop'
$root = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$prefix = $root.TrimEnd('\') + '\'

function SafePath([string]$relativePath) {
    $candidate = [IO.Path]::GetFullPath((Join-Path $root ($relativePath -replace '/', '\')))
    if (-not $candidate.StartsWith($prefix, [StringComparison]::OrdinalIgnoreCase)) {
        throw "路径越界：$relativePath"
    }
    $resolved = (Resolve-Path -LiteralPath $candidate).Path
    if (-not $resolved.StartsWith($prefix, [StringComparison]::OrdinalIgnoreCase)) {
        throw "解析后路径越界：$relativePath"
    }
    return $resolved
}

function SourceHashes([string]$folder) {
    $result = @{}
    foreach ($file in (Get-ChildItem -LiteralPath $folder -Recurse -File)) {
        $rel = $file.FullName.Substring($folder.Length)
        if ($rel -match '(?i)\\Debug\\|\\\.launches\\|\\\.settings\\') { continue }
        $result[$rel] = (Get-FileHash -LiteralPath $file.FullName -Algorithm SHA256).Hash
    }
    return $result
}

$deletions = New-Object 'System.Collections.Generic.List[object]'
$oldBoots = @(
    @{ Path='036/03_通用在线升级/历史APP及工具/LIN_旧包/ADP32F03x_RomBoot'; Retained='03_通用在线升级/LIN_新版'; Reason='旧 LIN BOOT，用户确认新版通用高低压' },
    @{ Path='036/03_通用在线升级/历史APP及工具/LIN_旧包/ADP32F03x_RomBoot.zip'; Retained='03_通用在线升级/LIN_新版'; Reason='旧 LIN BOOT 压缩包' },
    @{ Path='036/03_通用在线升级/历史APP及工具/CAN_旧包/CANUpgrade_Demo/ADP32F036CAN_Boot_20250611'; Retained='03_通用在线升级/CAN_新版'; Reason='旧 CAN BOOT，用户确认新版通用高低压' }
)
foreach ($entry in $oldBoots) {
    $deletions.Add([pscustomobject]@{ Path=$entry.Path; Full=(SafePath $entry.Path); Retained=$entry.Retained; Reason=$entry.Reason })
}

$duplicates = @(
    'Example_ADP32F036CANFD', 'Example_ADP32F036CANFD_CAN2.0',
    'Example_ADP32F036CANFD_ISR', 'Example_ADP32F036Ext_Int',
    'Example_ADP32F036Lin_Master', 'Example_ADP32F036PGA',
    'Example_ADP32F036SCIA', 'Example_ADP32F036SPI_LoopBack',
    'Example_ADP32F036WD'
)
foreach ($name in $duplicates) {
    $from = "036/04_基础例程/历史差异例程/$name"
    $keep = "036/04_基础例程/ADP32F036_Example_20241029/$name"
    $a = SafePath $from
    $b = SafePath $keep
    $ah = SourceHashes $a
    $bh = SourceHashes $b
    if ($ah.Count -ne $bh.Count) { throw "例程文件数不一致：$name" }
    foreach ($key in $ah.Keys) {
        if ($ah[$key] -ne $bh[$key]) { throw "例程源码不同：$name $key" }
    }
    $deletions.Add([pscustomobject]@{ Path=$from; Full=$a; Retained=$keep; Reason='非 Debug 源码文件 SHA-256 完全相同；仅旧副本' })
}

$manifest = New-Object 'System.Collections.Generic.List[object]'
foreach ($item in $deletions) {
    $files = if ((Get-Item -LiteralPath $item.Full).PSIsContainer) { Get-ChildItem -LiteralPath $item.Full -Recurse -File } else { @(Get-Item -LiteralPath $item.Full) }
    foreach ($file in $files) {
        $manifest.Add([pscustomobject]@{
            DeletedPath = $file.FullName.Substring($root.Length + 1).Replace('\','/')
            Bytes = $file.Length
            SHA256 = (Get-FileHash -LiteralPath $file.FullName -Algorithm SHA256).Hash
            Retained = $item.Retained
            Reason = $item.Reason
        })
    }
}
$manifest | Export-Csv -LiteralPath (Join-Path $PSScriptRoot '036_删除文件清单.csv') -NoTypeInformation -Encoding UTF8

foreach ($item in $deletions) {
    if ((Get-Item -LiteralPath $item.Full).PSIsContainer) {
        Remove-Item -LiteralPath $item.Full -Recurse -Force
    } else {
        Remove-Item -LiteralPath $item.Full -Force
    }
    if (Test-Path -LiteralPath $item.Full) { throw "删除后校验失败：$($item.Path)" }
}

Write-Output "deleted_targets=$($deletions.Count) audited_files=$($manifest.Count)"
