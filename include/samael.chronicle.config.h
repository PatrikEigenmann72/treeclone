// -------------------------------------------------------------------------------------------
// samael.chronicle.config.h - Header file for Samael's standardized configuration system.
// This library provides a dynamic in-memory configuration system. It manages an internal
// array of key=value entries, allowing applications to add configuration values at runtime
// and retrieve them through typed getter functions. The application does not need to know
// how the array is stored, how many entries exist, or what the internal type is called.
// 
// The configuration system serializes key=value pairs into memory and deserializes them
// upon request. If a key does not exist or a type conversion fails, safe defaults are
// returned to ensure resilience. This design keeps applications independent of internal
// structures while providing a unified interface for configuration management.
// -------------------------------------------------------------------------------------------
// Author:      Patrik Eigenmann
// eMail:       p.eigenman72@gmail.com
// GitHub:      https://github.com/PatrikEigenmann72/helloc
// -------------------------------------------------------------------------------------------
// Change Log:
// Tue 2025-11-25 File created.                                                 Version: 00.01
// Wed 2025-11-26 Structural redesign: typedef ConfigEntry implemented.         Version: 00.02
// Thu 2025-11-27 Structural redesign: added add_config() and removed extern.   Version: 00.03
// Thu 2025-11-27 Changed documentation in to more sophisticated descriptions.  Version: 00.04
// Thu 2025-11-27 Declared extern config_init() for application-specific setup. Version: 00.05
// -------------------------------------------------------------------------------------------

#ifndef SAMAEL_CHRONICLE_CONFIG_H
#define SAMAEL_CHRONICLE_CONFIG_H

#include <stdbool.h>

// -------------------------------------------------------------------------------------------
// ConfigEntry - Definition of a configuration record.
// Each entry represents a single key=value pair stored in memory. The `key` is a string
// identifier (e.g., "App.Name"), and the `value` is its associated string data. The config
// module serializes these pairs into an internal array and deserializes them when accessed
// through typed getters. Applications never manipulate this struct directly; they only
// interact with the configuration system via add_config() and retrieval functions.
// -------------------------------------------------------------------------------------------
typedef struct {
    const char* key;
    const char* value;
} ConfigEntry;

// -------------------------------------------------------------------------------------------
// config_init - Application-specific configuration initializer.
// This function must be implemented by the application using Samael. It is responsible for
// populating the Chronicle with all required key=value pairs via config_add(). The framework
// itself does not provide a default implementation.
//
// Example:
// void config_init(void) {
//     config_add("App.Name", "helloc");
//     config_add("App.Version", "00.03");
//     config_add("App.Label", "Hello C World!");
//     // … add more entries as needed
// }
//
// @note This function is declared extern here, but must be defined in the application.
// -------------------------------------------------------------------------------------------
extern void config_init(void);

// -------------------------------------------------------------------------------------------
// config_add - Adds a new configuration entry into the internal Chronicle.
// This function dynamically appends a key=value pair to the configuration memory. The key
// acts as a unique identifier, while the value is stored as a string. Applications can later
// retrieve the value using one of the typed getter functions. If the same key is added more
// than once, the most recent value will overwrite the previous one.
//
// @param key   The string identifier for the configuration entry (e.g., "App.Name").
// @param value The string value associated with the key (e.g., "HelloWorld").
// -------------------------------------------------------------------------------------------
void config_add(const char* key, const char* value);

// -------------------------------------------------------------------------------------------
// config_get_string - Retrieves a configuration value as a string.
// This function looks up the given key in the internal Chronicle and returns its associated
// string value. If the key does not exist, an empty string ("") is returned instead of NULL,
// ensuring that applications can safely use the result without additional checks.
//
// This function represents the most direct form of deserialization: the stored value is
// returned exactly as it was added via config_add(), without type conversion. It is the
// foundation upon which other typed getters (config_get_int, config_get_bool, etc.) build.
//
// @param key     The string identifier for the configuration entry (e.g., "App.Name").
// @return        The string value associated with the key, or "" if the key does not exist.
// -------------------------------------------------------------------------------------------
const char* config_get_string(const char* key);

