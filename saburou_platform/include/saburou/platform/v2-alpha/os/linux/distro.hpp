/**
 * @file distro.hpp
 * @brief Linux distribution information query functions for saburou-platform.
 */

#pragma once

#include <saburou/platform/v2-alpha/os/linux/types.hpp>

#include <fstream>

namespace saburou::platform::v2_alpha::os::linux {

/**
 * @brief Parses the /etc/os-release file to retrieve Linux distribution metadata.
 * @return A distro_info_t struct containing fields like ID, NAME, VERSION_ID, and BUILD_ID.
 * @note This function is only relevant on Linux-based systems. On other platforms or if /etc/os-release is
 * missing, it returns an empty distro_info_t.
 * @note Values are automatically stripped of surrounding double quotes if present in the source file.
 */
inline distro_info_t distro_info() {
    distro_info_t info{};

    std::ifstream file("/etc/os-release");
    if (!file.is_open()) return info;

    std::string line;
    while (std::getline(file, line)) {
        std::string_view view(line);
        auto pos = view.find('=');
        if (pos == std::string_view::npos) continue;

        std::string_view key = view.substr(0, pos);
        std::string_view val_part = view.substr(pos + 1);
        std::string_view val = val_part;
        if (val.size() >= 2 && val.starts_with('"') && val.ends_with('"')) {
            val.remove_prefix(1);
            val.remove_suffix(1);
        }

        if (key == "ID")
            info.id = val;
        else if (key == "NAME")
            info.name = val;
        else if (key == "VERSION_ID")
            info.version = val;
        else if (key == "BUILD_ID")
            info.build_id = val;
    }

    return info;
}

} // namespace saburou::platform::v2_alpha::os::linux
