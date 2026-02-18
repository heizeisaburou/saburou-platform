/**
 * @file types.hpp
 * @brief Linux-specific distribution information structures.
 */

#pragma once

#include <saburou/platform/v2-alpha/core.hpp>

#include <format>
#include <string>

namespace saburou::platform::v2_alpha::os::linux {

/**
 * @brief Data structure representing Linux distribution metadata.
 * * Typically parsed from /etc/os-release or similar system files.
 */
struct distro_info_t {
    std::string id = "unknown";      ///< Machine-readable ID (e.g., "arch", "ubuntu")
    std::string name = "Unknown OS"; ///< Human-readable name (e.g., "Arch Linux")
    std::string version;             ///< Version identifier (e.g., "22.04")
    std::string build_id;            ///< Build specific identifier (e.g., "rolling")
};

} // namespace saburou::platform::v2_alpha::os::linux

/**
 * @brief std::formatter specialization for distro_info_t.
 * Supported format specifiers: {} or {:s} for filtered output (omits empty fields), 
 * {:r} for full technical representation.
 */
template <> struct std::formatter<saburou::platform::v2_alpha::os::linux::distro_info_t> {
    bool repr = false;

    constexpr auto parse(std::format_parse_context &ctx) {
        auto it = ctx.begin(), end = ctx.end();
        if (it == end || *it == '}') return it;

        if (*it == 'r') repr = true;
        else if (*it == 's') repr = false;
        else throw std::format_error("Invalid format for distro_info_t: use 'r' or 's'");

        return ++it;
    }

    auto format(const saburou::platform::v2_alpha::os::linux::distro_info_t &info, std::format_context &ctx) const {
        if (repr) {
            return std::format_to(ctx.out(), "distro_info(id={}, name={}, version={}, build_id={})", 
                                  info.id, info.name, info.version, info.build_id);
        } else {
            auto out = std::format_to(ctx.out(), "distro_info(id={}, name={}", info.id, info.name);
            if (!info.version.empty()) out = std::format_to(out, ", version={}", info.version);
            if (!info.build_id.empty()) out = std::format_to(out, ", build_id={}", info.build_id);
            return std::format_to(out, ")");
        }
    }
};
