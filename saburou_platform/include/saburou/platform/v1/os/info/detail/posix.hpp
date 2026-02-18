/**
 * @file posix.hpp
 * @brief POSIX-specific implementation for retrieving kernel information.
 */

#pragma once

#include <saburou/platform/v1/os/info/types.hpp>

#include <sys/utsname.h>

namespace saburou::platform::v1::os::posix {

/**
 * @brief Retrieves kernel information using the POSIX uname system call.
 * @return An info_t struct populated with version numbers and the raw release string.
 * @note This implementation parses the release string to extract major, minor, and patch versions.
 */
inline saburou::platform::v1::os::info_t info() {
    utsname buffer;
    saburou::platform::v1::os::info_t info{};

    if (uname(&buffer) == 0) {
        // Parse the first three version numbers from the release string
        std::sscanf(buffer.release, "%d.%d.%d", &info.version.major, &info.version.minor, &info.version.patch);
        info.release_str = buffer.release;
    }
    return info;
}

} // namespace saburou::platform::v1::os::posix
