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
# ------------------------------------------------------------------------------------

# Project name = current folder name
$PROJECT = Split-Path -Leaf (Get-Location)

# Today's date
$TODAY = Get-Date -Format "ddd yyyy-MM-dd"

# Run treeclone and capture output
$TREE = treeclone | Out-String

# Start writing README.md
@"
# $PROJECT

## About

The original tree command didn’t come from UNIX at all — it first appeared in the DOS world in the 1980s, where early PC operating systems needed a simple way to visualize directory
structures. Over time, the idea spread, and the version most developers know today — the one installed on Linux and BSD systems — was written by Steve Baker, whose implementation
became the de‑facto standard across UNIX‑like environments. Other variants appeared in FreeDOS (by Dave Dunfield) and ReactOS (by Asif Bahrainwala), but Baker’s version is the one that shaped
how developers think about tree today.

"treeclone" is a small, focused re‑implementation of the classic tree utility. It doesn’t aim to replicate every feature of Steve Baker’s version, and it doesn’t include the dozens of flags and formatting modes that the original tool accumulated over the years. Instead, it concentrates on the core behavior that makes tree useful in the first place: walking a directory, sorting entries, and printing a clean, readable ASCII tree.
The tool supports a single command‑line option, -exclude, which filters out files or directories matching simple patterns. For my workflow, that’s the only feature I actually need. Everything else — depth limiting, color output, metadata, permissions, timestamps — is intentionally left out. The goal is clarity, not completeness.
Internally, treeclone uses a lightweight C kernel with deterministic sorting, directory‑first ordering, and always‑on logging. The output is stable, predictable, and easy to embed into documentation or tooling. It’s a compact utility that does one job well: generate a clean directory tree without external dependencies.

## Why This Exists

Before treeclone, I tried to generate directory trees on macOS using shell scripts. It was clumsy, inconsistent, and never produced output I actually liked. macOS doesn’t ship with the tree command, and every search result pointed to the same solution: install Homebrew. But Homebrew brings a lot with it — packages, dependencies, updates, and a whole ecosystem I didn’t want just to get one simple tool.
I know enough languages to be dangerous — C, Java, PHP, Python — but not enough in any of them to write a perfect clone of the original tree. Still, if I was going to “homebrew” something, I wanted to do it properly. A small, clean C program made the most sense: no dependencies, no package managers, no surprises. Just compile it and run it.
So treeclone exists because I needed a lightweight, predictable way to generate directory trees on macOS without pulling in an entire package manager. It’s not a full replacement for the original tree, and it doesn’t try to be. It’s a simple tool that solves a simple problem — and for my workflow, that’s enough.

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
"@ | Set-Content -Encoding UTF8 README.md

Write-Host "README.md regenerated successfully."