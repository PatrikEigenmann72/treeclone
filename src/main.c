// -------------------------------------------------------------------------------------------
// main.c - The heart of the software project. This is where the magic begins, and where the
// journey starts. For a C program, this is the entry point that brings everything together.
// It handles command-line arguments, manages the flow of execution, and coordinates the
// various components that make up the whole. This file is the central hub from which the
// program operates, ensuring that all parts work in harmony to achieve the desired
// functionality.
//
// Compile instructions:
// gcc/clang -DDEBUG -Wall -Wextra -I./include -o ./bin/treeclone src/*.c       (debug build)
// gcc/clang -Wall -Wextra -I./include -o ./bin/treeclone src/*.c               (release build)
// pmake helloc.pmake                                                           (pmake build)
// -------------------------------------------------------------------------------------------
// Author:  Patrik Eigenmann
// eMail:   p.eigenmann72@gmail.com
// GitHub:  https://github.com/PatrikEigenmann72/helloc
// -------------------------------------------------------------------------------------------
// Change Log:
// Mon 2026-04-20 File created.                                                 Version: 00.01
// Mon 2026-04-20 Added basic CLI parsing and initial debug/log initialization. Version: 00.02
// Mon 2026-04-20 Integrated Samael logging + debug macros.                     Version: 00.03
// Tue 2026-04-21 Added config_init() with default exclusion filter.            Version: 00.04
// Tue 2026-04-21 Added initial walk() call (non-recursive placeholder).        Version: 00.05
// Tue 2026-04-21 Fixed walk() parameter mismatch and missing continue bug.     Version: 00.06
// Tue 2026-04-21 Added exclusion-aware logic and passed filter to walk().      Version: 00.07
// Tue 2026-04-21 Cleaned up main(): removed inline recursion, delegated to
//                treeclone kernel (walk + is_excluded).                        Version: 00.08
// Tue 2026-04-21 Moved Entry struct and treeclone-kernel in treeclone.h/.c.    Version: 00.09
// Wed 2026-04-22 Finalized main.c as pure orchestrator.                        Version: 00.10
// -------------------------------------------------------------------------------------------

/* Include the standard libraries */
#include <stdio.h>      // printf, perror
#include <stdlib.h>     // EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>     // strcmp, snprintf
#include <dirent.h>     // DIR, opendir, readdir, closedir
#include <sys/stat.h>   // stat, S_ISDIR
#include <limits.h>     // PATH_MAX
#include <unistd.h>     // getcwd

/* Include the Samael libraries */
#include "samael.huginandmunin.debug.h"             // Minimalist debugger header.
#include "samael.chronicle.config.h"                // Configuration manager header.
#include "samael.alchemy.stringutility.h"           // String utility functions header.
#include "samael.alchemy.manpage.h"                 // ManPage generation and display header.
#include "samael.huginandmunin.log.h"               // Logging system header.

/* Include the treeclone files */
#include "treeclone.h"

// -------------------------------------------------------------------------------------------
// config_init - This function is required in every project that uses the Samael framework,
// specifically the samael.chronicle.config.h module. It defines the initial set of
// configuration values the application needs at startup. Developers implement config_init()
// to add key=value pairs using the config_add() API, such as application name, version,
// labels, numeric values, or other parameters.
//
// The purpose of config_init() is to provide a single, centralized place where an application
// declares its important settings. This makes configuration easy to understand, maintain,
// and adjust over time.
//
// @note This function fulfills the extern declaration in samael.chronicle.config.h. The
//       framework does not provide a default implementation; each application must define it.
// -------------------------------------------------------------------------------------------
void config_init(void) {
    
    // Add configuration entries using the public API.
    log_info("Add configuration entries using the public API.");
    debug_info("Add configuration entries using the public API.");

    config_add("App.ExtractFilter", "*.tmp, tmp/, .vscode, .git/, .DS_Store, bin/, build/");
    log_info("Added App.ExtractFilter -> *.tmp, tmp/, .vscode, .git/, .DS_Store");
    debug_info("Added App.ExtractFilter -> *.tmp, tmp/, .vscode, .git/, .DS_Store");
}

