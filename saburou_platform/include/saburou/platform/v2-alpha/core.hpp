/**
 * @file core.hpp
 * @brief Initialization and validation core for saburou-platform v1.
 * * This header acts as the primary configuration layer. It enforces C++ standard 
 * requirements, validates core revision integrity to prevent ODR violations, 
 * and sanitizes the global environment by removing conflicting legacy macros.
 */

#pragma once

// Guards
/** @brief Local revision identifier used by core_guard to verify binary compatibility. */
#define SABUROU_PLATFORM_CORE_REVISION_LOCAL 1
#include <saburou/platform/detail/gen1/core_guard.hpp> // IWYU pragma: keep

// SABUROU_MSG(SABUROU_STRINGIFY(__cplusplus))
/** @brief Minimum C++ standard required for this version. */
#define SABUROU_PLATFORM_MIN_CPP_VER 202302L
#define SABUROU_PLATFORM_MIN_CPP_NAME "C++23"
#include <saburou/macros/cppxx_guard.hpp> // IWYU pragma: keep

// Necessary environment cleanup (undefining linux, unix, etc.)
#include <saburou/platform/v2-alpha/env/cleanup.hpp> // IWYU pragma: keep

// Pragma utility for diagnostic reporting
#include <saburou/macros/pragma.hpp> // IWYU pragma: keep
