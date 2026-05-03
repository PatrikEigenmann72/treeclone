@echo off
:: ------------------------------------------------------------------------------------
:: Script:       install.bat
:: Description:  Installs the already compiled binary from .\bin into %USERPROFILE%\bin.
::               This script does NOT compile the project. Use compile.bat first.
:: ------------------------------------------------------------------------------------
:: Author:       Patrik Eigenmann
:: email:        p.eigenmann72@gmail.com
:: GitHub:       https://github.com/PatrikEigenmann72/Scripts
:: ------------------------------------------------------------------------------------
:: Change Log:
:: Thu 2026-04-30 File created and content added.                        Version: 00.01
:: ------------------------------------------------------------------------------------

:: Extract project name from current directory
set PROJECT=%CD%
for %%I in ("%PROJECT%") do set PROJECT=%%~nI

set BINARY=bin\%PROJECT%

echo Installing %PROJECT%...

if not exist "%BINARY%" (
    echo Error: Binary "%BINARY%" does not exist.
    echo Run "compile.bat" first.
    exit /b 1
)

if not exist "%USERPROFILE%\bin" mkdir "%USERPROFILE%\bin"
copy "%BINARY%" "%USERPROFILE%\bin" >nul

echo Installed to %USERPROFILE%\bin\%PROJECT%
echo Done. Type "%PROJECT%" to run it.