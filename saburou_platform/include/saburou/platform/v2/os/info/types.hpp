/**
 * @file types.hpp
 * @brief Operating system information structures and formatters.
 */

#pragma once

#include <saburou/platform/v2/core.hpp>

#include <format>
#include <string>

namespace saburou::platform::v2::os {

/**
 * @brief Represents a semantic version of the operating system.
 */
struct version_t {
    int major = 0; ///< Major version number
    int minor = 0; ///< Minor version number
    int patch = 0; ///< Patch/Build version number
};

/**
 * @brief Aggregated operating system information including version and release string.
 */
struct info_t {
    version_t version;       ///< Structured version data
    std::string release_str; ///< Raw release or build string from the system
};

} // namespace saburou::platform::v2::os

/**
 * @brief std::formatter specialization for version_t.
 * Supported format specifiers: {} or {:s} for "M.m.p", {:r} for "version(major=M, minor=m, patch=p)".
 */
template <> struct std::formatter<saburou::platform::v2::os::version_t> {
    bool repr = false;

    constexpr auto parse(std::format_parse_context &ctx) {
        auto it = ctx.begin(), end = ctx.end();
        if (it == end || *it == '}') return it;

        if (*it == 'r')
            repr = true;
        else if (*it == 's')
            repr = false;
        else
            throw std::format_error("Invalid format for version_t: use 'r' or 's'");

        return ++it;
    }

    auto format(const saburou::platform::v2::os::version_t &v, std::format_context &ctx) const {
        if (repr) {
            return std::format_to(
                ctx.out(), "version(major={}, minor={}, patch={})", v.major, v.minor, v.patch);
        }
        return std::format_to(ctx.out(), "{}.{}.{}", v.major, v.minor, v.patch);
    }
};

/**
 * @brief std::formatter specialization for info_t.
 * Supported format specifiers: {} or {:s} for standard output, {:r} for detailed representation.
 */
template <> struct std::formatter<saburou::platform::v2::os::info_t> {
    bool repr = false;

    constexpr auto parse(std::format_parse_context &ctx) {
        auto it = ctx.begin(), end = ctx.end();
        if (it == end || *it == '}') return it;

        if (*it == 'r')
            repr = true;
        else if (*it == 's')
            repr = false;
        else
            throw std::format_error("Invalid format for info_t: use 'r' or 's'");

        return ++it;
    }

    auto format(const saburou::platform::v2::os::info_t &t, std::format_context &ctx) const {
        if (repr) {
            return std::format_to(
                ctx.out(), "os_info(version={:r}, release_str={})", t.version, t.release_str);
        }
        return std::format_to(ctx.out(), "os_info(version={}, release_str={})", t.version, t.release_str);
    }
};
