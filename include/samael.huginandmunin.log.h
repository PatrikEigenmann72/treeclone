// -------------------------------------------------------------------------------------------
// samael.huginandmunin.log.h - This is a lightweight logging system for C applications. It
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
// -------------------------------------------------------------------------------------------
#ifndef SAMAEL_HUGINANDMUNIN_LOG_H
#define SAMAEL_HUGINANDMUNIN_LOG_H

// -------------------------------------------------------------------------------------------
// LOG_NONE - Bitmask constant representing the absence of logging. When this value is used
// as the active log_mask, no messages of any severity are recorded. This is typically employed
// to disable logging entirely, either for performance reasons or in production builds where
// diagnostic output is not desired.
// -------------------------------------------------------------------------------------------
#define LOG_NONE    0x00

// -------------------------------------------------------------------------------------------
// LOG_VERBOSE - Bitmask constant enabling verbose diagnostic output. This level is intended
// for highly detailed, low-level messages that trace program flow, variable states, and internal
// operations. It is primarily useful during development and debugging, and is usually disabled
// in production to avoid excessive log volume.
// -------------------------------------------------------------------------------------------
#define LOG_VERBOSE 0x01

// -------------------------------------------------------------------------------------------
// LOG_INFO - Bitmask constant enabling informational messages. This level records general status
// updates, configuration details, and non-critical progress reports. It provides visibility
// into normal application behavior without the noise of verbose debugging.
// -------------------------------------------------------------------------------------------
#define LOG_INFO    0x02

// -------------------------------------------------------------------------------------------
// LOG_WARN - Bitmask constant enabling warning messages. This level highlights unusual or
// potentially problematic conditions that do not immediately halt execution. Warnings
// serve as early indicators of issues that may require attention but are not fatal.
// -------------------------------------------------------------------------------------------
#define LOG_WARN    0x04


#define LOG_ERR     0x08


#define LOG_ALL     (LOG_VERBOSE | LOG_INFO | LOG_WARN | LOG_ERR)

// Initialize logging system.
// levelMaskIn: In — bitmask specifying which levels to log.
// Clears existing logfile if present.
void log_init(int levelMaskIn);

// Internal helper declaration for macro expansion.
// NOTE: Not for direct use — contributors should only use the macros.
void log_write_internal(int levelIn, const char* levelStr,
                        const char* fileIn, const char* funcIn, int lineIn,
                        const char* fmt, ...);

// Macros stay the same, but now they can take either a plain string or a format + args.
#define log_verbose(...) log_write_internal(LOG_VERBOSE, "VRB", __FILE__, __func__, __LINE__, __VA_ARGS__)
#define log_info(...)    log_write_internal(LOG_INFO,    "INF", __FILE__, __func__, __LINE__, __VA_ARGS__)
#define log_warn(...)    log_write_internal(LOG_WARN,    "WRN", __FILE__, __func__, __LINE__, __VA_ARGS__)
#define log_err(...)     log_write_internal(LOG_ERR,     "ERR", __FILE__, __func__, __LINE__, __VA_ARGS__)

#endif // SAMAEL_HUGINANDMUNIN_LOG_H