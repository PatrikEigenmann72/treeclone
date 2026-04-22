// -------------------------------------------------------------------------------------------
// samael.huginandmunin.log.c - This is a lightweight logging system for C applications. It
// provides macros for logging messages at various levels (verbose, info, warning, error),
// automatically including contextual information such as file name, function name, and line
// number. This module is designed to help developers track application flow and diagnose
// issues effectively.
// -------------------------------------------------------------------------------------------
// Author:      Patrik Eigenmann
// eMail:       p.eigenmann72@gmail.com
// GitHub:      www.github.com/PatrikEigenmann72/helloc
// -------------------------------------------------------------------------------------------
// Change Log:
// Sat 2026-01-04 File created.                                                 Version: 00.01
// Tue 2026-01-13 Corrected log_get_timestamp to match Java applications.       Version: 00.02
// Sat 2026-01-24 Added sophisticated comments and documentation.               Version: 00.03
// Sat 2026-01-24 Added #pragma regions for better code organization.           Version: 00.04
// -------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

#include "samael.huginandmunin.log.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <libgen.h>
#include <unistd.h>
#include <mach-o/dyld.h>
#include <sys/time.h>
#endif

#pragma region Internal Definitions
// -------------------------------------------------------------------------------------------
// log_mask - Central control variable for the logging subsystem. Instead of maintaining separate
// flags for each severity level, this integer holds a bitmask that encodes which categories of
// messages should be recorded. The design allows developers to enable or disable multiple levels
// simultaneously with a single assignment, providing flexible granularity in diagnostic output.
//
// Bit assignments are defined by constants:
// - Bit 0 (0x01) → Verbose: detailed flow and debugging information
// - Bit 1 (0x02) → Info: general operational messages
// - Bit 2 (0x04) → Warning: non-fatal issues requiring attention
// - Bit 3 (0x08) → Error: critical failures or unexpected conditions
//
// Example usage:
// - log_mask = LOG_VERBOSE | LOG_WARN;
//      → Enables verbose tracing and warnings, while suppressing info and error logs.
//
// - LOG_ALL (0x0F) activates every level, ensuring complete visibility.
// - LOG_NONE (0x00) disables all logging entirely.
// -------------------------------------------------------------------------------------------
static int log_mask = LOG_ALL;
#pragma endregion

#pragma region Internal Forward Declarations
// -------------------------------------------------------------------------------------------
// log_get_timestamp - Produces a formatted timestamp string representing the current local
// time with millisecond precision. This helper is used by the logging system to annotate each
// entry with the exact moment of occurrence, ensuring chronological traceability. It might be
// thread-unsafe due to the use of a static buffer.
//
// @return A pointer to a static buffer containing the timestamp in the format "hh:mm:ss.mmm".
//         The buffer is reused on each call, so the value must be consumed immediately or copied
//         if persistence is required.
// -------------------------------------------------------------------------------------------
static const char* log_get_timestamp(void);

// -------------------------------------------------------------------------------------------
// log_get_filename - Determines the absolute path for the log file associated with the current
// executable. This helper constructs a path under the user's "Documents/Logs" directory (on
// Windows or POSIX systems) and names the file after the executable, ensuring that each
// application maintains its own distinct log file. By tying the log name to the binary, the
// system guarantees separation of diagnostic output across different programs.
//
// @return A pointer to a static buffer containing the full path to the log file. The buffer is
//         reused on each call, so the value must be consumed immediately or copied if persistence
//         is required. If the user's home directory cannot be resolved, a relative path is used
//         as a fallback to maintain logging continuity.
// -------------------------------------------------------------------------------------------
static const char* log_get_filename(void);
#pragma endregion

#pragma region Public API Implementations
// -------------------------------------------------------------------------------------------
// log_init - Initializes the logging subsystem by setting the active log level mask and
// preparing the log file for new entries. This procedure ensures that subsequent logging
// operations respect the caller’s chosen verbosity and that old diagnostic data does not
// interfere with the current session. The log file is created (or truncated if it already
// exists) under the user’s Documents/Logs directory, named after the executable. This
// function must be called once at application startup before any logging macros are used.
//
// @param levelMaskIn An integer bitmask composed of LOG_VERBOSE, LOG_INFO, LOG_WARN, and
//                    LOG_ERR flags. This mask determines which categories of messages will
//                    be recorded. For example, LOG_INFO | LOG_ERR enables informational and
//                    error logs while suppressing verbose and warning output.
// -------------------------------------------------------------------------------------------
void log_init(int levelMaskIn) {
    log_mask = levelMaskIn;

    const char* filename = log_get_filename();
    FILE* f = fopen(filename, "w"); // truncate existing file
    if (f) fclose(f);
}

