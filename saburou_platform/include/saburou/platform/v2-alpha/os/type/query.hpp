/**
 * @file query.hpp
 * @brief Specific OS type query functions for saburou-platform.
 */

#pragma once

#include <saburou/platform/v2-alpha/detect.hpp>
#include <saburou/platform/v2-alpha/os/type/types.hpp>

#include <format>

namespace saburou::platform::v2_alpha::os {

/**
 * @brief Returns the specific operating system type at compile-time.
 * @return The detected type_t value. Note that detection order is prioritized (e.g., Android is checked before generic Linux).
 */
[[nodiscard]] consteval type_t type() {
    // Ordering matters: Android must be checked before generic Linux.
#if defined(SABUROU_PLATFORM_V2_ALPHA_OS_WINDOWS)
    return type_t::windows;
#elif defined(SABUROU_PLATFORM_V2_ALPHA_OS_IOS_SIMULATOR)
    return type_t::ios_simulator;
#elif defined(SABUROU_PLATFORM_V2_ALPHA_OS_IOS)
    return type_t::ios;
#elif defined(SABUROU_PLATFORM_V2_ALPHA_OS_MACOS)
    return type_t::macos;
#elif defined(SABUROU_PLATFORM_V2_ALPHA_OS_ANDROID)
    return type_t::android;
#elif defined(SABUROU_PLATFORM_V2_ALPHA_OS_LINUX)
    return type_t::linux;
#elif defined(SABUROU_PLATFORM_V2_ALPHA_OS_FREEBSD)
    return type_t::freebsd;
#elif defined(SABUROU_PLATFORM_V2_ALPHA_OS_NETBSD)
    return type_t::netbsd;
#elif defined(SABUROU_PLATFORM_V2_ALPHA_OS_OPENBSD)
    return type_t::openbsd;
#elif defined(SABUROU_PLATFORM_V2_ALPHA_OS_DRAGONFLY)
    return type_t::dragonfly;
#elif defined(SABUROU_PLATFORM_V2_ALPHA_OS_WASM_EMSCRIPTEN)
    return type_t::emscripten;
#elif defined(SABUROU_PLATFORM_V2_ALPHA_OS_WASM_WASI)
    return type_t::wasi;
#else
    return type_t::unknown;
#endif
}

/**
 * @brief Converts a type_t value to its human-readable display name.
 * @param t The OS type value to convert.
 * @return A formatted display string (e.g., "macOS", "iOS Simulator").
 */
[[nodiscard]] constexpr const char *to_name(type_t t) {
    switch (t) {
    case type_t::windows: return "Windows";
    case type_t::macos: return "macOS";
    case type_t::ios: return "iOS";
    case type_t::ios_simulator: return "iOS Simulator";
    case type_t::linux: return "Linux";
    case type_t::android: return "Android";
    case type_t::freebsd: return "FreeBSD";
    case type_t::netbsd: return "NetBSD";
    case type_t::openbsd: return "OpenBSD";
    case type_t::dragonfly: return "DragonFly";
    case type_t::emscripten: return "Emscripten";
    case type_t::wasi: return "WASI";
    case type_t::unknown: return "Unknown";
    default: return "Unknown";
    }
}

/**
 * @brief Converts a type_t value to its technical lowercase string representation.
 * @param t The OS type value to convert.
 * @return A lowercase string identifier matching the enum naming (e.g., "ios_simulator").
 */
[[nodiscard]] constexpr const char *to_code_name(type_t t) {
    switch (t) {
    case type_t::windows: return "windows";
    case type_t::macos: return "macos";
    case type_t::ios: return "ios";
    case type_t::ios_simulator: return "ios_simulator";
    case type_t::linux: return "linux";
    case type_t::android: return "android";
    case type_t::freebsd: return "freebsd";
    case type_t::netbsd: return "netbsd";
    case type_t::openbsd: return "openbsd";
    case type_t::dragonfly: return "dragonfly";
    case type_t::emscripten: return "emscripten";
    case type_t::wasi: return "wasi";
    default: return "unknown";
    }
}

/**
 * @brief Returns the human-readable name of the specific current OS type at compile-time.
 * @return The detected OS type display name string.
 */
[[nodiscard]] consteval const char *name() { return to_name(type()); }

} // namespace saburou::platform::v2_alpha::os

/**
 * @brief std::formatter specialization for type_t.
 * Supported format specifiers: {} or {:s} for technical lowercase name, {:r} for qualified representation (e.g., "type_t::linux").
 * @note Use to_name for a human-readable representation (e.g., "macOS").
 */
template <> struct std::formatter<saburou::platform::v2_alpha::os::type_t> {
    bool repr = false;
    constexpr auto parse(std::format_parse_context &ctx) {
        auto it = ctx.begin(), end = ctx.end();
        if (it != end && *it == 'r') {
            repr = true;
            ++it;
        }
        return it;
    }

    auto format(const saburou::platform::v2_alpha::os::type_t &t, std::format_context &ctx) const {
        auto name = saburou::platform::v2_alpha::os::to_code_name(t);
        return repr ? std::format_to(ctx.out(), "type_t::{}", name) : std::format_to(ctx.out(), "{}", name);
    }
};
