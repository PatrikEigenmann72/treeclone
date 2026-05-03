#!/bin/bash
# ------------------------------------------------------------------------------------
# Script:       install.sh
# Description:  Installs the already compiled binary from ./bin into ~/bin.
#               This script does NOT compile the project. Use compile.sh first.
# ------------------------------------------------------------------------------------
# Author:       Patrik Eigenmann
# email:        p.eigenmann72@gmail.com
# GitHub:       https://github.com/PatrikEigenmann72/Scripts
# ------------------------------------------------------------------------------------
# Change Log:
# Thu 2025-08-14 File created and content added.                        Version: 00.01
# Thu 2026-04-30 Updated to remove compilation logic.                   Version: 00.02
# ------------------------------------------------------------------------------------

show_help() {
cat << EOF | less
NAME
    install.sh - install the compiled project binary into ~/bin

SYNOPSIS
    install.sh [OPTIONS]

DESCRIPTION
    This script takes the active directory as project name and
    installs the existing binary from ./bin/ into ~/bin/.
    It does NOT compile the project. Use compile.sh first.

OPTIONS
    -h, -help, -?   Show this help menu

EXAMPLES
    install.sh
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
BINARY="./bin/$PROJECT"

echo "Installing $PROJECT..."

# Ensure binary exists
if [ ! -f "$BINARY" ]; then
    echo "Error: Binary '$BINARY' does not exist."
    echo "Run './compile.sh' first."
    exit 1
fi

# Install to ~/bin
mkdir -p "$HOME/bin"
cp "$BINARY" "$HOME/bin"

echo "Installed to ~/bin/$PROJECT"
echo "Done. Type '$PROJECT' to run it."