// -------------------------------------------------------------------------------------------
// config_get_int - Retrieves a configuration value as an integer.
// This function looks up the given key in the internal Chronicle and attempts to convert
// its associated string value into an integer using standard C library functions. If the
// key does not exist or the value cannot be parsed as a valid integer, the function returns
// 0 as a safe default.
//
// This function represents a typed deserialization: the stored string is interpreted as
// numeric data. It allows applications to work with configuration values directly as
// integers without needing to perform manual conversions.
//
// @param key     The string identifier for the configuration entry (e.g., "App.Port").
// @return        The integer value associated with the key, or 0 if the key does not exist
//                or the conversion fails.
// -------------------------------------------------------------------------------------------
int config_get_int(const char* key);

// -------------------------------------------------------------------------------------------
// config_get_bool - Retrieves a configuration value as a boolean.
// This function looks up the given key in the internal Chronicle and attempts to interpret
// its associated string value as a boolean. Recognized values are strictly English forms:
//
//   Logical truth:  "true", "yes", "on", "+", or any integer > 1
//   Logical false:  "false", "no", "off", "0"
//
// Case-insensitivity is applied to ensure flexible matching. Other language-specific forms
// (e.g., "ja", "si", "oui") are not recognized and will default to false.
//
// If the key does not exist or the value cannot be interpreted as a valid boolean, the
// function returns false as a safe default.
//
// @param key     The string identifier for the configuration entry (e.g., "Feature.Enabled").
// @return        true if the value represents a logical truth, false if the value represents
//                a logical falsehood, or if the key does not exist or parsing fails.
// -------------------------------------------------------------------------------------------
bool config_get_bool(const char* key);

// -------------------------------------------------------------------------------------------
// config_get_float - Retrieves a configuration value as a floating-point number (float).
// This function looks up the given key in the internal Chronicle and attempts to convert
// its associated string value into a float using standard C library functions. If the key
// does not exist or the value cannot be parsed as a valid floating-point number, the
// function returns 0.0f as a safe default.
//
// This function represents a typed deserialization: the stored string is interpreted as
// numeric data in single-precision format. It allows applications to work directly with
// configuration values as floats without needing to perform manual conversions.
//
// @param key     The string identifier for the configuration entry (e.g., "App.ScaleFactor").
// @return        The float value associated with the key, or 0.0f if the key does not exist
//                or the conversion fails.
// -------------------------------------------------------------------------------------------
float config_get_float(const char* key);

// -------------------------------------------------------------------------------------------
// config_get_double - Retrieves a configuration value as a double-precision floating-point number.
// This function looks up the given key in the internal Chronicle and attempts to convert
// its associated string value into a double using standard C library functions. If the key
// does not exist or the value cannot be parsed as a valid double, the function returns 0.0
// as a safe default.
//
// This function represents a typed deserialization: the stored string is interpreted as
// numeric data in double-precision format. It allows applications to work directly with
// configuration values as doubles without needing to perform manual conversions.
//
// @param key     The string identifier for the configuration entry (e.g., "App.TimeoutSeconds").
// @return        The double value associated with the key, or 0.0 if the key does not exist
//                or the conversion fails.
// -------------------------------------------------------------------------------------------
double config_get_double(const char* key);

// -------------------------------------------------------------------------------------------
// config_get_char - Retrieves a configuration value as a single character.
// This function looks up the given key in the internal Chronicle and attempts to return
// the first character of its associated string value. If the key does not exist or the
// value is an empty string, the function returns the null character ('\0') as a safe default.
//
// This function represents a typed deserialization: the stored string is reduced to its
// leading character. It is useful for scenarios where configuration values are expected
// to be symbolic flags, single-letter codes, or other compact identifiers.
//
// @param key     The string identifier for the configuration entry (e.g., "App.ModeFlag").
// @return        The first character of the string value associated with the key, or '\0'
//                if the key does not exist or the value is empty.
// -------------------------------------------------------------------------------------------
char config_get_char(const char* key);

#endif // SAMAEL_CHRONICLE_CONFIG_H