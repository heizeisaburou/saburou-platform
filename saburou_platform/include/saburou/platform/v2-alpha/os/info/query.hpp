/**
 * @file query.hpp
 * @brief OS kernel information query functions for saburou-platform.
 */

#pragma once

#include <saburou/platform/v2-alpha/detect.hpp>
#include <saburou/platform/v2-alpha/os/info/types.hpp>

#if defined(SABUROU_PLATFORM_V2_ALPHA_POSIX_LIKE)
#include <saburou/platform/v2-alpha/os/info/detail/posix.hpp>
#endif

namespace saburou::platform::v2_alpha::os {

/**
 * @brief Retrieves detailed information about the current operating system kernel.
 * @return An info_t struct containing the kernel version and release string.
 * @note This function performs a runtime system call (e.g., uname on POSIX) to fetch live data from the kernel.
 * @note If the current platform is unsupported, it returns a default-initialized info_t (0.0.0, "unknown").
 */
[[nodiscard]] inline info_t info() {
#if defined(SABUROU_PLATFORM_V2_ALPHA_POSIX_LIKE)
    // Delegate to POSIX implementation (uname)
    return posix::info();
#elif defined(SABUROU_PLATFORM_V2_ALPHA_OS_WINDOWS)
    // Future Win32 adapter (e.g., RtlGetVersion)
    return info_t{};
#else
    // Unhandled case: return empty/default object
    return info_t{};
#endif
}

} // namespace saburou::platform::v2_alpha::os
