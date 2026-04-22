// -------------------------------------------------------------------------------------------
// samael.chronicle.config.c - Function library for Samael's in-memory configuration system.
// This file implements the logic declared in samael.chronicle.config.h. It manages an internal
// array of key=value entries, allowing applications to add configuration values dynamically
// and retrieve them through typed getter functions. The application does not need to know
// how the array is stored, how many entries exist, or what type is used internally.
//
// The configuration system serializes key=value pairs into memory and deserializes them
// upon request. If a key does not exist or a type conversion fails, safe defaults are
// returned to ensure resilience. Errors are logged via the Hugin & Munin debug system to
// provide visibility without breaking execution flow.
// -------------------------------------------------------------------------------------------
// Author:      Patrik Eigenmann
// eMail:       p.eigenmann72@gmail.com
// GitHub:      https://github.com/PatrikEigenmann72/helloc
// -------------------------------------------------------------------------------------------
// Change Log:
// Tue 2025-11-25 File created.                                                 Version: 00.01
// Wed 2025-11-26 Structural redesign: typedef ConfigEntry implemented.         Version: 00.02
// Thu 2025-11-27 Structural redesign: added config_add() and removed extern.   Version: 00.03
// Thu 2025-11-27 Added sophisticated documentation for implementation.         Version: 00.04
// Thu 2025-11-27 Integrated debug_error logging for missing keys/conversions.  Version: 00.05
// -------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "samael.chronicle.config.h"
#include "samael.huginandmunin.debug.h"   // Debugging support

// -------------------------------------------------------------------------------------------
// Internal Chronicle State
// These variables represent the private storage of configuration entries. They are declared
// as static to restrict their visibility to this translation unit (samael.chronicle.config.c).
// Applications cannot access them directly; they interact only through the public API.
//
// settings        Dynamic array of ConfigEntry records (key=value pairs).
// settings_count  Number of entries currently stored in the Chronicle.
// -------------------------------------------------------------------------------------------
static ConfigEntry* settings = NULL;
static int settings_count = 0;

// -------------------------------------------------------------------------------------------
// config_add - Adds a new configuration entry into the internal Chronicle.
// This function dynamically appends a key=value pair to the configuration memory. The key
// acts as a unique identifier, while the value is stored as a string. If the same key is
// added more than once, the most recent value overwrites the previous one.
//
// @param key   The string identifier for the configuration entry (e.g., "App.Name").
// @param value The string value associated with the key (e.g., "HelloWorld").
// -------------------------------------------------------------------------------------------
void config_add(const char* key, const char* value) {
    ConfigEntry* new_settings = realloc(settings, (settings_count + 1) * sizeof(ConfigEntry));
    if (!new_settings) {
        debug_err("Memory allocation failed while adding key '%s'.", key);
        return;
    }
    settings = new_settings;
    settings[settings_count].key = key;
    settings[settings_count].value = value;
    settings_count++;
}

// -------------------------------------------------------------------------------------------
// get_setting - Internal lookup function for configuration entries.
// This helper searches the Chronicle’s internal array for a given key and returns its
// associated string value. If the key does not exist, an error is logged.
//
// @param key     The string identifier for the configuration entry.
// @return        The string value associated with the key, or NULL if the key does not exist.
// -------------------------------------------------------------------------------------------
static const char* get_setting(const char* key) {
    for (int i = 0; i < settings_count; i++) {
        if (strcmp(settings[i].key, key) == 0) {
            return settings[i].value;
        }
    }
    debug_err("Configuration key '%s' not found.", key);
    return NULL;
}

// -------------------------------------------------------------------------------------------
// config_get_string - Retrieves a configuration value as a string.
// Returns the stored string exactly as added via config_add(). If the key does not exist,
// logs an error and returns an empty string.
//
// @param key     The string identifier for the configuration entry.
// @return        The string value associated with the key, or "" if the key does not exist.
// -------------------------------------------------------------------------------------------
const char* config_get_string(const char* key) {
    const char* value = get_setting(key);
    if (!value) {
        debug_err("Conversion error: key '%s' has no valid string value.", key);
        return "";
    }
    return value;
}

