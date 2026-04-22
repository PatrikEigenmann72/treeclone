// -----------------------------------------------------------------------------------------------
// samael.alchemy.manpage.c - This library exists to give every application a simple, consistent
// way of handling its own manual pages. By defining the structures and functions here, we make
// sure that help text is not scattered or improvised, but instead follows a predictable pattern
// that developers can rely on and contributors can understand.
//
// The purpose is straightforward: applications embed their documentation directly in code, and
// this library provides the tools to check whether that documentation is up to date, manage its
// lifecycle, and ensure it is always available when a user asks for help. The methods defined
// here also dictate how the text is formatted and displayed, enforcing the familiar UNIX style
// so that every application presents its help in a uniform, recognizable way.
//
// In short, this library is about clarity and consistency. It helps technical teams keep their
// codebases clean, while also reassuring non‑technical stakeholders that users will always find
// accessible guidance without extra dependencies or complicated setups.
// -----------------------------------------------------------------------------------------------
// Author:      Patrik Eigenmann
// eMail:       p.eigenmann@gmx.net
// GitHub:      www.github.com/PatrikEigenmann72/helloc
// -----------------------------------------------------------------------------------------------
// Change Log:
// Sun 2025-06-23 File created.                                                     Version: 00.01
// Tue 2025-12-09 Split append_format into samael.alchemy.stringutility module.     Version: 00.02
// Tue 2025-12-09 Implemented samael.huginandmunin.debug.h for debug messages.      Version: 00.03
// Wed 2025-12-10 Fixed a memory issue bug in isHelpTriggered.                      Version: 00.04
// Thu 2025-12-11 Standardized function naming to {module}_{function} pattern.      Version: 00.05
// Thu 2025-12-11 Separated file into public API and private helper section.        Version: 00.06
// Thu 2025-12-11 Refactored doesFileExist into static char *manpage_read.          Version: 00.07
// Thu 2025-12-11 Refactored isHelpTriggered into int manpage_is_help_triggered.    Version: 00.08
// Thu 2025-12-11 Refactored create_manpage into void manpage_init.                 Version: 00.09
// Thu 2025-12-11 Added private helper static void manpage_write.                   Version: 00.10
// Thu 2025-12-11 Added private helper static char *manpage_explode.                Version: 00.11
// Thu 2025-12-11 Added private helper static void manpage_free.                    Version: 00.12
// -----------------------------------------------------------------------------------------------
// Include standard headers.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// Include custom headers.
#include "samael.alchemy.manpage.h"
#include "samael.alchemy.stringutility.h"

// ----------------------------------------------------------------------------------------------
// Platform Setup - This section handles the differences between Windows and Unix-like systems.
// The goal is simple: every application should be able to store and display its manual page
// without worrying about which operating system it runs on.
//
// On Windows:
//   - We include <io.h> for file access.
//   - The user's home directory is taken from USERPROFILE.
//   - Manual pages are stored under \AppData\Local\.
//   - The display command is "more", which is the Windows equivalent of "less".
//
// On Unix/MacOS:
//   - We include <unistd.h> for file access.
//   - The user's home directory is taken from HOME.
//   - Manual pages are stored under ~/.local/share/.
//   - The display command is "less", which is the familiar pager in the Unix world.
//
// In short, this block abstracts away platform quirks. Developers can rely on the same
// behavior everywhere: manual pages are written to a predictable location and displayed
// with the appropriate pager command.
// ----------------------------------------------------------------------------------------------
#ifdef _WIN32
    #include <io.h>
    #define _home() getenv("USERPROFILE")
    char command[255] = "more ";
    char *PATH = "\\AppData\\Local\\man\\";
#else
    #include <unistd.h>
    #define _home() getenv("HOME")
    char command[255] = "less ";
    char *PATH = "/.local/share/man/";
#endif

