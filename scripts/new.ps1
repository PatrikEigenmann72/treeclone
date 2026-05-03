#!/usr/bin/env pwsh
# ----------------------------------------------------------------------------------------
# Script:       new.ps1
# Description:  Create new C projects and modules using templates, parameters,
#               and naming conventions.
# ----------------------------------------------------------------------------------------
# Author:       Patrik Eigenmann
# eMail:        p.eigenmann72@gmail.com
# GitHub:       https://github.com/PatrikEigenmann72/scripts.git
# ----------------------------------------------------------------------------------------
# Change Log:
# Fri 2024-06-04 Script created.                                                    Version: 00.01
# Sat 2026-04-25 Project and module generation logic added.                         Version: 00.02
# Fri 2026-05-01 Header cleanup, help menu, and script-family fixes.                Version: 00.03
# Sat 2026-05-02 Added the <project>/resources/txt/project.txt to the copy list.    Version: 00.04
# ----------------------------------------------------------------------------------------

param(
    [string]$project,
    [string]$module,
    [switch]$help
)

function Show-Help {
@"
NAME
    new.ps1 - create new C projects or modules from templates

SYNOPSIS
    new.ps1 -project NAME
    new.ps1 -module NAME

DESCRIPTION
    Generates a new C project or module using predefined templates,
    naming conventions, and folder structures.

OPTIONS
    -project NAME
        Create a new project with the given NAME.

    -module NAME
        Create a new module inside the current project.

    -help
        Show this help menu.

EXAMPLES
    new.ps1 -project testapp
    new.ps1 -module samael.chronicle.config
"@ | less
}

if ($help) {
    Show-Help
    exit
}

$TODAY = Get-Date -Format "ddd yyyy-MM-dd"
$TEMPLATE = "$HOME/Development/Templates"

# ====================================================================================
# PROJECT GENERATION
# ====================================================================================
if ($project) {

    Write-Host "Creating project: $project"

    $projDir = $project
    $FILENAME = $project

    $ALLCAPS = ($project.ToUpper() -replace '[^A-Z0-9]', '_') + "_H"

    # Create folder structure
    New-Item -ItemType Directory -Force -Path "$projDir/include","$projDir/src","$projDir/bin","$projDir/resources","$projDir/scripts" | Out-Null

    # Create txt folder and copy project identity template
    New-Item -ItemType Directory -Force -Path "$projDir/resources/txt" | Out-Null
    Copy-Item "$TEMPLATE/project.txt" "$projDir/resources/txt/project.txt"

    # Copy static templates
    Copy-Item "$TEMPLATE/LICENSE"        "$projDir/LICENSE"
    Copy-Item "$TEMPLATE/.gitignore"     "$projDir/.gitignore"

    # Copy readme script family
    Copy-Item "$TEMPLATE/readme"         "$projDir/scripts/readme"
    Copy-Item "$TEMPLATE/readme.ps1"     "$projDir/scripts/readme.ps1"
    Copy-Item "$TEMPLATE/readme.bat"     "$projDir/scripts/readme.bat"

    # Copy install script family
    Copy-Item "$TEMPLATE/install.sh"     "$projDir/scripts/install.sh"
    Copy-Item "$TEMPLATE/install.ps1"    "$projDir/scripts/install.ps1"
    Copy-Item "$TEMPLATE/install.bat"    "$projDir/scripts/install.bat"

    # Copy compile script family
    Copy-Item "$TEMPLATE/install.sh"     "$projDir/scripts/compile.sh"
    Copy-Item "$TEMPLATE/install.ps1"    "$projDir/scripts/compile.ps1"
    Copy-Item "$TEMPLATE/install.bat"    "$projDir/scripts/compile.bat"

    # Copy new script family
    Copy-Item "$TEMPLATE/install.sh"     "$projDir/scripts/new.sh"
    Copy-Item "$TEMPLATE/install.ps1"    "$projDir/scripts/new.ps1"
    Copy-Item "$TEMPLATE/install.bat"    "$projDir/scripts/new.bat"

    # Copy get script family
    Copy-Item "$TEMPLATE/install.sh"     "$projDir/scripts/get.sh"
    Copy-Item "$TEMPLATE/install.ps1"    "$projDir/scripts/get.ps1"
    Copy-Item "$TEMPLATE/install.bat"    "$projDir/scripts/get.bat"

    # Generate project header
    (Get-Content "$TEMPLATE/project.h") `
        -replace "{PROJECT}", $project `
        -replace "{ALLCAPS_PROJECT}", $ALLCAPS `
        -replace "{DATE}", $TODAY `
        -replace "{FILENAME}", $FILENAME |
        Set-Content "$projDir/include/$project.h"

    # Generate project source
    (Get-Content "$TEMPLATE/project.c") `
        -replace "{PROJECT}", $project `
        -replace "{ALLCAPS_PROJECT}", $ALLCAPS `
        -replace "{DATE}", $TODAY `
        -replace "{FILENAME}", $FILENAME |
        Set-Content "$projDir/src/$project.c"

    # Generate main.c
    (Get-Content "$TEMPLATE/main.c") `
        -replace "{PROJECT}", $project `
        -replace "{ALLCAPS_PROJECT}", $ALLCAPS `
        -replace "{DATE}", $TODAY |
        Set-Content "$projDir/src/main.c"

    exit
}

# ====================================================================================
# MODULE GENERATION
# ====================================================================================
if ($module) {

    Write-Host "Creating module: $module"

    $FILENAME = $module
    $ALLCAPS = ($FILENAME.ToUpper() -replace '[^A-Z0-9]', '_') + "_H"

    (Get-Content "$TEMPLATE/module.h") `
        -replace "{PROJECT}", $module `
        -replace "{ALLCAPS_FILENAME}", $ALLCAPS `
        -replace "{DATE}", $TODAY `
        -replace "{FILENAME}", $FILENAME |
        Set-Content "include/$FILENAME.h"

    (Get-Content "$TEMPLATE/module.c") `
        -replace "{PROJECT}", $module `
        -replace "{ALLCAPS_FILENAME}", $ALLCAPS `
        -replace "{DATE}", $TODAY `
        -replace "{FILENAME}", $FILENAME |
        Set-Content "src/$FILENAME.c"

    exit
}