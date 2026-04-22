// -----------------------------------------------------------------------------------------------
// samael.alchemy.manpage.h - This file exists to give every application a simple, consistent way
// of handling its own manual pages. By defining the structures and functions here, we make sure
// that help text is not scattered or improvised, but instead follows a predictable pattern that
// developers can rely on and contributors can understand.
//
// The purpose is straightforward: applications embed their documentation directly in code, and
// this header provides the tools to check whether that documentation is up to date, manage its
// lifecycle, and ensure it is always available when a user asks for help. The methods defined
// here also dictate how the text is formatted and displayed, enforcing the familiar UNIX style
// so that every application presents its help in a uniform, recognizable way.
//
// In short, this header is about clarity and consistency. It helps technical teams keep their
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
// Tue 2025-12-09 Split ManPage in synopsis, description, options, and license.     Version: 00.03
// Tue 2025-12-09 Decided to go back to simple manpage.                             Version: 00.04
// Tue 2025-12-09 Declared print_help as extern for implementation in main.c.       Version: 00.05
// Wed 2025-12-10 Fixed a memory issue bug in isHelpTriggered.                      Version: 00.06
// Wed 2025-12-10 Restructured the whole header file for better clarity.            Version: 00.07
// Thu 2025-12-11 Privatized manpage_write and manpage_free in .c file.             Version: 00.08
// Thu 2025-12-11 Simplified manpage_init to void, handled internally.              Version: 00.09
// Thu 2025-12-11 Updated manpage_is_help_triggered to accept help flags anywhere.  Version: 00.10
// -----------------------------------------------------------------------------------------------
#ifndef SAMAEL_ALCHEMY_MANPAGE_H
#define SAMAEL_ALCHEMY_MANPAGE_H

// ----------------------------------------------------------------------------------------------
// ManPage - This struct defines the container for all sections of a manual page. It keeps the
// documentation organized in one place, so help text is not scattered across the codebase.
//
// The purpose is to make creation and maintenance predictable. Every application can present
// its help in the familiar UNIX style, while remaining portable across platforms. Developers
// gain clarity, and users benefit from consistent, accessible documentation.
// ----------------------------------------------------------------------------------------------
typedef struct ManPage ManPage;

// ----------------------------------------------------------------------------------------------
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
    char *licenseIn);

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
int manpage_is_help_triggered(int argcIn, char *argvIn[]);

// ---------------------------------------------------------------------------------------
// manpage_display - This function displays a familiar UNIX-style manpage tailored for your
// application. It ensures that users can quickly access structured documentation without
// confusion, following the same predictable format every time.
//
// Note: This function is declared here but must be implemented in main.c, because each
// application has its own specific content to present. The module enforces the structure,
// while the application provides the details.
// ---------------------------------------------------------------------------------------
extern void manpage_display();
#endif