// ------------------------------------------------------------------------------------------
// ManPage - This struct is the single container for a manual page. It gathers all the pieces
// of documentation into one predictable unit, so developers don’t scatter help text across
// the codebase. The design is simple: public fields hold the visible sections of the manual,
// while private fields manage the internals automatically. Outsiders only need to know that
// the struct exists and that it guarantees a complete, uniform manual page when used through
// the public API.
// ------------------------------------------------------------------------------------------
struct ManPage {
    // Pubic properties
    int major;
    int minor;
    char *name;
    char *synopsis;
    char *description;
    char *options;
    char *license;

    // Private properties
    char *filename;
};

// Forward declarations of private helpers
static char *manpage_explode(ManPage *mp);
static void  manpage_write(const char *manual, const char *filename);
static char *manpage_read(const char *filename);
static void  manpage_free(ManPage *mp);

/* ---------------------- Public Functions (declared in the header) ------------------------- */
// ------------------------------------------------------------------------------------------
// manpage_init - This function initializes the manual page by gathering all essential sections
// of documentation into one structured unit. By passing in the version numbers, name, synopsis,
// description, options, and license, you ensure that the resulting manual page is complete,
// consistent, and ready to be displayed in the familiar UNIX style.
//
// The purpose is to prevent help text from being scattered across the codebase. Instead,
// everything is initialized here in a predictable way, which makes maintenance easier for
// developers and guarantees that users always see a polished, accessible manual page when
// they ask for help.
//
// @param int majorIn           - Application's major version number.
// @param int minorIn           - Application's minor version number.
// @param char *nameIn          - Name of the application.
// @param char *synopsisIn      - The synopsis section of the manual page.
// @param char *descriptionIn   - The description section of the manual page.
// @param char *optionsIn       - The options section of the manual page.
// @param char *licenseIn       - The license section of the manual page.
// ----------------------------------------------------------------------------------------------
void manpage_init(int majorIn,
    int minorIn,
    char *nameIn,
    char *synopsisIn,
    char *descriptionIn,
    char *optionsIn,
    char *licenseIn) {

    // Initialize the ManPage struct with the provided parameters
    ManPage mp;
    mp.major = majorIn;
    mp.minor = minorIn;
    mp.name = strdup(nameIn);
    mp.synopsis = strdup(synopsisIn);
    mp.description = strdup(descriptionIn);
    mp.options = strdup(optionsIn);
    mp.license = strdup(licenseIn);
    
    // Build the filename based on the application name.
    mp.filename = NULL;
    append_format(&mp.filename, _home());
    append_format(&mp.filename, PATH);
    append_format(&mp.filename, "%s.man", mp.name);

    char *manual = manpage_explode(&mp);
    char *exist = manpage_read(mp.filename);

    // If the file does not exist or the content has changed, update it.
    // exist will never be NULL because manpage_read() will return an empty
    // string in case of the files absence.
    if (strcmp(manual, exist) != 0) {
        manpage_write(manual, mp.filename);
    }

    free(exist);
    free(manual);


    // Display the manual page using the appropriate command
    strcat(command, mp.filename);
    system(command);

    // Free the allocated memory
    manpage_free(&mp);
}

// ---------------------------------------------------------------------------------------
// manpage_is_help_triggered - This function checks whether the user has requested help by
// scanning the entire command-line argument array for a help trigger. It recognizes flags
// like -h, -H, -help, -Help, or -?, no matter where they appear in the option order. This
// approach reflects how people actually use command-line tools: they may start typing
// options they want, then decide to ask for help without reordering or deleting what they
// already wrote. By allowing the help flag anywhere, the application feels more natural
// and forgiving, rather than rigid or robotic.
//
// Note: Changed argv from char* to char*[] to fix a memory issue.
//
// @param int argcIn - The number of command-line arguments.
// @param char *argvIn[] - The array of command-line arguments.
// @return int - Returns 1 if help is triggered, 0 otherwise.
// ---------------------------------------------------------------------------------------
int manpage_is_help_triggered(int argcIn, char *argvIn[]){
    // Safety guard: no arguments beyond program name
    if (argcIn == 1) {
        return 0;
    }

    // Scan through all arguments
    for (int i = 1; i < argcIn; i++) {
        if (strcmp(argvIn[i], "-h") == 0 ||
            strcmp(argvIn[i], "-H") == 0 ||
            strcmp(argvIn[i], "-help") == 0 ||
            strcmp(argvIn[i], "-Help") == 0 ||
            strcmp(argvIn[i], "-?") == 0) {
            return 1; // help flag found anywhere
        }
    }

    return 0; // no help flag present
}

