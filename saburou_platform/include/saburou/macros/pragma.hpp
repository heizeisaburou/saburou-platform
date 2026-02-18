/**
 * @file pragma.hpp
 * @brief Portable preprocessor utilities for stringification and pragma messages.
 *
 * This header provides a set of macros to handle string conversion and compiler 
 * pragmas in a uniform way across different compilers, enabling custom 
 * diagnostic messages during the compilation process.
 */

#pragma once

/**
 * @brief Implementation detail for stringification.
 */
#define SABUROU_STRINGIFY_IMPL(x) #x

/**
 * @brief Converts a macro argument into a string literal.
 * @param x The token or macro to be stringified.
 */
#define SABUROU_STRINGIFY(x) SABUROU_STRINGIFY_IMPL(x)

/**
 * @brief Implementation detail for portable _Pragma.
 */
#define SABUROU_PRAGMA_IMPL(x) _Pragma(#x)

/**
 * @brief Wrapper for the _Pragma operator to allow macro expansion within pragmas.
 * @param x The pragma directive to be issued.
 */
#define SABUROU_PRAGMA(x) SABUROU_PRAGMA_IMPL(x)

/**
 * @brief Outputs a custom message during compilation using compiler pragmas.
 * @param x The message string to be displayed.
 */
#define SABUROU_MSG(x) SABUROU_PRAGMA(message(x))