// -------------------------------------------------------------------------------------------
// manpage_display
//
// Builds and displays the application's integrated manpage. This function assembles all
// sections—name, synopsis, description, options, and license—into a complete help page and
// delegates the final rendering to manpage_init(). The content is constructed dynamically
// using append_format(), which keeps the layout flexible and easy to maintain.
//
// The design mirrors the UNIX manpage structure so developers and system administrators feel
// immediately at home. This function is only invoked when help flags are detected, and its
// output replaces normal program execution.
//
// @note Memory for all dynamically built sections is allocated here and freed before return.
// -------------------------------------------------------------------------------------------
void manpage_display() {
    
    log_info("Assembling and displaying the manpage content.");
    debug_info("Assembling and displaying the manpage content.");
    
    int major = MAJOR;
    log_info("Setting major version to %02d.", major);
    debug_info("Setting major version to %02d.", major);
    
    int minor = MINOR;
    log_info("Setting minor version to %02d.", minor);
    debug_info("Setting minor version to %02d.", minor);

    // Define the filename of the manpage content
    char *name = NULL;
    log_info("Setting application name to NULL.");
    debug_info("Setting application name to NULL.");

    append_format(&name, APP_NAME);
    log_verbose("Appended application name: %s.", name);
    debug_info("Appended application name: %s.", name);

    char *synopsis = NULL;
    log_info("Setting synopsis to NULL.");
    debug_info("Setting synopsis to NULL.");

    append_format(&synopsis, "      treeclone [-exclude] [-h | -H | -help | -Help | -(\\)?]");
    log_info("Building synopsis: treeclone [-exclude] [-h | -H | -help | -Help | -(\\)?]");
    debug_info("Building synopsis: treeclone [-exclude] [-h | -H | -help | -Help | -(\\)?]");

    char *description = NULL;
    log_info("Setting description to NULL.");
    debug_info("Setting description to NULL.");

    append_format(&description, "      treeclone - as the name suggests, this is a simple clone of the CLI tree.\n");
    log_info("Built description content.");
    debug_info("Built description content.");

    char *options = NULL;
    log_info("Setting options to NULL.");
    debug_info("Setting options to NULL");

    append_format(&options, "      -exclude \"*.tmp, .git/, tmp/\"\n");
    append_format(&options, "      Any folder or file, one time or reoccuring you define after the -exclude\n");
    append_format(&options, "      flag in a comma seperated string will be extracted from the tree.\n");
    append_format(&options, "\n");
    append_format(&options, "      -h, -H, -help, -Help, -(\\)?\n");
    append_format(&options, "      Do you need help? Any of these flags will open the application's manpage.\n");
    append_format(&options, "      This UNIX-style help file, familiar to developers and system administrators,\n");
    append_format(&options, "      is integrated into helloc itself. The beauty of this approach is that anyone\n");
    append_format(&options, "      working on macOS, BSD, UNIX, or Linux will instantly feel at home with the layout.\n");
    append_format(&options, "      Think of it as your built-in guide whenever you need more insight into the\n");
    append_format(&options, "      program helloc.\n");
    log_info("Built options content.");
    debug_info("Built options content.");

    char *license = NULL;
    log_info("Setting license to NULL.");
    debug_info("Setting license to NULL.");

    append_format(&license, "      Copyright 2024 Free Software Foundation, Inc. License GPLv3+: GNU GPL version 3\n");
    append_format(&license, "      or later <https://gnu.org/licenses/gpl.html>. This is free software: you are free\n");
    append_format(&license, "      to change and redistribute it. There is NO WARRANTY, to the extent permitted by law.\n");
    log_info("Built license content.");
    debug_info("Built license content.");

    // Create the manpage in the file
    log_info("Initializing manpage with assembled content.");
    debug_info("Initializing manpage with assembled content.");
    manpage_init(major, minor, name, synopsis, description, options, license);

    // Free up the memory.
    free(name);             
    log_info("Freed memory allocated for name.");
    debug_info("Freed memory allocated for name.");
    
    free(synopsis);         
    log_info("Freed memory allocated for synopsis.");
    debug_info("Freed memory allocated for synopsis.");

    free(description);      
    log_info("Freed memory allocated for description.");
    debug_info("Freed memory allocated for description.");

    free(options);          
    log_info("Freed memory allocated for options.");
    debug_info("Freed memory allocated for options.");

    free(license);          
    log_info("Freed memory allocated for license.");
    debug_info("Freed memory allocated for license.");
}

