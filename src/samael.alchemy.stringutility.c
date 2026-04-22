// -------------------------------------------------------------------------------------------
// samael.alchemy.stringutility.c — In the absence of native dynamic string support in C, this
// module serves as a foundation for safe and flexible string manipulation. It currently offers
// a utility to append formatted content to existing strings, expanding memory on demand to
// balance readability, maintainability, and resilience.
// -------------------------------------------------------------------------------------------
// Author:      Patrik Eigenmann
// eMail:       p.eigenmann72@gmail.com
// GitHub:      www.github.com/PatrikEigenmann72/helloc
// -------------------------------------------------------------------------------------------
// Change Log:
// Sun 2025-12-03 File created.                                                 Version: 00.01
// Sat 2025-12-06 Implementing debug_err & debug_warn messagages for clarity.   Version: 00.02
// Sun 2025-12-14 Removed debug_info calls to reduce verbosity.                 Version: 00.03
// Sun 2025-12-14 Added public function is_numeric implementation.              Version: 00.04
// Sun 2025-12-14 Added forward declarations for private helpers of is_numeric. Version: 00.05
// Sun 2025-12-14 Implemented private helper is_int for is_numeric.             Version: 00.06
// Sun 2025-12-14 Implemented private helper is_float for is_numeric.           Version: 00.07
// Sun 2025-12-14 Implemented private helper is_double for is_numeric.          Version: 00.08
// Sun 2025-12-14 Implemented private helper is_hex for is_numeric.             Version: 00.09
// Sun 2025-12-14 Implemented private helper is_calc for arithmetic detection.  Version: 00.10
// Sun 2025-12-14 Implemented private helper eval_calc for expression eval.     Version: 00.11
// Sun 2025-12-14 Implemented private helper parse_expr for + and - ops.        Version: 00.12
// Sun 2025-12-14 Implemented private helper parse_term for * and / ops.        Version: 00.13
// Sun 2025-12-14 Implemented private helper parse_factor for ^ and ().         Version: 00.14
// Sun 2025-12-14 Implemented private helper parse_number with % as percent.    Version: 00.15
// Sun 2025-12-14 Updated is_numeric to classify arithmetic expressions.        Version: 00.16
// Sun 2025-12-14 Publicized eval_calc, returning arithmetic results as string. Version: 00.17
// Sun 2025-12-14 Renamed eval_calc to calc_expression for clearer semantics.   Version: 00.18
// Mon 2025-12-29 Bugfix: Corrected recursion in pars_expr and parse_term.      Version: 00.19
// Mon 2025-12-29 Added to_lower and to_upper functions for string conversion.  Version: 00.20
// Sat 2026-01-04 Added str_trim function to remove whitespace.                 Version: 00.21
// Sat 2026-01-04 Fixed a memory issue in str_trim by copy strIn.               Version: 00.22
// -------------------------------------------------------------------------------------------
// ToDo List:
// These are possible string helpers we may want to add into stringutility over time. The idea
// is to cover common operations that ISO C doesn’t provide directly, but that developers reach
// for all the time. Each helper is small, focused, and designed to make everyday string
// handling safer and easier.
//
// - trim(char *str)                                                                    done
//      Remove leading and trailing whitespace from a string in place.
//
// - to_lower(char *str)                                                                done
// - to_upper(char *str)                                                                done
//      Convert all characters in a string to lowercase or uppercase.
//
// - starts_with(const char *str, const char *prefix)
// - ends_with(const char *str, const char *suffix)
//      Check whether a string begins or ends with a given substring.
//
// - substr(const char *str, size_t start, size_t length)
//      Return a newly allocated substring from the given range.
//
// - str_replace(const char *str, const char *old, const char *new)
//      Replace all occurrences of one substring with another.
//
// - split(const char *str, const char *delimiter, size_t *count)
//      Break a string into tokens based on a delimiter, returning an array of strings.
//
// - join(const char **parts, size_t count, const char *delimiter)
//      Concatenate an array of strings into one, separated by a delimiter.
//
// - str_dup(const char *str)
//      Duplicate a string safely (ISO C doesn’t guarantee strdup).
// -------------------------------------------------------------------------------------------

// Standard Library Includes
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Samael Library Includes
#include "samael.alchemy.stringutility.h"
#include "samael.huginandmunin.debug.h"

