// -------------------------------------------------------------------------------------------
// treeclone.c - Who hasn’t used the UNIX tree command to get a quick overview of a directory?
// treeclone is a compact re‑implementation of that idea — a small C program that walks your
// filesystem and prints a clean, readable tree view of folders and files. It’s fast, minimal,
// and configurable, with sensible defaults and a simple exclusion system. If you want a portable,
// dependency‑free tree tool that behaves exactly the way you expect, treeclone has you covered.
// This c file is the implementation of the treeclone kernel.
// -------------------------------------------------------------------------------------------
// Author:  Patrik Eigenmann
// eMail:   p.eigenmann72@gmail.com
// GitHub:  https://github.com/PatrikEigenmann72/helloc
// -------------------------------------------------------------------------------------------
// Change Log:
// Tue 2026-04-20 File created.                                                 Version: 00.01
// -------------------------------------------------------------------------------------------

#include <stdio.h>     // printf, snprintf
#include <stdlib.h>    // malloc, free
#include <string.h>    // strdup, strtok, strlen, strstr, strcmp, strncpy, strcat
#include <dirent.h>    // DIR, struct dirent, opendir, readdir, closedir
#include <sys/stat.h>  // stat, S_ISDIR
#include <unistd.h>    // PATH_MAX on some systems


#include "samael.huginandmunin.debug.h"   // debug_info()
#include "samael.alchemy.stringutility.h" // str_trim()


#include "treeclone.h"

// ------------------------------------------------------------
// Identity constants
// ------------------------------------------------------------
const int MAJOR = 0;
const int MINOR = 1;
const char *APP_NAME = "treeclone";
const char *APP_EXTRACT_FILTER =
    "*.tmp,tmp/,.vscode,.git/,.DS_Store,bin/,build/";

// -------------------------------------------------------------------------------------------
// is_excluded - Evaluates whether a given filename should be filtered out based on a comma‑
// separated list of exclusion tokens. The logic is intentionally simple and deterministic:
// each token is trimmed and matched using a raw substring search. If the filename contains
// any token, the entry is excluded.
//
// This avoids globbing, regex engines, or hidden matching rules. The behavior is transparent,
// predictable, and easy to reason about—consistent with the UNIX philosophy of simple,
// explicit tools.
//
// @param name        The filename or directory name to evaluate.
// @param patterns    A comma‑separated list of exclusion tokens (may be NULL).
// @return            1 if the entry should be excluded, 0 otherwise.
// -------------------------------------------------------------------------------------------
int is_excluded(const char *name, const char *patterns) {

    if (!patterns) return 0;

    char *copy = strdup(patterns);
    char *token = strtok(copy, ",");

    while (token) {

        str_trim(token); // your string utility

        // If token is empty, skip
        if (strlen(token) > 0) {

            // Substring match
            if (strstr(token, name) != NULL) {
                free(copy);
                return 1;
            }
        }

        token = strtok(NULL, ",");
    }

    free(copy);
    return 0;
}

// -------------------------------------------------------------------------------------------
// walk - Recursively traverses a directory and prints its contents in a clean, human‑readable
// tree structure. This function is the core engine of the application: it collects entries,
// applies exclusion rules, sorts them (directories first, then alphabetical), prints the
// current level, and then descends into subdirectories.
//
// The design is intentionally explicit and deterministic:
//   • No global state is accessed.
//   • No hidden configuration lookups occur.
//   • All behavior (prefix formatting, depth tracking, exclusion filtering) is controlled
//     entirely by the parameters passed in.
//   • Recursion is transparent and predictable.
//
// This keeps the traversal easy to reason about, easy to debug, and consistent with the
// UNIX philosophy of simple, composable tools.
//
// @param path        The filesystem path to traverse (e.g., "./src").
// @param basename    The root directory name (used only for display).
// @param prefix      The accumulated path prefix for nested directories.
// @param depth       Current recursion depth, used for visual indentation.
// @param excludes    The merged exclusion filter string passed down from main().
// -------------------------------------------------------------------------------------------
void walk(const char *path, const char *basename, const char *prefix, int depth, const char *excludes) {

    Entry entries[1024];
    int count = 0;

    struct dirent *entry;
    char full[PATH_MAX];

    debug_info("walk() into: path='%s' prefix='%s' depth=%d", path, prefix, depth);

    DIR *dir = opendir(path);
    if (!dir) {
        debug_info("FAILED to open directory: %s", path);
        return;
    }

    // Collect entries
    while ((entry = readdir(dir)) != NULL) {

        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
            continue;

    if (is_excluded(entry->d_name, excludes))
        continue;

        snprintf(full, sizeof(full), "%s/%s", path, entry->d_name);

        struct stat st;
        if (stat(full, &st) != 0) {
            debug_info("  stat() FAILED for %s", full);
            continue;
        }

        strncpy(entries[count].name, entry->d_name, PATH_MAX);
        entries[count].is_dir = S_ISDIR(st.st_mode);

        debug_info("  collected: %s (%s)",
                   entry->d_name,
                   entries[count].is_dir ? "DIR" : "FILE");

        count++;
    }

    closedir(dir);

    // Sort: directories first, then alphabetical
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {

            int swap = 0;

            if (entries[i].is_dir != entries[j].is_dir) {
                if (entries[j].is_dir)
                    swap = 1;
            } else {
                if (strcmp(entries[i].name, entries[j].name) > 0)
                    swap = 1;
            }

            if (swap) {
                Entry tmp = entries[i];
                entries[i] = entries[j];
                entries[j] = tmp;
            }
        }
    }

    debug_info("  sorted %d entries in '%s'", count, prefix);

    // Print + recurse
    for (int i = 0; i < count; i++) {

        snprintf(full, sizeof(full), "%s/%s", path, entries[i].name);

        int is_last = (i == count - 1);

        // Build visual prefix
        char visual[PATH_MAX] = "";
        for (int d = 0; d < depth; d++) {
            strcat(visual, "│   ");
        }

        // Choose symbol
        const char *symbol = is_last ? "└── " : "├── ";

        if (entries[i].is_dir) {

            debug_info("  DIR: %s -> recurse", entries[i].name);

            printf("%s%s%s/\n", visual, symbol, entries[i].name);

            char newprefix[PATH_MAX];
            snprintf(newprefix, sizeof(newprefix), "%s/%s", prefix, entries[i].name);

            walk(full, basename, newprefix, depth + 1, excludes);

        } else {

            debug_info("  FILE: %s", entries[i].name);

            printf("%s%s%s\n", visual, symbol, entries[i].name);
        }
    }
}