#!/bin/bash
# --------------------------------------------------------------------------------------------
# Script:       new.sh
# Description:  Create new C projects and modules using templates, parameters,
#               and naming conventions.
# --------------------------------------------------------------------------------------------
# Author:       Patrik Eigenmann
# eMail:        p.eigenmann72@gmail.com
# GitHub:       https://github.com/PatrikEigenmann72/scripts.git
# --------------------------------------------------------------------------------------------
# Change Log:
# Fri 2024-06-04 Script created.                                                Version: 00.01
# Sat 2026-04-25 Project and module generation logic added.                     Version: 00.02
# Fri 2026-05-01 Header cleanup, help menu, and script-family fixes.            Version: 00.03
# Sat 2026-05-02 Added the resources/txt/project.txt to the copy list.          Version: 00.04
# --------------------------------------------------------------------------------------------

# -----------------------------------------
# Manpage-style help
# -----------------------------------------
show_help() {
cat << EOF | less
NAME
    new.sh - create new C projects or modules from templates

SYNOPSIS
    new.sh -project NAME
    new.sh -module NAME

DESCRIPTION
    Generates a new C project or module using predefined templates,
    naming conventions, and folder structures.

OPTIONS
    -project NAME, -p NAME
        Create a new project with the given NAME.
        Generates include/, src/, bin/, resources/, scripts/ and
        copies template script families and boilerplate files.

    -module NAME, -m NAME
        Create a new module inside the current project.
        Generates NAME.h and NAME.c using module templates.

    -h, --help
        Show this help menu.

EXAMPLES
    new.sh -project testapp
    new.sh -module samael.chronicle.config
EOF
}

# -----------------------------------------
# Parse parameters
# -----------------------------------------
while [[ $# -gt 0 ]]; do
    case "$1" in
        -project|-p)
            PROJECTNAME="$2"
            shift 2
            ;;
        -module|-m)
            MODULENAME="$2"
            shift 2
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        *)
            echo "Unknown parameter: $1"
            exit 1
            ;;
    esac
done

# -----------------------------------------
# Date formats
# -----------------------------------------
TODAY=$(date +"%a %Y-%m-%d")