// -------------------------------------------------------------------------------------------
// main - This function marks the beginning of execution for any C program. It serves as the
// central coordinator, handling command-line arguments, initializing subsystems, and directing
// the overall flow of the application. From here, the program invokes its core routines,
// manages resources, and produces output. The main function embodies the universal convention
// of C: every application, regardless of purpose or domain, begins its journey here.
//
// @param argc   The number of command-line arguments passed to the program.
// @param argv   The array of command-line argument strings.
// @return       EXIT_SUCCESS (0) on successful execution, or an error code otherwise.
// -------------------------------------------------------------------------------------------
int main(int argc, char **argv) {

    // The very first thing an application needs is a working logging system.
    log_init(LOG_ALL);  // Initialize logging system with all levels enabled.

    // After that, we need to initialize the configuration if it has one.
    config_init();

    // And now, let's see if the manpage is triggered, because if it is, the display of
    // the manpage will end the application on QTRL + Z I believe.
    if (manpage_is_help_triggered(argc, argv)) {
        manpage_display();
        return EXIT_SUCCESS;
    }
    
    // After all that, we can actually start the application and start with the initialization
    // of some properties and variables. Also, debugging and logging is ready to go.
    debug_info("Start the application %s.", APP_NAME);
    log_info("Start the application %s.", APP_NAME);
    
    // Define and initialize if necessary all variables used in the project.
    Entry entries[1024];                        // Array of entries. An entry can be a folder or a file.

    // check if the parent (active directory) is a folder!
    // If not, there is something estancially wrong with the file system.
    DIR *dir = opendir(".");
    if (!dir) {
        debug_err("Active directory is not a folder!");
        log_err("Active directory is not a folder!");
        return EXIT_FAILURE;
    }

    struct dirent *entry;                       // Directory entry.
    char path[PATH_MAX];                        // Absolut path.

    char cwd[PATH_MAX];                         // Relative path.
    getcwd(cwd, sizeof(cwd));                   // Getting the active directory

    char *basename = strrchr(cwd, '/');         // Storing the active directory name in basename/
    basename = basename ? basename + 1 : cwd;   // If basename exists, skip past the slash; otherwise use cwd as fallback 

    debug_info("Active directory: %s", basename);
    log_info("Active directory: %s", basename);

    char *cli_excludes = NULL;                  // Let's start preparing for -exclude

    // Iterate throuhg the command line parameter.
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-exclude") == 0 && i + 1 < argc) {
            cli_excludes = argv[i + 1];
            debug_info("CLI exclusion filter detected: %s", cli_excludes);
            log_info("CLI exclusion filter detected: %s", cli_excludes);
        }
    }

    // Get the standard excludes from the config memory.
    const char *default_excludes = config_get_string("App.ExtractFilter");

    // We need this final string to merge the config filters and the command line filters.
    char *final_excludes = NULL;

    // Here is where the merge happens
    if (default_excludes && cli_excludes) {
        append_format(&final_excludes, "%s,%s", default_excludes, cli_excludes);
    } else if (cli_excludes) {
        append_format(&final_excludes, "%s", cli_excludes);
    } else if (default_excludes) {
        append_format(&final_excludes, "%s", default_excludes);
    }

    debug_info("Final exclusion filter: %s", final_excludes);
    log_info("Final exclusion filter: %s", final_excludes);

    // Store merged filter back into config so it is ready for walk().
    if (final_excludes) {
        debug_info("Rewrite exclusion filter: %s in config", final_excludes);
        log_info("Rewrite exclusion filter: %s in config", final_excludes);
        config_add("App.ExtractFilter", final_excludes);
    }

    // Here we start with variables used for the application purpose.
    int count = 0;                              // This is the level of subfolders we are in.
    const char *excludes = final_excludes;      // convenience alias.

    // We are entering the main loop for the tree building sequence.
    debug_info("Entering the main loop for tree building in memory.");
    log_info("Entering the main loop for tree building in memory.");
    while ((entry = readdir(dir)) != NULL) {

        // Calling a dir or ls -l the first two entries are just the active directory
        // and the directory before that. So, cd .. works. We don't need that. The
        // Tree should start cleanly with the active directory. {Please notice, I do
        // not call it project, because project can be a file, active directory is always a
        // directory.}
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0){
            debug_info(". .. Exclusion");
            log_info(". .. Exclusion");
            continue;
        }

        // Now we have enough information about the filters, here we apply all the exclusion
        // filters, so that unwanted folders and files are not shown in a clean tree display.
        if (is_excluded(entry->d_name, excludes)) {
            debug_info("Check exclusions: %s with %s.", entry->d_name, excludes);
            log_info("Check exclusions: %s with %s", entry->d_name, excludes);
            continue;
        }

        // Safely build a formatted path string like "path/filename" without overflowing memory.
        snprintf(path, sizeof(path), "./%s", entry->d_name);

        struct stat st;                                         // Create a stat struct.
        if (stat(path, &st) != 0){
            debug_info("Check status of %s went wrong.", path);
            log_info("Check status of %s went wrong.", path);
            continue;
        }

        debug_info("Add entry %s to the tree.", entry->d_name);
        log_info("Add entry %s to the tree.", entry->d_name);
        strncpy(entries[count].name, entry->d_name, PATH_MAX);  // Copy filename into storage.
        entries[count].is_dir = S_ISDIR(st.st_mode);            // Check if it is a directory
        debug_info("Increase counter %d by 1.", count);
        log_info("Increase counter %d by 1.", count);
        count++;    // Tree branch done, move on.
    }

    debug_info("Close directory");
    log_info("Close directory");
    closedir(dir);  // Close the directory we are in.

    // It performs a stable, explicit, two‑pass comparison over the entries[] array to ensure:
    // 1.  Directories come before files.
    // 2.  Names are sorted alphabetically within each group.
    debug_info("Applying a naive O(n²) swap sort. It's dumb, but directories are small.");
    log_info("Applying a naive O(n²) swap sort. It's dumb, but directories are small.");
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
                debug_info("Swap entries: %s with %s.", entries[i].name, entries[j].name);
                log_info("Swap entries: %s with %s.", entries[i].name, entries[j].name);
                Entry tmp = entries[i];
                entries[i] = entries[j];
                entries[j] = tmp;
            }
        }
    }

    // The entries are checked and sorted, now we can start displaying the result.
    // We start with the base (root/project) name.
    debug_info("Uff, after all that we finally get to output the tree starting with %s.", basename);
    log_info("Uff, after all that we finally get to output the tree starting with %s.", basename);
    printf("%s/\n", basename);

    // And now we are moving into the hirarchical structure of the active directory.
    for (int i = 0; i < count; i++) {

        // Safely build a formatted path string like "path/filename" without overflowing memory.
        snprintf(path, sizeof(path), "./%s", entries[i].name);

        int is_last = (i == count - 1);                             // Check if it is the last folder.
        const char *symbol = is_last ? "└── " : "├── ";             // Nice symbols to create a human readable tree.

        // check if the entry is a directory.
        if (entries[i].is_dir) {
            // If it is a directory, print it as a directory.
            debug_info("Directory: %s.", entries[i].name);
            log_info("Directory: %s.", entries[i].name);
            printf("%s%s/\n", symbol, entries[i].name);
            // And then walk recursively throught the new directory.
            walk(path, basename, entries[i].name, 1, excludes);
        } else {
            // If it is a file, just display it as a file.
            debug_info("File: %s.", entries[i].name);
            log_info("File: %s.", entries[i].name);
            printf("%s%s\n", symbol, entries[i].name);
        }
    }

    // Whole program went through and was successfull.
    return EXIT_SUCCESS;
}