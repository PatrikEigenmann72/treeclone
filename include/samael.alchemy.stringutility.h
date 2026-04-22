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
// Wed 2025-12-03 File created.                                                 Version: 00.01
// Sun 2025-12-14 Add function is_numeric into the library.                     Version: 00.02
// Sun 2025-12-14 Add function calc_expression into the library.                Version: 00.03
// Mon 2025-12-29 Add functions to_lower and to_upper for string conversion.    Version: 00.04
// Sat 2026-01-04 Add function str_trim to remove whitespace.                   Version: 00.05
// -------------------------------------------------------------------------------------------
// ToDo List:
// These are possible string helpers we may want to add into stringutility over time. The idea
// is to cover common operations that ISO C doesn’t provide directly, but that developers reach
// for all the time. Each helper is small, focused, and designed to make everyday string
// handling safer and easier.
//
// - trim(char *str)
//      Remove leading and trailing whitespace from a string in place.
//
// - to_lower(char *str)
// - to_upper(char *str)
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
#ifndef SAMAEL_ALCHEMY_STRINGUTILITY_H
#define SAMAEL_ALCHEMY_STRINGUTILITY_H

// -------------------------------------------------------------------------------------------
// appen_format - This functions is appending formatted content within this method, we ensure
// a seamless and efficient way to dynamically build strings. This method not only enhances
// the maintainability and readability of your codebase, but also guarantees that formatted
// content is appended consistently and effectively.
// 
// Adopting the append_format method will streamline your string manipulation tasks, fostering
// better organization and flexibility, and ultimately contributing to a more polished and
// efficient product.
// 
// @param char **dest - The destination string to which formatted content will be appended.
// @param const char *format - The format string.
// @param ... - Additional arguments to format and append to the destination string.
// -------------------------------------------------------------------------------------------
void append_format(char **dest, const char *format, ...);

// -------------------------------------------------------------------------------------------
// is_numeric - This function is validating whether a given string represents a numerical value.
// By consolidating the detection of integers, floating-point numbers, and hexadecimal values
// into a single method, we ensure a streamlined and reliable approach to input validation. This
// method not only improves the clarity and maintainability of your codebase, but also guarantees
// that numerical strings are classified consistently and effectively.
//
// Adopting the is_numeric method will simplify your validation tasks, foster better organization,
// and ultimately contribute to a more robust and predictable product.
//
// @param char *whatInOut       - A flag that will be set to 'i' for integer, 'f' for float, or 'h'
//                                for hex when the input is valid.
// @param const char *numberIn  - The string to validate as a numerical value.
// @return int                  - Returns 1 if the string is a valid number, 0 otherwise.
// --------------------------------------------------------------------------------------------
int is_numeric(char *whatInOut, const char *numberIn);

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
char *calc_expression(const char *expressionIn);

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
char *to_lower(char *strInOut);

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
char *to_upper(char *strInOut);

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
char *str_trim(const char *strIn);

#endif // SAMAEL_ALCHEMY_STRINGUTILITY_H