// -------------------------------------------------------------------------------------------
// Private helpers for is_numeric are forward declared here!
// -------------------------------------------------------------------------------------------
static int is_int(const char *strIn);        // Check for Integer
static int is_float(const char *strIn);      // Check for Float
static int is_double(const char *strIn);     // Check for Double (scientific notation)
static int is_hex(const char *strIn);        // Check for Hexadecimal
static int is_calc(const char *strIn);          // Detect if string is an arithmetic expression

// -------------------------------------------------------------------------------------------
// Private helpers for arithmetic calculation eval_calc are forward declared here!
// -------------------------------------------------------------------------------------------
static double parse_expr(const char **expr);    // Handle + and - operations
static double parse_term(const char **expr);    // Handle * and / operations
static double parse_factor(const char **expr);  // Handle ^ (power) and parentheses
static double parse_number(const char **expr);  // Extract numeric values, including percentage (%)
static void skip_spaces(const char **expr);     // Skip whitespace characters

#pragma region Function: append_format
// -------------------------------------------------------------------------------------------
// This functions is appending formatted content within this method, we ensure a seamless and
// efficient way to dynamically build strings. This method not only enhances the maintainability
// and readability of your codebase, but also guarantees that formatted content is appended
// consistently and effectively.
// 
// Adopting the append_format method will streamline your string manipulation tasks, fostering
// better organization and flexibility, and ultimately contributing to a more polished and
// efficient product.
// 
// @param char **dest - The destination string to which formatted content will be appended.
// @param const char *format - The format string.
// @param ... - Additional arguments to format and append to the destination string.
// --------------------------------------------------------------------------------------------------------
void append_format(char **dest, const char *format, ...) {
    // We’re about to handle a variable number of arguments (like printf does),
    // so we set up a `va_list` to walk through them safely.
    va_list args;
    va_start(args, format);
    
    // To figure out how much memory we’ll need for the formatted string,
    // we make a copy of the argument list. This way, we can “dry run” the
    // formatting without consuming the original arguments.
    va_list args_copy;
    va_copy(args_copy, args);
    
    // Ask `vsnprintf` how many characters the formatted string would take.
    // Passing NULL as the buffer means “don’t actually write, just tell me the size.”
    // We add 1 to account for the null terminator at the end.
    int size = vsnprintf(NULL, 0, format, args_copy) + 1;
    va_end(args_copy);

    // Now that we know the exact size, we allocate a temporary buffer to hold
    // the formatted string. If allocation fails, we log the error and bail out.
    char *temp = malloc(size);
    if (temp == NULL) {
        // If the memory allocation went wrong, we document the error here.
        // This message only shows up if the application was compiled with -DDEBUG,
        // so developers get a clear signal about what failed.
        debug_err("Memory allocation failed in append_format");
        va_end(args);
        return;
    }
    
    // With memory ready, we actually perform the formatting and write the
    // result into our temporary buffer.
    vsprintf(temp, format, args);
    
    // We’re done with the variable arguments, so we clean them up.
    va_end(args);

    // Time to merge the new formatted string into the destination.
    // If `*dest` hasn’t been allocated yet, this is our first write.
    if (*dest == NULL) {
        // First-time allocation: just copy the new string into place.
        *dest = malloc(size);
        if (*dest == NULL) {
            // Again, if allocation fails, we log the error so developers
            // know exactly why the function bailed out.
            debug_err("Memory allocation failed in append_format");
            free(temp);
            return;
        }

        strcpy(*dest, temp);
    } else {
        // If `*dest` already has content, we need to grow it to fit the new data.
        // `realloc` handles this by extending the existing buffer.
        *dest = realloc(*dest, strlen(*dest) + size);
        if (*dest == NULL) {
            // If reallocation fails, we capture the error here.
            // With -DDEBUG enabled, this message helps track down memory issues.
            debug_err("Memory reallocation failed in append_format");
            free(temp);
            return;
        }
        // Once resized, we tack the new string onto the end of the old one.
        strcat(*dest, temp);
    }

    // Finally, we free the temporary buffer since its contents have been
    // safely copied into `*dest`. No leaks left behind.
    free(temp);
}
#pragma endregion Function: append_format

