/**
 * @file types.hpp
 * @brief Detailed OS type definitions.
 */

#pragma once

#include <saburou/platform/v2-alpha/core.hpp>

#include <cstdint>

namespace saburou::platform::v2_alpha::os {

/**
 * @brief Explicit operating system types.
 * * Provides granular identification of specific distributions, 
 * kernels, and environments.
 */
enum class type_t : uint16_t {
    windows,
    macos,
    ios,
    ios_simulator,
    linux,
    android,
    freebsd,
    netbsd,
    openbsd,
    dragonfly,
    emscripten,
    wasi,
    unknown
};

} // namespace saburou::platform::v2_alpha::os
