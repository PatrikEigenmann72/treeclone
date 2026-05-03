# ------------------------------------------------------------------------------------
# Script:       readme.ps1
# Description:  Regenerates README.md with About, Author, and a live file tree.
# ------------------------------------------------------------------------------------
# Author:       Patrik Eigenmann
# eMail:        p.eigenmann72@gmail.com
# GitHub:       https://github.com/PatrikEigenmann72
# ------------------------------------------------------------------------------------
# Change log:
# Wed 2026-04-22 File created.                                          Version: 00.01
# Sat 2026-05-02 Dynamic content management for About/Why per project.  Version: 00.02
# ------------------------------------------------------------------------------------

# Project name = current folder name
$PROJECT = Split-Path -Leaf (Get-Location)

# Today's date
$TODAY = Get-Date -Format "ddd yyyy-MM-dd"

# Working directory (equivalent to $PWD in Bash)
$ROOT = Get-Location

# Path to project-specific text
$ProjectTxt = Join-Path $ROOT "resources/txt/project.txt"

# Run treeclone and capture output
$TREE = treeclone | Out-String

# Start writing README.md
@"
# $PROJECT

"@ | Set-Content -Encoding UTF8 README.md

# Insert project-specific text
Get-Content $ProjectTxt | Add-Content -Encoding UTF8 README.md

@"

## Folder Structure

The folders with binaries like executables, *.class - files, *.dll's will not be tracked.

$TREE

## Author

My name is Patrik Eigenmann. I learned to code when I was eight years old, long before it became my profession. Later I spent nine years as a software engineer, writing code because it was my job. When I moved into live sound, that obligation disappeared — but the curiosity stayed. I still write software because I want to understand things, not because someone assigns me a task. Coding keeps me sharp, and it gives me the freedom to explore ideas on my own terms.
I work on projects in my spare time, usually because something catches my interest or I want to figure out how something works under the hood. Nothing I build is backed by a team or a company. It’s just me, learning and creating because I enjoy it.
If you want to support my work, you can send a donation to p.eigenmann@gmx.net via PayPal. It’s optional, but appreciated.
Everything I publish is free under the GNU Public License v3.0. Use it, modify it, break it, rebuild it — whatever helps you learn or solve your own problems.

## Last Updated
$TODAY
"@ | Add-Content -Encoding UTF8 README.md

Write-Host "README.md regenerated successfully."