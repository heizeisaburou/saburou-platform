/**
 * @file query.hpp
 * @brief OS family query functions for saburou-platform.
 */

#pragma once

#include <saburou/platform/v1/detect.hpp>
#include <saburou/platform/v1/os/family/types.hpp>

#include <format>

namespace saburou::platform::v1::os {

/**
 * @brief Returns the current operating system family at compile-time.
 * @return The detected family_t value based on platform detection macros.
 */
[[nodiscard]] consteval family_t family() {
#if defined SABUROU_PLATFORM_V1_OS_WINDOWS
    return family_t::windows;
#elif defined SABUROU_PLATFORM_V1_OS_DARWIN
    return family_t::darwin;
#elif defined SABUROU_PLATFORM_V1_OS_BSD
    return family_t::bsd;
#elif defined SABUROU_PLATFORM_V1_OS_ANDROID
    return family_t::android;
#elif defined SABUROU_PLATFORM_V1_OS_WASM
    return family_t::wasm;
#elif defined SABUROU_PLATFORM_V1_OS_LINUX
    return family_t::linux;
#elif defined SABUROU_PLATFORM_V1_OS_POSIX
    return family_t::posix;
#else
    return family_t::unknown;
#endif
}

/**
 * @brief Converts a family_t value to its technical lowercase string representation.
 * @param f The OS family value to convert.
 * @return A lowercase string identifier (e.g., "linux", "darwin").
 */
[[nodiscard]] constexpr const char *to_code_name(family_t f) {
    switch (f) {
    case family_t::windows: return "windows";
    case family_t::darwin:  return "darwin";
    case family_t::bsd:     return "bsd";
    case family_t::android: return "android";
    case family_t::wasm:    return "wasm";
    case family_t::linux:   return "linux";
    case family_t::posix:   return "posix";
    default:                return "unknown";
    }
}

/**
 * @brief Converts a family_t value to its human-readable display name.
 * @param f The OS family value to convert.
 * @return A formatted display string (e.g., "Windows", "WebAssembly").
 */
[[nodiscard]] constexpr const char *to_family_name(family_t f) {
    switch (f) {
    case family_t::windows: return "Windows";
    case family_t::darwin:  return "Darwin";
    case family_t::bsd:     return "BSD";
    case family_t::android: return "Android";
    case family_t::wasm:    return "WebAssembly";
    case family_t::linux:   return "Linux";
    case family_t::posix:   return "POSIX";
    case family_t::unknown: return "Unknown";
    default:                return "Unknown";
    }
}

/**
 * @brief Returns the human-readable name of the current OS family at compile-time.
 * @return The detected OS family display name string.
 */
[[nodiscard]] consteval const char *family_name() { return to_family_name(family()); }

} // namespace saburou::platform::v1::os

/**
 * @brief std::formatter specialization for family_t.
 * Supported format specifiers: {} or {:s} for technical lowercase name, {:r} for qualified representation (e.g., "family_t::linux").
 * @note Use to_family_name for a human-readable representation (e.g., "Linux").
 */
template <> struct std::formatter<saburou::platform::v1::os::family_t> {
    bool repr = false;
    constexpr auto parse(std::format_parse_context &ctx) {
        auto it = ctx.begin(), end = ctx.end();
        if (it != end && *it == 'r') {
            repr = true;
            ++it;
        }
        return it;
    }

    auto format(const saburou::platform::v1::os::family_t &f, std::format_context &ctx) const {
        auto name = saburou::platform::v1::os::to_code_name(f);
        return repr ? std::format_to(ctx.out(), "family_t::{}", name) : std::format_to(ctx.out(), "{}", name);
    }
};