#pragma region Function: is_numeric
// -------------------------------------------------------------------------------------------
// is_numeric - This function validates whether a given string represents a numerical value.
// By consolidating the detection of integers, floating-point numbers, doubles (scientific
// notation), and hexadecimal values into a single method, we ensure a streamlined and reliable
// approach to input validation. This method not only improves the clarity and maintainability
// of your codebase, but also guarantees that numerical strings are classified consistently
// and effectively.
//
// Adopting the is_numeric method will simplify your validation tasks, foster better organization,
// and ultimately contribute to a more robust and predictable product.
//
// @param char *whatInOut       - A flag that will be set to 'i' for integer, 'f' for float,
//                                'd' for double, or 'h' for hex when the input is valid.
// @param const char *numberIn  - The string to validate as a numerical value.
// @return int                  - Returns 1 if the string is a valid number, 0 otherwise.
// -------------------------------------------------------------------------------------------
int is_numeric(char *whatInOut, const char *numberIn) {
    // First, check if the input pointer itself is NULL.
    // If no string was provided, we immediately return 0.
    if (!numberIn) return 0;

    // Next, check if the string represents a hexadecimal number.
    // This covers formats like "0x1A3F".
    if (is_hex(numberIn)) {
        if (whatInOut) *whatInOut = 'h';
        return 1;
    }

    // Check if the string is a plain integer (e.g., "42", "-7").
    if (is_int(numberIn)) {
        if (whatInOut) *whatInOut = 'i';
        return 1;
    }

    // Check if the string is a floating-point number with a decimal point.
    if (is_float(numberIn)) {
        if (whatInOut) *whatInOut = 'f';
        return 1;
    }

    // Check if the string is a double in scientific notation
    // (e.g., "1.23e4", "-2.5E-3").
    if (is_double(numberIn)) {
        if (whatInOut) *whatInOut = 'd';
        return 1;
    }

    // Very cool feature: detect if the string looks like an arithmetic expression.
    // This allows for dynamic calculations like "3+5*2" or "10/(4-1)".
    // Order of operations and parenthesis are respected.
    if (is_calc(numberIn)) {
        if (whatInOut) *whatInOut = 'a';
        return 1;
    }

    // If none of the checks passed, the string is not numeric.
    return 0;
}

// -------------------------------------------------------------------------------------------
// is_int - This private helper validates whether a given string represent a positive or 
// negative integer.
// 
// @param const char *strIn - The string to validate as an integer.
// @return int              - Returns 1 if the string is valid, else 0.
// -------------------------------------------------------------------------------------------
static int is_int(const char *strIn) {
    // Allow optional leading '+' or '-' sign
    if (*strIn == '+' || *strIn == '-') {
        strIn++;
    }

    // Reject empty string after sign
    if (!*strIn) return 0;

    // Walk through each character, ensuring all are digits
    while (*strIn) {
        if (!isdigit((unsigned char)*strIn)) {
            return 0;
        }
        strIn++;
    }

    return 1;
}

// -------------------------------------------------------------------------------------------
// is_float - This private helper validates whether a given string represents
// a floating-point number with a decimal point.
// 
// @param const char *strIn - The string to validate as a float.
// @return int              - Returns 1 if the string is valid, else 0.
// -------------------------------------------------------------------------------------------
static int is_float(const char *strIn) {
    int seen_dot = 0;

    // Allow optional leading '+' or '-' sign
    if (*strIn == '+' || *strIn == '-') {
        strIn++;
    }

    // Reject empty string after sign
    if (!*strIn) return 0;

    while (*strIn) {
        if (*strIn == '.') {
            if (seen_dot) return 0; // multiple dots not allowed
            seen_dot = 1;
        } else if (!isdigit((unsigned char)*strIn)) {
            return 0;
        }
        strIn++;
    }

    return seen_dot; // must contain a dot to be considered float
}

// -------------------------------------------------------------------------------------------
// is_double - This private helper validates whether a given string represents
// a double in scientific notation (e.g., "1.23e4").
// 
// @param const char *strIn - The string to validate as a double.
// @return int              - Returns 1 if the string is valid, else 0.
// -------------------------------------------------------------------------------------------
static int is_double(const char *strIn) {
    int seen_dot = 0, seen_exp = 0;

    // Allow optional leading '+' or '-' sign
    if (*strIn == '+' || *strIn == '-') {
        strIn++;
    }

    // Reject empty string after sign
    if (!*strIn) return 0;

    while (*strIn) {
        if (*strIn == '.') {
            if (seen_dot || seen_exp) return 0;
            seen_dot = 1;
        } else if (*strIn == 'e' || *strIn == 'E') {
            if (seen_exp) return 0;
            seen_exp = 1;
            strIn++;
            if (*strIn == '+' || *strIn == '-') strIn++;
            if (!isdigit((unsigned char)*strIn)) return 0;
        } else if (!isdigit((unsigned char)*strIn)) {
            return 0;
        }
        strIn++;
    }

    return seen_dot || seen_exp;
}

