# ------------------------------------------------------------------------------------
# Script:       install.ps1
# Description:  Installs the already compiled binary from .\bin into ~/bin.
#               This script does NOT compile the project. Use compile.ps1 first.
# ------------------------------------------------------------------------------------
# Author:       Patrik Eigenmann
# email:        p.eigenmann72@gmail.com
# GitHub:       https://github.com/PatrikEigenmann72/Scripts
# ------------------------------------------------------------------------------------
# Change Log:
# Thu 2025-08-14 File created and content added.                        Version: 00.01
# Thu 2026-04-30 Updated to remove compilation logic.                   Version: 00.02
# Thu 2026-04-30 Added manpage-style help section.                      Version: 00.03
# ------------------------------------------------------------------------------------

param(
    [string]$Flag
)

function Show-Help {
@"
NAME
    install.ps1 - install the compiled project binary into ~/bin

SYNOPSIS
    .\install.ps1 [OPTIONS]

DESCRIPTION
    This script takes the active directory as project name and
    installs the existing binary from .\bin\ into ~/bin.
    It does NOT compile the project. Use compile.ps1 first.

OPTIONS
    -h, -help, -?   Show this help menu

EXAMPLES
    .\install.ps1
"@ | more
}

# Parse arguments
if ($Flag -in @("-h", "-help", "-?")) {
    Show-Help
    exit
}

# Extract project name from current directory
$projectName = Split-Path -Leaf (Get-Location)
$binary = ".\bin\$projectName"

Write-Host "Installing $projectName..."

# Ensure binary exists
if (-not (Test-Path $binary)) {
    Write-Host "Error: Binary '$binary' does not exist."
    Write-Host "Run '.\compile.ps1' first."
    exit 1
}

# Install to ~/bin
$homeBin = Join-Path $HOME "bin"
New-Item -ItemType Directory -Force -Path $homeBin | Out-Null
Copy-Item $binary $homeBin -Force

Write-Host "Installed to $homeBin\$projectName"
Write-Host "Done. Type '$projectName' to run it."