// -------------------------------------------------------------------------------------------
// config_get_int - Retrieves a configuration value as an integer.
// Converts the stored string into an integer. If the key does not exist or conversion fails,
// logs an error and returns 0 as a safe default.
//
// @param key     The string identifier for the configuration entry.
// @return        The integer value associated with the key, or 0 if missing/invalid.
// -------------------------------------------------------------------------------------------
int config_get_int(const char* key) {
    const char* value = get_setting(key);
    if (!value) return 0;

    char* endptr;
    int result = (int)strtol(value, &endptr, 10);
    if (*endptr != '\0') {
        debug_err("Conversion error: key '%s' value '%s' is not a valid integer.", key, value);
        return 0;
    }
    return result;
}

// -------------------------------------------------------------------------------------------
// config_get_bool - Retrieves a configuration value as a boolean.
// Recognized values (case-insensitive):
//   true, yes, on, +, or any integer > 1   → logical truth
//   false, no, off, 0                      → logical falsehood
//
// If the key does not exist or parsing fails, logs an error and returns false.
//
// @param key     The string identifier for the configuration entry.
// @return        true if the value represents logical truth, false otherwise.
// -------------------------------------------------------------------------------------------
bool config_get_bool(const char* key) {
    const char* value = get_setting(key);
    if (!value) return false;

    char normalized[32];
    snprintf(normalized, sizeof(normalized), "%s", value);
    for (char* p = normalized; *p; ++p) *p = tolower(*p);

    if (strcmp(normalized, "true") == 0 || strcmp(normalized, "yes") == 0 ||
        strcmp(normalized, "on") == 0 || strcmp(normalized, "+") == 0) {
        return true;
    }
    if (strcmp(normalized, "false") == 0 || strcmp(normalized, "no") == 0 ||
        strcmp(normalized, "off") == 0 || strcmp(normalized, "0") == 0) {
        return false;
    }

    char* endptr;
    long num = strtol(normalized, &endptr, 10);
    if (*endptr == '\0' && num > 1) {
        return true;
    }

    debug_err("Conversion error: key '%s' value '%s' is not a valid boolean.", key, value);
    return false;
}

// -------------------------------------------------------------------------------------------
// config_get_double - Retrieves a configuration value as a double.
// Converts the stored string into a double-precision floating-point number. If the key
// does not exist or conversion fails, logs an error and returns 0.0.
//
// @param key     The string identifier for the configuration entry.
// @return        The double value associated with the key, or 0.0 if missing/invalid.
// -------------------------------------------------------------------------------------------
double config_get_double(const char* key) {
    const char* value = get_setting(key);
    if (!value) return 0.0;

    char* endptr;
    double result = strtod(value, &endptr);
    if (*endptr != '\0') {
        debug_err("Conversion error: key '%s' value '%s' is not a valid double.", key, value);
        return 0.0;
    }
    return result;
}

// -------------------------------------------------------------------------------------------
// config_get_float - Retrieves a configuration value as a float.
// Converts the stored string into a single-precision floating-point number. If the key
// does not exist or conversion fails, logs an error and returns 0.0f.
//
// @param key     The string identifier for the configuration entry.
// @return        The float value associated with the key, or 0.0f if missing/invalid.
// -------------------------------------------------------------------------------------------
float config_get_float(const char* key) {
    const char* value = get_setting(key);
    if (!value) return 0.0f;

    char* endptr;
    float result = strtof(value, &endptr);
    if (*endptr != '\0') {
        debug_err("Conversion error: key '%s' value '%s' is not a valid float.", key, value);
        return 0.0f;
    }
    return result;
}

// -------------------------------------------------------------------------------------------
// config_get_char - Retrieves a configuration value as a single character.
// Returns the first character of the stored string. If the key does not exist or the
// value is empty, logs an error and returns the null character ('\0').
//
// @param key     The string identifier for the configuration entry.
// @return        The first character of the string value, or '\0' if missing/empty.
// -------------------------------------------------------------------------------------------
char config_get_char(const char* key) {
    const char* value = get_setting(key);
    if (!value || strlen(value) == 0) {
        debug_err("Conversion error: key '%s' has no valid character value.", key);
        return '\0';
    }
    return value[0];
}