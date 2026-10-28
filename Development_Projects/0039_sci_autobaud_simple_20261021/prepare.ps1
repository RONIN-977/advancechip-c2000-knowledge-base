$ErrorActionPreference = 'Stop'
$taskBase = 'C:/Users/17879/Desktop/autobaud'
$taskSource = Join-Path $taskBase 'sci_ex3_autobaud_9600'
$taskTarget = Join-Path $taskBase '0039_sci_autobaud'
if(Test-Path -LiteralPath $taskTarget) { throw 'Target already exists; refusing to overwrite.' }
Add-Type -AssemblyName System.IO.Compression.FileSystem
$taskBackup = Join-Path $taskBase ('sci_ex3_autobaud_9600_backup_' + (Get-Date -Format 'yyyyMMdd_HHmmss') + '.zip')
[IO.Compression.ZipFile]::CreateFromDirectory($taskSource, $taskBackup, [IO.Compression.CompressionLevel]::Optimal, $true)
New-Item -ItemType Directory -Path $taskTarget | Out-Null
foreach($taskName in @('.project','.cproject','.ccsproject','28003x_generic_flash_lnk.cmd')) {
    Copy-Item -LiteralPath (Join-Path $taskSource $taskName) -Destination $taskTarget
}
New-Item -ItemType Directory -Path (Join-Path $taskTarget 'device/driverlib/ccs/Debug') -Force | Out-Null
Get-ChildItem -LiteralPath (Join-Path $taskSource 'device') -File | Copy-Item -Destination (Join-Path $taskTarget 'device')
Get-ChildItem -LiteralPath (Join-Path $taskSource 'device/driverlib') -Filter '*.h' -File | Copy-Item -Destination (Join-Path $taskTarget 'device/driverlib')
Copy-Item -LiteralPath (Join-Path $taskSource 'device/driverlib/inc') -Destination (Join-Path $taskTarget 'device/driverlib') -Recurse
Copy-Item -LiteralPath (Join-Path $taskSource 'device/driverlib/ccs/Debug/driverlib.lib') -Destination (Join-Path $taskTarget 'device/driverlib/ccs/Debug')
New-Item -ItemType Directory -Path (Join-Path $taskTarget 'targetConfigs') | Out-Null
Copy-Item -LiteralPath (Join-Path $taskSource 'targetConfigs/TMS320F280039C.ccxml') -Destination (Join-Path $taskTarget 'targetConfigs')
Copy-Item -LiteralPath (Join-Path $taskSource 'vendor/c2000ware/license.txt') -Destination (Join-Path $taskTarget 'TI_license.txt')

[xml]$taskProject = Get-Content -Raw -LiteralPath (Join-Path $taskTarget '.project')
$taskProject.projectDescription.name = '0039_sci_autobaud'
foreach($taskVar in @($taskProject.SelectNodes('//variableList/variable'))) {
    if($taskVar.name -ne 'C2000WARE_DLIB_ROOT') { [void]$taskVar.ParentNode.RemoveChild($taskVar) }
}
$taskProject.Save((Join-Path $taskTarget '.project'))

[xml]$taskCproject = Get-Content -Raw -LiteralPath (Join-Path $taskTarget '.cproject')
foreach($taskConfig in @($taskCproject.SelectNodes('//cconfiguration'))) {
    if(!$taskConfig.SelectSingleNode('storageModule[@name="CPU1_FLASH"]')) { [void]$taskConfig.ParentNode.RemoveChild($taskConfig) }
}
foreach($taskNode in @($taskCproject.SelectNodes('//tool[contains(@superClass,"sysConfig")]|//extension[contains(@id,"SysConfig")]|//macros'))) {
    [void]$taskNode.ParentNode.RemoveChild($taskNode)
}
foreach($taskOpt in @($taskCproject.SelectNodes('//listOptionValue'))) {
    if(($taskOpt.value -match 'SYSCONFIG|C2000WARE_ROOT|c2000ware_libraries|PROJECT_BUILD_DIR.*syscfg|^PRODUCTS=|^PRODUCT_MACRO_IMPORTS=')) {
        [void]$taskOpt.ParentNode.RemoveChild($taskOpt)
    }
}
foreach($taskEntry in $taskCproject.SelectNodes('//sourceEntries/entry')) {
    $taskEntry.SetAttribute('excluding','CPU1_FLASH|device/driverlib')
}
foreach($taskAttr in $taskCproject.SelectNodes('//@*')) {
    $taskAttr.Value = $taskAttr.Value.Replace('sci_ex3_autobaud','0039_sci_autobaud')
}
$taskCproject.Save((Join-Path $taskTarget '.cproject'))

$taskOldMain = Get-Content -Raw -LiteralPath (Join-Path $taskSource 'sci_ex3_echoback.c')
$taskLicenseStart = $taskOldMain.IndexOf('// $Copyright:')
$taskLicenseEnd = $taskOldMain.IndexOf('// Included Files')
$taskLicense = $taskOldMain.Substring($taskLicenseStart, $taskLicenseEnd-$taskLicenseStart)
$taskHeader = "//#############################################################################`r`n// FILE: 0039_sci_autobaud.c`r`n// TITLE: SCI polling and periodic TI autobaud test (Flash).`r`n//#############################################################################`r`n"
$taskBody = Get-Content -Raw -LiteralPath (Join-Path $PSScriptRoot 'main_body.txt')
[IO.File]::WriteAllText((Join-Path $taskTarget '0039_sci_autobaud.c'), $taskHeader+$taskLicense+$taskBody, [Text.UTF8Encoding]::new($false))
Copy-Item -LiteralPath (Join-Path $PSScriptRoot 'README_测试说明.md') -Destination $taskTarget
Write-Output "Backup: $taskBackup"
Write-Output "Project: $taskTarget"
