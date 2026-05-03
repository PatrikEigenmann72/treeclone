#!/bin/bash
# ----------------------------------------------------------------------------------------
# Script:       get.sh
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

show_help() {
cat << EOF | less
NAME
    get.sh - copy C components or script components from template project

SYNOPSIS
    get.sh [COMPONENT]

DESCRIPTION
    Copies C source/header files or script families from the template project
    into the active project. Behavior is determined by the suffix of COMPONENT:

        foo.c       -> copy foo.c from template src/ to active src/
        foo.h       -> copy foo.h from template include/ to active include/
                       if foo.c exists in template src/, copy it too
        foo.sh      -> copy foo.sh, foo.ps1, foo.bat from template scripts/
        foo.ps1     -> same as above
        foo.bat     -> same as above
        foo         -> copy foo.c, foo.h, and foo.sh/.ps1/.bat if they exist

OPTIONS
    -h, --help     Show this help menu

EXAMPLES
    get.sh foo.c
    get.sh samael.chronicle.config.h
    get.sh install.sh
    get.sh install
EOF
}

TEMPLATE=/Users/patrik/Development/cpp/helloc
DEST_SRC=src
DEST_INC=include
DEST_SCR=scripts

set -e

copy_script_family() {
    local BASE="$1"

    # .sh
    if [[ -f "$TEMPLATE/$DEST_SCR/$BASE.sh" ]]; then
        mkdir -p "$DEST_SCR"
        cp "$TEMPLATE/$DEST_SCR/$BASE.sh" "$DEST_SCR/"
        echo "Copied $BASE.sh to $DEST_SCR/"
    fi

    # .ps1
    if [[ -f "$TEMPLATE/$DEST_SCR/$BASE.ps1" ]]; then
        mkdir -p "$DEST_SCR"
        cp "$TEMPLATE/$DEST_SCR/$BASE.ps1" "$DEST_SCR/"
        echo "Copied $BASE.ps1 to $DEST_SCR/"
    fi

    # .bat
    if [[ -f "$TEMPLATE/$DEST_SCR/$BASE.bat" ]]; then
        mkdir -p "$DEST_SCR"
        cp "$TEMPLATE/$DEST_SCR/$BASE.bat" "$DEST_SCR/"
        echo "Copied $BASE.bat to $DEST_SCR/"
    fi
}

if [[ $# -eq 0 ]]; then
    show_help
    exit 1
fi

case "$1" in
    -h|--help)
        show_help
        exit 0
        ;;

    *.c)
        FILE=$(basename "$1")
        echo "$TEMPLATE/$DEST_SRC/$FILE"
        if [[ -f "$TEMPLATE/$DEST_SRC/$FILE" ]]; then
            mkdir -p "$DEST_SRC"
            cp "$TEMPLATE/$DEST_SRC/$FILE" "$DEST_SRC/"
            echo "Copied $FILE to $DEST_SRC/"
        else
            echo "Source file not found in template: $FILE"
        fi
        ;;

    *.h)
        FILE=$(basename "$1")
        BASE="${FILE%.h}"
        echo "$TEMPLATE/$DEST_INC/$FILE"
        if [[ -f "$TEMPLATE/$DEST_INC/$FILE" ]]; then
            mkdir -p "$DEST_INC"
            cp "$TEMPLATE/$DEST_INC/$FILE" "$DEST_INC/"
            echo "Copied $FILE to $DEST_INC/"

            if [[ -f "$TEMPLATE/$DEST_SRC/$BASE.c" ]]; then
                mkdir -p "$DEST_SRC"
                cp "$TEMPLATE/$DEST_SRC/$BASE.c" "$DEST_SRC/"
                echo "Copied counterpart $BASE.c to $DEST_SRC/"
            fi
        else
            echo "Header file not found in template: $FILE"
        fi
        ;;

    *.sh|*.ps1|*.bat)
        FILE=$(basename "$1")
        BASE="${FILE%.*}"
        copy_script_family "$BASE"
        ;;

    *)
        BASE="$1"

        # Copy .c
        if [[ -f "$TEMPLATE/$DEST_SRC/$BASE.c" ]]; then
            mkdir -p "$DEST_SRC"
            cp "$TEMPLATE/$DEST_SRC/$BASE.c" "$DEST_SRC/"
            echo "Copied $BASE.c to $DEST_SRC/"
        fi

        # Copy .h
        if [[ -f "$TEMPLATE/$DEST_INC/$BASE.h" ]]; then
            mkdir -p "$DEST_INC"
            cp "$TEMPLATE/$DEST_INC/$BASE.h" "$DEST_INC/"
            echo "Copied $BASE.h to $DEST_INC/"
        fi

        # Copy script family
        copy_script_family "$BASE"
        ;;
esac