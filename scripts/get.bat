@echo off
REM ----------------------------------------------------------------------------------------
REM Script:       get.bat
REM Description:  Fetches components from the template project into the active project.
REM               Supports C source/header files and script families (.sh/.ps1/.bat).
REM               Used to pull new or updated components intentionally and explicitly.
REM ----------------------------------------------------------------------------------------
REM Author:       Patrik Eigenmann
REM eMail:        p.eigenmann72@gmail.com
REM GitHub:       https://github.com/PatrikEigenmann72/scripts.git
REM ----------------------------------------------------------------------------------------
REM Change Log:
REM Fri 2024-06-04 Script created.                                             Version: 00.01
REM Fri 2026-05-01 Script header added and script-family logic integrated.     Version: 00.02
REM ----------------------------------------------------------------------------------------

if "%~1"=="" (
    echo Usage: get.bat COMPONENT
    exit /b 1
)

set COMPONENT=%~1
set TEMPLATE=C:\Users\peige\Development\cpp\helloc
set DEST_SRC=src
set DEST_INC=include
set DEST_SCR=scripts

REM Extract base name (remove extension)
for %%F in ("%COMPONENT%") do (
    set FILE=%%~nxF
    set BASE=%%~nF
    set EXT=%%~xF
)

REM Copy script family
:copy_scripts
for %%E in (.sh .ps1 .bat) do (
    if exist "%TEMPLATE%\%DEST_SCR%\%BASE%%%E" (
        if not exist "%DEST_SCR%" mkdir "%DEST_SCR%"
        copy "%TEMPLATE%\%DEST_SCR%\%BASE%%%E" "%DEST_SCR%" >nul
        echo Copied %BASE%%%E to %DEST_SCR%/
    )
)
goto :eof

REM Handle .c
if /i "%EXT%"==".c" (
    if exist "%TEMPLATE%\%DEST_SRC%\%FILE%" (
        if not exist "%DEST_SRC%" mkdir "%DEST_SRC%"
        copy "%TEMPLATE%\%DEST_SRC%\%FILE%" "%DEST_SRC%" >nul
        echo Copied %FILE% to %DEST_SRC%/
    ) else (
        echo Source file not found in template: %FILE%
    )
    exit /b
)

REM Handle .h
if /i "%EXT%"==".h" (
    if exist "%TEMPLATE%\%DEST_INC%\%FILE%" (
        if not exist "%DEST_INC%" mkdir "%DEST_INC%"
        copy "%TEMPLATE%\%DEST_INC%\%FILE%" "%DEST_INC%" >nul
        echo Copied %FILE% to %DEST_INC%/

        if exist "%TEMPLATE%\%DEST_SRC%\%BASE%.c" (
            if not exist "%DEST_SRC%" mkdir "%DEST_SRC%"
            copy "%TEMPLATE%\%DEST_SRC%\%BASE%.c" "%DEST_SRC%" >nul
            echo Copied counterpart %BASE%.c to %DEST_SRC%/
        )
    ) else (
        echo Header file not found in template: %FILE%
    )
    exit /b
)

REM Handle script extensions
if /i "%EXT%"==".sh"  goto copy_scripts
if /i "%EXT%"==".ps1" goto copy_scripts
if /i "%EXT%"==".bat" goto copy_scripts

REM Base name: copy .c, .h, and script family
if exist "%TEMPLATE%\%DEST_SRC%\%BASE%.c" (
    if not exist "%DEST_SRC%" mkdir "%DEST_SRC%"
    copy "%TEMPLATE%\%DEST_SRC%\%BASE%.c" "%DEST_SRC%" >nul
    echo Copied %BASE%.c to %DEST_SRC%/
)

if exist "%TEMPLATE%\%DEST_INC%\%BASE%.h" (
    if not exist "%DEST_INC%" mkdir "%DEST_INC%"
    copy "%TEMPLATE%\%DEST_INC%\%BASE%.h" "%DEST_INC%" >nul
    echo Copied %BASE%.h to %DEST_INC%/
)

goto copy_scripts