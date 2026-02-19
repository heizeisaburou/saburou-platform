/**
 * @file cleanup.hpp
 * @brief Umbrella header for environment sanitization.
 *
 * Automatically detects and undefines conflicting legacy platform macros (like 'linux' or 'unix') 
 * to ensure a clean global namespace for modern C++ development within saburou-platform.
 */

#pragma once

#include <saburou/platform/detail/gen1/env_cleanup/cleanup.hpp> // IWYU pragma: keep