// -------------------------------------------------------------------------------------------
// is_hex - This private helper validates whether a given string represents
// a hexadecimal number (e.g., "0x1A3F").
// 
// @param const char *strIn - The string to validate as hex.
// @return int              - Returns 1 if the string is valid, else 0.
// -------------------------------------------------------------------------------------------
static int is_hex(const char *strIn) {
    // Must start with "0x" or "0X"
    if (strlen(strIn) > 2 && strIn[0] == '0' && (strIn[1] == 'x' || strIn[1] == 'X')) {
        strIn += 2;

        // Reject empty string after prefix
        if (!*strIn) return 0;

        while (*strIn) {
            if (!isxdigit((unsigned char)*strIn)) {
                return 0;
            }
            strIn++;
        }
        return 1;
    }
    return 0;
}

// -------------------------------------------------------------------------------------------
// is_calc - Detect if a string looks like an arithmetic expression.
// 
// @param const char *strIn - The string to validate.
// @return int              - Returns 1 if expression detected, else 0.
// -------------------------------------------------------------------------------------------
static int is_calc(const char *strIn) {
    while (*strIn) {
        if (*strIn == '+' || *strIn == '-' || *strIn == '*' ||
            *strIn == '/' || *strIn == '^' || *strIn == '%' ||
            *strIn == '(' || *strIn == ')') {
            return 1;
        }
        strIn++;
    }
    return 0;
}
#pragma endregion Function: is_numeric

#pragma region Function: calc_expression
// -------------------------------------------------------------------------------------------
// calc_expression - This function is evaluating whether a given string represents a valid
// arithmetic expression and returns the computed result as a string. By consolidating the
// parsing and evaluation of mathematical operations into a single method, we ensure a
// streamlined and reliable approach to expression handling. This method not only improves the
// clarity and maintainability of your codebase, but also guarantees that arithmetic strings
// are processed consistently and effectively.
//
// The decision to return the result as a string is deliberate: since this module belongs to
// stringutility, all public functions should consistently operate on strings. This approach
// empowers developers to decide how they want to consume the result — whether as an integer,
// float, or double — without imposing a specific numeric type at the API level. In doing so,
// we preserve flexibility, avoid type mismatches, and maintain a clean separation of concerns.
//
// Adopting the eval_calc method will simplify arithmetic evaluation tasks, foster better
// organization, and ultimately contribute to a more predictable and developer‑friendly product.
//
// @param const char *expressionIn - The arithmetic expression string to evaluate.
// @return char*                   - Result as a newly allocated string (caller frees).
// --------------------------------------------------------------------------------------------
char *calc_expression(const char *expressionIn) {
    skip_spaces(&expressionIn);
    double value = parse_expr(&expressionIn);
    skip_spaces(&expressionIn);

    if (*expressionIn != '\0') {
        // Handle error: leftover characters
        fprintf(stderr, "Unexpected input at: %s\n", expressionIn);
    }

    char bufferOut[64];
    snprintf(bufferOut, sizeof(bufferOut), "%.10g", value);
    return strdup(bufferOut);
}

static void skip_spaces(const char **expr) {
    while (**expr == ' ' || **expr == '\t') (*expr)++;
}

// -------------------------------------------------------------------------------------------
// parse_expr - Handle + and - operations.
// 
// @param const char **expr - Pointer to expression string.
// @return double           - Result of addition/subtraction.
// -------------------------------------------------------------------------------------------
static double parse_expr(const char **expr) {
    skip_spaces(expr);
    double value = parse_term(expr);

    while (1) {
        skip_spaces(expr);
        if (**expr == '+' || **expr == '-') {
            char op = *(*expr)++;
            double rhs = parse_term(expr);
            if (op == '+') value += rhs;
            else value -= rhs;
        } else {
            break;
        }
    }
    return value;
}

// -------------------------------------------------------------------------------------------
// parse_term - Handle * and / operations.
// 
// @param const char **expr - Pointer to expression string.
// @return double           - Result of multiplication/division.
// -------------------------------------------------------------------------------------------
static double parse_term(const char **expr) {
    skip_spaces(expr);
    double value = parse_factor(expr);

    while (1) {
        skip_spaces(expr);
        if (**expr == '*' || **expr == '/') {
            char op = *(*expr)++;
            double rhs = parse_factor(expr);
            if (op == '*') value *= rhs;
            else value /= rhs;
        } else {
            break;
        }
    }
    return value;
}