/* ---------- Private Functions (static functions for internal usage only) --------------- */

// -----------------------------------------------------------------------------------------
// manpage_explode - This helper assembles all sections into one coherent string. It ensures
// the documentation is complete and properly formatted before the next steps handle storage
// or display. In the overall flow, this is the point where scattered fields become a single,
// predictable manual page.
// 
// @param ManPage *mp - Pointer to the struct containing all sections.
// @return char* - The assembled manual page content.
// -----------------------------------------------------------------------------------------
static char *manpage_explode(ManPage *mp) {
    // Create the manual content
    char *manual = NULL;

    append_format(&manual, "NAME:\n");
    append_format(&manual, "      %s - Version: %02d.%02d\n", mp->name, mp->major, mp->minor);
    append_format(&manual, "\n");
    append_format(&manual, "SYNOPSIS:\n");
    append_format(&manual, "%s\n", mp->synopsis);
    append_format(&manual, "\n");
    append_format(&manual, "DESCRIPTION:\n");
    append_format(&manual, "%s\n", mp->description);
    append_format(&manual, "\n");
    append_format(&manual, "OPTIONS:\n");
    append_format(&manual, "%s\n", mp->options);
    append_format(&manual, "\n");
    append_format(&manual, "LICENSE:\n");
    append_format(&manual, "%s\n", mp->license);

    return manual;
}

// -----------------------------------------------------------------------------------------
// manpage_write - This function writes the manual page content to a file. It ensures that the
// manual page is saved in a consistent format and location, which is essential for users to
// easily access and refer to it.
//
// @param const char *manual - The manual page content to be written.
// @param const char *filename - The name of the file where the manual page will be
// written.
// -----------------------------------------------------------------------------------------
static void manpage_write(const char *manual, const char *filename) {
        FILE *file = fopen(filename, "w");
    
        if (file == NULL) {
            printf("Error opening file!\n");
            return;
        }

        fprintf(file, "%s", manual);

        // Now we can close the file.
        fclose(file);
}

// -----------------------------------------------------------------------------------------
// manpage_read - Reading a manpage file is a recurring task. Instead of duplicating code,
// this helper centralizes the logic: it opens the file and returns its contents as a string.
// 
// @param const char *filename - The name of the file to read from.
// @return char* - The file contents as a string.
// -----------------------------------------------------------------------------------------
static char *manpage_read(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        // Return an allocated empty string
        char *empty = malloc(1);
        if (empty) empty[0] = '\0';
        return empty;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char *buffer = malloc(size + 1);
    if (!buffer) {
        fclose(fp);
        // Return an allocated empty string
        char *empty = malloc(1);
        if (empty) empty[0] = '\0';
        return empty;
    }

    fread(buffer, 1, size, fp);
    buffer[size] = '\0';
    fclose(fp);

    return buffer;
}

// -----------------------------------------------------------------------------------------
// manpage_free - Freeing allocated fields of a manpage is a recurring task. Instead of
// repeating cleanup code, this helper centralizes the logic: it releases memory tied to the
// struct so the application stays predictable and leak-free.
//
// @param ManPage *mp - Pointer to the struct containing all sections.
// -----------------------------------------------------------------------------------------
static void manpage_free(ManPage *mp) {
    // Clean up public properties
    mp->major = 0;
    mp->minor = 0;
    if(mp->name) free(mp->name);
    if(mp->synopsis) free(mp->synopsis);
    if(mp->description) free(mp->description);
    if(mp->options) free(mp->options);
    if(mp->license) free(mp->license);

    // Clean up private properties
    if(mp->filename) free(mp->filename);
}