@echo off
REM ----------------------------------------------------------------------------------------
REM Script:       new.bat
REM Description:  Create new C projects and modules using templates, parameters,
REM               and naming conventions.
REM ----------------------------------------------------------------------------------------
REM Author:       Patrik Eigenmann
REM eMail:        p.eigenmann72@gmail.com
REM GitHub:       https://github.com/PatrikEigenmann72/scripts.git
REM ----------------------------------------------------------------------------------------
REM Change Log:
REM Fri 2024-06-04 Script created.                                             Version: 00.01
REM Sat 2026-04-25 Project and module generation logic added.                  Version: 00.02
REM Fri 2026-05-01 Header cleanup, help menu, and script-family fixes.         Version: 00.03
REM Sat 2026-05-02 Added the <project>\resources\txt\project.txt copy step.    Version: 00.04
REM ----------------------------------------------------------------------------------------

if "%~1"=="" (
    echo Usage:
    echo   new.bat -project NAME
    echo   new.bat -module NAME
    exit /b 1
)

set TEMPLATE=%USERPROFILE%\Development\Templates

for /f "tokens=1-2" %%A in ("%*") do (
    if "%%A"=="-project" set PROJECTNAME=%%B
    if "%%A"=="-p"       set PROJECTNAME=%%B
    if "%%A"=="-module"  set MODULENAME=%%B
    if "%%A"=="-m"       set MODULENAME=%%B
)

for /f %%A in ('powershell -NoLogo -Command "Get-Date -Format \"ddd yyyy-MM-dd\""') do set TODAY=%%A

REM ====================================================================================
REM PROJECT GENERATION
REM ====================================================================================
if defined PROJECTNAME (
    echo Creating project: %PROJECTNAME%

    set PROJDIR=%PROJECTNAME%
    set FILENAME=%PROJECTNAME%

    set ALLCAPS=%PROJECTNAME%
    set ALLCAPS=%ALLCAPS:"=%
    set ALLCAPS=%ALLCAPS:.=%
    set ALLCAPS=%ALLCAPS: =_%
    set ALLCAPS=%ALLCAPS%_H

    mkdir "%PROJDIR%\include"
    mkdir "%PROJDIR%\src"
    mkdir "%PROJDIR%\bin"
    mkdir "%PROJDIR%\resources"
    mkdir "%PROJDIR%\resources\txt"
    mkdir "%PROJDIR%\scripts"

    REM Copy project identity file
    copy "%TEMPLATE%\project.txt" "%PROJDIR%\resources\txt\project.txt" >nul

    REM Copy static templates
    copy "%TEMPLATE%\LICENSE"        "%PROJDIR%\LICENSE" >nul
    copy "%TEMPLATE%\.gitignore"     "%PROJDIR%\.gitignore" >nul

    REM Copy readme script family
    copy "%TEMPLATE%\readme"         "%PROJDIR%\scripts\readme" >nul
    copy "%TEMPLATE%\readme.ps1"     "%PROJDIR%\scripts\readme.ps1" >nul
    copy "%TEMPLATE%\readme.bat"     "%PROJDIR%\scripts\readme.bat" >nul

    REM Copy install script family
    copy "%TEMPLATE%\install.sh"     "%PROJDIR%\scripts\install.sh" >nul
    copy "%TEMPLATE%\install.ps1"    "%PROJDIR%\scripts\install.ps1" >nul
    copy "%TEMPLATE%\install.bat"    "%PROJDIR%\scripts\install.bat" >nul

    REM Copy compile script family
    copy "%TEMPLATE%\install.sh"     "%PROJDIR%\scripts\compile.sh" >nul
    copy "%TEMPLATE%\install.ps1"    "%PROJDIR%\scripts\compile.ps1" >nul
    copy "%TEMPLATE%\install.bat"    "%PROJDIR%\scripts\compile.bat" >nul

    REM Copy new script family
    copy "%TEMPLATE%\install.sh"     "%PROJDIR%\scripts\new.sh" >nul
    copy "%TEMPLATE%\install.ps1"    "%PROJDIR%\scripts\new.ps1" >nul
    copy "%TEMPLATE%\install.bat"    "%PROJDIR%\scripts\new.bat" >nul

    REM Copy get script family
    copy "%TEMPLATE%\install.sh"     "%PROJDIR%\scripts\get.sh" >nul
    copy "%TEMPLATE%\install.ps1"    "%PROJDIR%\scripts\get.ps1" >nul
    copy "%TEMPLATE%\install.bat"    "%PROJDIR%\scripts\get.bat" >nul

    REM Generate project header
    powershell -NoLogo -Command ^
        "(Get-Content \"%TEMPLATE%\project.h\") ^
        -replace '{PROJECT}', '%PROJECTNAME%' ^
        -replace '{ALLCAPS_PROJECT}', '%ALLCAPS%' ^
        -replace '{DATE}', '%TODAY%' ^
        -replace '{FILENAME}', '%FILENAME%' ^
        | Set-Content \"%PROJDIR%\include\%PROJECTNAME%.h\""

    REM Generate project source
    powershell -NoLogo -Command ^
        "(Get-Content \"%TEMPLATE%\project.c\") ^
        -replace '{PROJECT}', '%PROJECTNAME%' ^
        -replace '{ALLCAPS_PROJECT}', '%ALLCAPS%' ^
        -replace '{DATE}', '%TODAY%' ^
        -replace '{FILENAME}', '%FILENAME%' ^
        | Set-Content \"%PROJDIR%\src\%PROJECTNAME%.c\""

    REM Generate main.c
    powershell -NoLogo -Command ^
        "(Get-Content \"%TEMPLATE%\main.c\") ^
        -replace '{PROJECT}', '%PROJECTNAME%' ^
        -replace '{ALLCAPS_PROJECT}', '%ALLCAPS%' ^
        -replace '{DATE}', '%TODAY%' ^
        | Set-Content \"%PROJDIR%\src\main.c\""

    exit /b
)

REM ====================================================================================
REM MODULE GENERATION
REM ====================================================================================
if defined MODULENAME (
    echo Creating module: %MODULENAME%

    set FILENAME=%MODULENAME%

    set ALLCAPS=%FILENAME%
    set ALLCAPS=%ALLCAPS:"=%
    set ALLCAPS=%ALLCAPS:.=%
    set ALLCAPS=%ALLCAPS: =_%
    set ALLCAPS=%ALLCAPS%_H

    powershell -NoLogo -Command ^
        "(Get-Content \"%TEMPLATE%\module.h\") ^
        -replace '{PROJECT}', '%MODULENAME%' ^
        -replace '{ALLCAPS_FILENAME}', '%ALLCAPS%' ^
        -replace '{DATE}', '%TODAY%' ^
        -replace '{FILENAME}', '%FILENAME%' ^
        | Set-Content \"include\%FILENAME%.h\""

    powershell -NoLogo -Command ^
        "(Get-Content \"%TEMPLATE%\module.c\") ^
        -replace '{PROJECT}', '%MODULENAME%' ^
        -replace '{ALLCAPS_FILENAME}', '%ALLCAPS%' ^
        -replace '{DATE}', '%TODAY%' ^
        -replace '{FILENAME}', '%FILENAME%' ^
        | Set-Content \"src\%FILENAME%.c\""

    exit /b
)