// -------------------------------------------------------------------------------------------
// parse_factor - Handle ^ (power) and parentheses.
// 
// @param const char **expr - Pointer to expression string.
// @return double           - Result of factor evaluation.
// -------------------------------------------------------------------------------------------
static double parse_factor(const char **expr) {
    skip_spaces(expr);
    double value;

    if (**expr == '(') {
        (*expr)++;
        value = parse_expr(expr);
        skip_spaces(expr);
        if (**expr == ')') (*expr)++;
    } else {
        value = parse_number(expr);
    }

    skip_spaces(expr);
    while (**expr == '^') {
        (*expr)++;
        double rhs = parse_factor(expr);
        value = pow(value, rhs);
        skip_spaces(expr);
    }

    return value;
}

// -------------------------------------------------------------------------------------------
// parse_number - Extract numeric values, including percentage (%).
// 
// @param const char **expr - Pointer to expression string.
// @return double           - Parsed number (percentage handled).
// -------------------------------------------------------------------------------------------
static double parse_number(const char **expr) {
    char *end;
    double value = strtod(*expr, &end);
    *expr = end;

    // Handle percentage as postfix operator
    if (**expr == '%') {
        (*expr)++;
        value /= 100.0;
    }

    return value;
}
#pragma endregion Function: calc_expression

#pragma region Function: String Conversions (to_lower, to_upper)
// -------------------------------------------------------------------------------------------
// to_lower - Transform every character in a string to its lowercase form.
//
// This utility walks through the provided string and converts each alphabetic character
// to its lowercase equivalent. Non-alphabetic characters remain untouched, ensuring that
// punctuation, digits, and symbols are preserved exactly as they are. The conversion is
// performed in place, so the original string is modified directly.
//
// @param char *strInOut - Pointer to the string that should be converted. Must be null-terminated.
//                         Acts as InOut: the input string is read and modified in place.
// @return char*         - Returns the same pointer to the input string, allowing for convenient
//                         chaining or direct use after conversion.
// -------------------------------------------------------------------------------------------
char *to_lower(char *strInOut) {
    if (!strInOut) return NULL;
    for (char *p = strInOut; *p; ++p) {
        *p = (char)tolower((unsigned char)*p);
    }
    return strInOut;
}

// -------------------------------------------------------------------------------------------
// to_upper - Transform every character in a string to its uppercase form.
//
// This function iterates through the given string and converts each alphabetic character
// to uppercase. Characters that are not letters (such as digits, spaces, or punctuation)
// remain unchanged. The transformation is applied directly to the input string, so no
// additional memory allocation is required.
//
// @param char *strInOut - Pointer to the string that should be converted. Must be null-terminated.
//                         Acts as InOut: the input string is read and modified in place.
// @return char*         - Returns the same pointer to the input string, enabling chaining or
//                         immediate use after conversion.
// -------------------------------------------------------------------------------------------
char *to_upper(char *strInOut) {
    if (!strInOut) return NULL;
    for (char *p = strInOut; *p; ++p) {
        *p = (char)toupper((unsigned char)*p);
    }
    return strInOut;
}
#pragma endregion Function: String Conversions (to_lower, to_upper)

#pragma region Function: str_trim
// -------------------------------------------------------------------------------------------
// str_trim - Remove leading and trailing whitespace from a string.
//
// This function removes all leading and trailing whitespace characters (spaces, tabs,
// newlines, etc.) from the input string. It modifies the string in place and returns a
// pointer to the first non-whitespace character in the modified string.
//
// @param char *strInOut - Pointer to the string that should be trimmed. Must be null-terminated.
//                         Acts as InOut: the input string is read and modified in place.
// @return char*         - Returns a pointer to the first non-whitespace character in the
//                         modified string, or NULL if the entire string was whitespace.
// -------------------------------------------------------------------------------------------
char *str_trim(const char *strIn) {
    if (!strIn) return NULL;

    // Skip leading whitespace
    const char *start = strIn;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }

    if (*start == '\0') {
        // Entire string was whitespace
        return NULL;
    }

    // Find end of non-whitespace
    const char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }

    // Length of trimmed substring
    size_t len = end - start + 1;

    // Allocate new buffer (+1 for null terminator)
    char *strOut = malloc(len + 1);
    if (!strOut) return NULL;

    memcpy(strOut, start, len);
    strOut[len] = '\0';

    return strOut;
}
#pragma endregion Function: str_trim