@echo off
REM ------------------------------------------------------------------------------------
REM Script:       readme.bat
REM Description:  Regenerates README.md with About, Author, and a live file tree.
REM ------------------------------------------------------------------------------------
REM Author:       Patrik Eigenmann
REM eMail:        p.eigenmann72@gmail.com
REM GitHub:       https://github.com/PatrikEigenmann72
REM ------------------------------------------------------------------------------------
REM Change log:
REM Wed 2026-04-22 File created.                                          Version: 00.01
REM Sat 2026-05-02 Dynamic content management for About/Why per project.  Version: 00.02
REM ------------------------------------------------------------------------------------

REM Determine project name from current directory
for %%I in ("%CD%") do set PROJECT=%%~nI

REM Today's date
set TODAY=%date%

REM Path to project-specific text (equivalent to $PWD/resources/txt/project.txt)
set PROJECTTXT=%CD%\resources\txt\project.txt

REM Run treeclone.exe and capture output
treeclone > tree_output.tmp

REM Start writing README.md
(
echo # %PROJECT%
echo.
) > README.md

REM Insert project-specific text
type "%PROJECTTXT%" >> README.md

REM Continue README.md
(
echo.
echo ## Folder Structure
echo.
echo The folders with binaries like executables, *.class - files, *.dll's will not be tracked.
echo.
echo ```
) >> README.md

REM Append tree output
type tree_output.tmp >> README.md
echo ``` >> README.md

REM Append Author section
(
echo.
echo ## Author
echo.
echo My name is Patrik Eigenmann. I learned to code when I was eight years old, long before it became my profession. Later I spent nine years as a software engineer, writing code because it was my job. When I moved into live sound, that obligation disappeared — but the curiosity stayed. I still write software because I want to understand things, not because someone assigns me a task. Coding keeps me sharp, and it gives me the freedom to explore ideas on my own terms.
echo I work on projects in my spare time, usually because something catches my interest or I want to figure out how something works under the hood. Nothing I build is backed by a team or a company. It’s just me, learning and creating because I enjoy it.
echo If you want to support my work, you can send a donation to p.eigenmann@gmx.net via PayPal. It’s optional, but appreciated.
echo Everything I publish is free under the GNU Public License v3.0. Use it, modify it, break it, rebuild it — whatever helps you learn or solve your own problems.
echo.
echo ## Last Updated
echo %TODAY%
) >> README.md

del tree_output.tmp

echo README.md regenerated successfully.