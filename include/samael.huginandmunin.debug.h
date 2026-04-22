/* ****************************************************************************************************
 * samael.huginandmunin.debug.h — Minimalist debugger, a developer’s tool that toggles internal printf
 * -style debug messages, timestamped, colorized with clarity and intention. To toggle debug output on
 * give the -DDEBUG flag at compile time.
 *
 * If DEBUG is defined at compile time, debug macros expand to timestamped, color-coded printf calls.
 * If not, they compile to nothing — zero overhead, zero output. It’s how I narrate processor intent
 * during development without polluting release builds.
 *
 * Insert this header in your c/cpp projects, sprinkle debug_info(), debug_warn() or debug_err() calls
 * where you need insight, and toggle debug output on with a single compile-time flag.
 * ----------------------------------------------------------------------------------------------------
 * Author:      Patrik Eigenmann
 * eMail:       p.eigenmann72@gmail.com
 * GitHub:      www.github.com/PatrikEigenmann/jmake
 * ----------------------------------------------------------------------------------------------------
 * Change Log:
 * Sat 2025-11-15 Added timestamped, color-coded debug macros.                          Version: 00.02
 * Sun 2025-06-22 File created.                                                         Version: 00.01
 * **************************************************************************************************** */
#ifndef SAMAEL_HUGINANDMUNIN_DEBUG_H
#define SAMAEL_HUGINANDMUNIN_DEBUG_H

#include <stdio.h>
#include <time.h>

// Toggle debug mode based on whether DEBUG is defined at compile time.
// This keeps all debug macros lean — either fully active or completely silent.
// One flag, no runtime cost, no surprises.
#ifdef DEBUG
#define DEBUG_MODE 1
#else
#define DEBUG_MODE 0
#endif

// These ANSI color codes are used to colorize Info, Warnings, and Errors differently.
// They improve visual parsing of debug output in supported terminals.
#define COLOR_RESET  "\x1b[0m"
#define COLOR_BLUE   "\x1b[34m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_RED    "\x1b[31m"

// This inline function returns a timestamp string in the format "YYYY-MM-DD HH:MM AM/PM".
// It is used to prefix debug messages with temporal context — a breadcrumb trail for execution.
static inline const char* debug_timestamp() {
    static char buffer[64];
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %I:%M %p", t);
    return buffer;
}

// These macros emit timestamped, color-coded debug messages based on severity level.
// They vanish entirely in release builds — no function calls, no output, no cost.
// Usage: debug_info("x = %d", x); debug_warn("Low memory"); debug_err("Failed to open %s", path);
#if DEBUG_MODE
#define debug_info(fmt, ...) \
    do { printf(COLOR_BLUE  "[INF] %s - " fmt COLOR_RESET "\n", debug_timestamp(), ##__VA_ARGS__); } while (0)

#define debug_warn(fmt, ...) \
    do { printf(COLOR_YELLOW "[WRN] %s - " fmt COLOR_RESET "\n", debug_timestamp(), ##__VA_ARGS__); } while (0)

#define debug_err(fmt, ...) \
    do { printf(COLOR_RED   "[ERR] %s - " fmt COLOR_RESET "\n", debug_timestamp(), ##__VA_ARGS__); } while (0)
#else
#define debug_info(fmt, ...)
#define debug_warn(fmt, ...)
#define debug_err(fmt, ...)
#endif

#endif // SAMAEL_HUGINANDMUNIN_DEBUG_H