# ====================================================================================
# PROJECT GENERATION
# ====================================================================================
if [[ -n "$PROJECTNAME" ]]; then

    echo "Creating project: $PROJECTNAME"

    TEMPLATE_DIR="$HOME/Development/Templates"

    # Namespace = project name
    FILENAME="$PROJECTNAME"

    # Include guard for project
    ALLCAPS_PROJECT=$(echo "$PROJECTNAME" \
        | tr '[:lower:]' '[:upper:]' \
        | sed -e 's/[^A-Z0-9]/_/g')
    ALLCAPS_PROJECT="${ALLCAPS_PROJECT}_H"

    # Create folder structure
    mkdir -p "$PROJECTNAME"/{include,src,bin,resources,scripts}
    mkdir -p "$PROJECTNAME/resources/txt"
    cp "$TEMPLATE_DIR/project.txt" "$PROJECTNAME/resources/txt/project.txt"

    # Copy static templates
    cp "$TEMPLATE_DIR/LICENSE"        "$PROJECTNAME/LICENSE"
    cp "$TEMPLATE_DIR/.gitignore"     "$PROJECTNAME/.gitignore"

    # Copy readme script family
    cp "$TEMPLATE_DIR/readme"         "$PROJECTNAME/scripts/readme"
    cp "$TEMPLATE_DIR/readme.ps1"     "$PROJECTNAME/scripts/readme.ps1"
    cp "$TEMPLATE_DIR/readme.bat"     "$PROJECTNAME/scripts/readme.bat"

    # Copy install script family
    cp "$TEMPLATE_DIR/install.sh"     "$PROJECTNAME/scripts/install.sh"
    cp "$TEMPLATE_DIR/install.ps1"    "$PROJECTNAME/scripts/install.ps1"
    cp "$TEMPLATE_DIR/install.bat"    "$PROJECTNAME/scripts/install.bat"

    # Copy compile script family
    cp "$TEMPLATE_DIR/install.sh"     "$PROJECTNAME/scripts/compile.sh"
    cp "$TEMPLATE_DIR/install.ps1"    "$PROJECTNAME/scripts/compile.ps1"
    cp "$TEMPLATE_DIR/install.bat"    "$PROJECTNAME/scripts/compile.bat"

    # Copy new script family
    cp "$TEMPLATE_DIR/install.sh"     "$PROJECTNAME/scripts/new.sh"
    cp "$TEMPLATE_DIR/install.ps1"    "$PROJECTNAME/scripts/new.ps1"
    cp "$TEMPLATE_DIR/install.bat"    "$PROJECTNAME/scripts/new.bat"

    # Copy get script family
    cp "$TEMPLATE_DIR/install.sh"     "$PROJECTNAME/scripts/get.sh"
    cp "$TEMPLATE_DIR/install.ps1"    "$PROJECTNAME/scripts/get.ps1"
    cp "$TEMPLATE_DIR/install.bat"    "$PROJECTNAME/scripts/get.bat"

    # Make scripts executable
    chmod +x "$PROJECTNAME/scripts/readme"
    chmod +x "$PROJECTNAME/scripts/"*.sh

    # Generate project header
    sed \
        -e "s/{PROJECT}/$PROJECTNAME/g" \
        -e "s/{ALLCAPS_PROJECT}/$ALLCAPS_PROJECT/g" \
        -e "s/{DATE}/$TODAY/g" \
        -e "s/{FILENAME}/$FILENAME/g" \
        "$TEMPLATE_DIR/project.h" > "$PROJECTNAME/include/$PROJECTNAME.h"

    # Generate project source
    sed \
        -e "s/{PROJECT}/$PROJECTNAME/g" \
        -e "s/{ALLCAPS_PROJECT}/$ALLCAPS_PROJECT/g" \
        -e "s/{DATE}/$TODAY/g" \
        -e "s/{FILENAME}/$FILENAME/g" \
        "$TEMPLATE_DIR/project.c" > "$PROJECTNAME/src/$PROJECTNAME.c"

    # Generate main.c
    sed \
        -e "s/{PROJECT}/$PROJECTNAME/g" \
        -e "s/{ALLCAPS_PROJECT}/$ALLCAPS_PROJECT/g" \
        -e "s/{DATE}/$TODAY/g" \
        "$TEMPLATE_DIR/main.c" > "$PROJECTNAME/src/main.c"

    exit 0
fi

# ====================================================================================
# MODULE GENERATION
# ====================================================================================
if [[ -n "$MODULENAME" ]]; then

    echo "Creating module: $MODULENAME"

    TEMPLATE_DIR="$HOME/Development/Templates"

    FILENAME="$MODULENAME"

    ALLCAPS_FILENAME=$(echo "$FILENAME" \
        | tr '[:lower:]' '[:upper:]' \
        | sed -e 's/[^A-Z0-9]/_/g')
    ALLCAPS_FILENAME="${ALLCAPS_FILENAME}_H"

    # Generate module header
    sed \
        -e "s/{PROJECT}/$MODULENAME/g" \
        -e "s/{ALLCAPS_FILENAME}/$ALLCAPS_FILENAME/g" \
        -e "s/{DATE}/$TODAY/g" \
        -e "s/{FILENAME}/$FILENAME/g" \
        "$TEMPLATE_DIR/module.h" > "include/$FILENAME.h"

    # Generate module source
    sed \
        -e "s/{PROJECT}/$MODULENAME/g" \
        -e "s/{ALLCAPS_FILENAME}/$ALLCAPS_FILENAME/g" \
        -e "s/{DATE}/$TODAY/g" \
        -e "s/{FILENAME}/$FILENAME/g" \
        "$TEMPLATE_DIR/module.c" > "src/$FILENAME.c"

    exit 0
fi