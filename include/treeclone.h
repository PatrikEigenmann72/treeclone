// -------------------------------------------------------------------------------------------
// treeclone.h - Who hasn’t used the UNIX tree command to get a quick overview of a directory?
// treeclone is a compact re‑implementation of that idea — a small C program that walks your
// filesystem and prints a clean, readable tree view of folders and files. It’s fast, minimal,
// and configurable, with sensible defaults and a simple exclusion system. If you want a portable,
// dependency‑free tree tool that behaves exactly the way you expect, treeclone has you covered.
// This header file is the kernel definition of the treeclone - the classic UNIX tree tool.
// -------------------------------------------------------------------------------------------
// Author:  Patrik Eigenmann
// eMail:   p.eigenmann72@gmail.com
// GitHub:  https://github.com/PatrikEigenmann72/helloc
// -------------------------------------------------------------------------------------------
// Change Log:
// Tue 2026-04-20 File created.                                                 Version: 00.01
// -------------------------------------------------------------------------------------------
#ifndef TREECLONE_H
#define TREECLONE_H

#include <limits.h>

// ------------------------------------------------------------
// Entry is a simple data structure to define filesystem entries.
// ------------------------------------------------------------
typedef struct {
    char name[PATH_MAX];
    int is_dir;
} Entry;

// ------------------------------------------------------------
// Treeclone-specific functions
// ------------------------------------------------------------
void walk(const char *path, const char *basename, const char *prefix, int depth, const char *excludes);
int is_excluded(const char *name, const char *exclude);

// ------------------------------------------------------------
// Treeclone identity
// ------------------------------------------------------------
extern const int MAJOR;
extern const int MINOR;
extern const char *APP_NAME;
extern const char *APP_EXTRACT_FILTER;

#endif