// -------------------------------------------------------------------------------------------
// log_write_internal - Core routine of the logging subsystem. This procedure records a single
// log entry to the application’s log file, combining contextual metadata with a formatted
// message. Each entry is annotated with a timestamp, severity level, source file, function,
// and line number, providing developers with precise diagnostic information. Messages are
// only written if the requested severity level is enabled in the current log_mask.
//
// @param levelIn   Integer bitmask flag indicating the severity category of the message.
//                  Must correspond to one of LOG_VERBOSE, LOG_INFO, LOG_WARN, or LOG_ERR.
//
// @param levelStr  Short string label (e.g., "INF", "ERR") used to identify the severity
//                  in the log output.
//
// @param fileIn    Name of the source file where the log macro was invoked.
// @param funcIn    Name of the function where the log macro was invoked.
// @param lineIn    Line number in the source file where the log macro was invoked.
// @param fmt       printf-style format string describing the message content.
// @param ...       Variadic arguments matching the format specifiers in fmt.
// -------------------------------------------------------------------------------------------
void log_write_internal(int levelIn, const char* levelStr,
                        const char* fileIn, const char* funcIn, int lineIn,
                        const char* fmt, ...) {
    if (!(log_mask & levelIn)) return;

    char buffer[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    const char* filename = log_get_filename();
    FILE* f = fopen(filename, "a");
    if (!f) return;

    fprintf(f, "%s [%s] [%s:%s:%d] - %s\n", log_get_timestamp(),
        levelStr, fileIn, funcIn, lineIn, buffer);

    fclose(f);
}
#pragma endregion

#pragma region Internal Helper Implementations
// -------------------------------------------------------------------------------------------
// log_get_timestamp - Produces a formatted timestamp string representing the current local time
// with millisecond precision. This helper is used by the logging system to annotate each entry
// with the exact moment of occurrence, ensuring chronological traceability across log files.
// The implementation is platform-aware: on Windows it queries the SYSTEMTIME structure, while
// on POSIX systems it uses gettimeofday combined with localtime for conversion.
//
// @return buffer → A pointer to a static character buffer containing the timestamp in the
//                  format "hh:mm:ss.mmm". The buffer is reused on each call, so the value
//                  must be consumed immediately or copied if persistence is required.
// -------------------------------------------------------------------------------------------
static const char* log_get_timestamp(void) {
    static char buffer[64];

#ifdef _WIN32
    SYSTEMTIME st;
    GetLocalTime(&st);
    snprintf(buffer, sizeof(buffer),
             "%02d:%02d:%02d.%03d",
             st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm* t = localtime(&tv.tv_sec);
    snprintf(buffer, sizeof(buffer),
             "%02d:%02d:%02d.%03d",
             t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec / 1000);
#endif

    return buffer;
}

// -------------------------------------------------------------------------------------------
// log_get_filename - Determines the absolute path for the log file associated with the current
// executable. This helper constructs a path under the user's "Documents/Logs" directory and
// names the file after the executable, ensuring that each application maintains its own
// distinct log file. By tying the log name to the binary, the system guarantees separation
// of diagnostic output across different programs. The implementation is platform-aware:
// on Windows it queries the executable name via GetModuleFileNameA, while on POSIX systems
// it uses _NSGetExecutablePath combined with basename for extraction.
//
// @return buffer → A pointer to a static character buffer containing the full path to the
//                  log file. The buffer is reused on each call, so the value must be consumed
//                  immediately or copied if persistence is required. If the user's home
//                  directory cannot be resolved, a relative path is used as a fallback to
//                  maintain logging continuity.
// -------------------------------------------------------------------------------------------
static const char* log_get_filename(void) {
    static char path[512];
#ifdef _WIN32
    char exeName[MAX_PATH];
    GetModuleFileNameA(NULL, exeName, sizeof(exeName));
    char* base = strrchr(exeName, '\\');
    const char* exeBase = base ? base + 1 : exeName;

    const char* home = getenv("USERPROFILE");
    if (!home) home = ".";
    snprintf(path, sizeof(path), "%s\\Documents\\Logs\\%s.log", home, exeBase);
#else
    char exeName[1024];
    uint32_t size = sizeof(exeName);
    if (_NSGetExecutablePath(exeName, &size) != 0) {
        strcpy(exeName, "unknown");
    }
    const char* exeBase = basename(exeName);

    const char* home = getenv("HOME");
    if (!home) home = ".";
    snprintf(path, sizeof(path), "%s/Documents/Logs/%s.log", home, exeBase);
#endif
    return path;
}
#pragma endregion