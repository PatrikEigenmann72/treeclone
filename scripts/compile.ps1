# ------------------------------------------------------------------------------------
# Script:       compile.ps1
# Description:  A simple build script for C projects. It calls pmake using the
#               active directory name as the project name and forwards -DDEBUG
#               when requested.
# ------------------------------------------------------------------------------------
# Author:       Patrik Eigenmann
# email:        p.eigenmann72@gmail.com
# GitHub:       https://github.com/PatrikEigenmann72/Scripts
# ------------------------------------------------------------------------------------
# Change Log:
# Thu 2025-08-14 File created and content added.                        Version: 00.01
# Thu 2026-04-30 Updated to use pmake and unified debug flag.          Version: 00.02
# Thu 2026-04-30 Added manpage-style help section.                     Version: 00.03
# ------------------------------------------------------------------------------------

param(
    [string]$Flag
)

function Show-Help {
@"
NAME
    compile.ps1 - build and prepare project binaries

SYNOPSIS
    .\compile.ps1 [OPTIONS]

DESCRIPTION
    This script takes the active directory as project name and
    calls pmake to build the project.

OPTIONS
    -h, -help, -?   Show this help menu
    -DDEBUG         Compile with debug information (forwards -DDEBUG)

EXAMPLES
    .\compile.ps1
    .\compile.ps1 -DDEBUG
"@ | more
}

# Parse arguments
if ($Flag -in @("-h", "-help", "-?")) {
    Show-Help
    exit
}

# Extract project name from current directory
$projectName = Split-Path -Leaf (Get-Location)

Write-Host "Building $projectName..."

if ($Flag -eq "-DDEBUG") {
    Write-Host "Compiling with -DDEBUG flag..."
    pmake $projectName -DDEBUG
} else {
    Write-Host "Compiling for RELEASE..."
    pmake $projectName
}

Write-Host "Done. Type 'bin\$projectName' to begin."