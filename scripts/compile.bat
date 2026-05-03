@echo off
:: ------------------------------------------------------------------------------------
:: Script:       compile.bat
:: Description:  A simple build script for C projects. It calls pmake using the
::               active directory name as the project name and forwards -DDEBUG
::               when requested.
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

echo Building %PROJECT%...

if "%1"=="-DDEBUG" (
    echo Compiling with -DDEBUG flag...
    pmake %PROJECT% -DDEBUG
) else (
    echo Compiling for RELEASE...
    pmake %PROJECT%
)

echo Done. Type "bin\%PROJECT%" to begin.