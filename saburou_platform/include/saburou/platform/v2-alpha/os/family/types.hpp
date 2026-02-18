/**
 * @file types.hpp
 * @brief OS family type definitions for saburou-platform.
 */

#pragma once

#include <saburou/platform/v2-alpha/core.hpp>

#include <cstdint>

namespace saburou::platform::v2_alpha::os {

/**
 * @brief Categorizes operating systems into logical families.
 * * Provides a high-level classification of the underlying system architecture 
 * and standards compliance (e.g., POSIX, Windows).
 */
enum class family_t : uint8_t {
    windows, // Windows desktop/server
    darwin,  // macOS, iOS, iOS Simulator
    bsd,     // FreeBSD, NetBSD, OpenBSD, DragonFly
    android, // Android (standalone category)
    wasm,    // Emscripten, WASI
    linux,   // Linux desktop/server
    posix,   // Any POSIX-Like system not explicitly covered
    unknown  // Fallback for undetected systems
};

} // namespace saburou::platform::v2_alpha::os
