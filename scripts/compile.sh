#!/bin/bash
# ------------------------------------------------------------------------------------
# Script:       compile.sh
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
# Thu 2026-04-30 Updated to use pmake and unified debug flag.           Version: 00.02
# ------------------------------------------------------------------------------------

show_help() {
cat << EOF | less
NAME
    compile.sh - build and prepare project binaries

SYNOPSIS
    compile.sh [OPTIONS]

DESCRIPTION
    This script takes the active directory as project name and
    calls pmake to build the project.

OPTIONS
    -h, -help, -?   Show this help menu
    -DDEBUG         Compile with debug information (forwards -DDEBUG)

EXAMPLES
    compile.sh
    compile.sh -DDEBUG
EOF
}

# Parse arguments
for arg in "$@"; do
    case $arg in
        -h|-help|-\?)
            show_help
            exit 0
            ;;
    esac
done

set -e

# Extract project name from current directory
PROJECT="$(basename "$PWD")"

echo "Building $PROJECT..."

# Forward -DDEBUG only if requested
if [ "$1" = "-DDEBUG" ]; then
    echo "Compiling with -DDEBUG flag..."
    pmake "$PROJECT" -DDEBUG
else
    echo "Compiling for RELEASE..."
    pmake "$PROJECT"
fi

echo "Done. Type 'bin/$PROJECT' to begin."