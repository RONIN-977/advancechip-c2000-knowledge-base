@echo off
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0Install-Chuangxin-USB-Driver.ps1"
if errorlevel 1 echo Driver installation did not complete. See the message above.
pause
