#!/usr/bin/env pwsh
# ----------------------------------------------------------------------------------------
# Script:       get.ps1
# Description:  Fetches components from the template project into the active project.
#               Supports C source/header files and script families (.sh/.ps1/.bat).
#               Used to pull new or updated components intentionally and explicitly.
# ----------------------------------------------------------------------------------------
# Author:       Patrik Eigenmann
# eMail:        p.eigenmann72@gmail.com
# GitHub:       https://github.com/PatrikEigenmann72/scripts.git
# ----------------------------------------------------------------------------------------
# Change Log:
# Fri 2024-06-04 Script created.                                             Version: 00.01
# Fri 2026-05-01 Script header added and script-family logic integrated.     Version: 00.02
# ----------------------------------------------------------------------------------------

param(
    [Parameter(Mandatory=$true)]
    [string]$Component
)

$Template = "/Users/patrik/Development/cpp/helloc"
$DestSrc  = "src"
$DestInc  = "include"
$DestScr  = "scripts"

function Copy-ScriptFamily {
    param([string]$Base)

    foreach ($ext in @("sh", "ps1", "bat")) {
        $src = "$Template/$DestScr/$Base.$ext"
        if (Test-Path $src) {
            if (-not (Test-Path $DestScr)) { New-Item -ItemType Directory -Path $DestScr | Out-Null }
            Copy-Item $src $DestScr
            Write-Host "Copied $Base.$ext to $DestScr/"
        }
    }
}

switch -regex ($Component) {

    ".*\.c$" {
        $file = Split-Path $Component -Leaf
        $src = "$Template/$DestSrc/$file"
        if (Test-Path $src) {
            if (-not (Test-Path $DestSrc)) { New-Item -ItemType Directory -Path $DestSrc | Out-Null }
            Copy-Item $src $DestSrc
            Write-Host "Copied $file to $DestSrc/"
        } else {
            Write-Host "Source file not found in template: $file"
        }
    }

    ".*\.h$" {
        $file = Split-Path $Component -Leaf
        $base = $file -replace "\.h$",""
        $hdr  = "$Template/$DestInc/$file"
        $srcC = "$Template/$DestSrc/$base.c"

        if (Test-Path $hdr) {
            if (-not (Test-Path $DestInc)) { New-Item -ItemType Directory -Path $DestInc | Out-Null }
            Copy-Item $hdr $DestInc
            Write-Host "Copied $file to $DestInc/"

            if (Test-Path $srcC) {
                if (-not (Test-Path $DestSrc)) { New-Item -ItemType Directory -Path $DestSrc | Out-Null }
                Copy-Item $srcC $DestSrc
                Write-Host "Copied counterpart $base.c to $DestSrc/"
            }
        } else {
            Write-Host "Header file not found in template: $file"
        }
    }

    ".*\.(sh|ps1|bat)$" {
        $file = Split-Path $Component -Leaf
        $base = $file -replace "\.[^.]+$",""
        CopyScriptFamily $base
    }

    default {
        $base = $Component

        $srcC = "$Template/$DestSrc/$base.c"
        if (Test-Path $srcC) {
            if (-not (Test-Path $DestSrc)) { New-Item -ItemType Directory -Path $DestSrc | Out-Null }
            Copy-Item $srcC $DestSrc
            Write-Host "Copied $base.c to $DestSrc/"
        }

        $hdr = "$Template/$DestInc/$base.h"
        if (Test-Path $hdr) {
            if (-not (Test-Path $DestInc)) { New-Item -ItemType Directory -Path $DestInc | Out-Null }
            Copy-Item $hdr $DestInc
            Write-Host "Copied $base.h to $DestInc/"
        }

        CopyScriptFamily $base
    }
}