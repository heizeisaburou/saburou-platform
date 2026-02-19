#pragma once

/**
 * @file little.hpp
 * @brief Host to Little-Endian conversion utilities.
 */

#include <saburou/platform/v2/bytes/byte_swap.hpp>

#include <bit> // C++20 <bit>

namespace saburou::platform::v2::bytes::endian {

/** @brief True if the host is little-endian. */
constexpr bool is_little = (std::endian::native == std::endian::little);

#if !SABUROU_PLATFORM_V2_ENDIAN_UNKNOWN
    // Validamos que nuestra macro coincida con la realidad del compilador
    static_assert(
        is_little == (SABUROU_PLATFORM_V2_LITTLE_ENDIAN == 1),
        "SABUROU_PLATFORM: Little-endian macro mismatch with std::endian!"
    );
#endif

/**
 * @brief Converts a value from host native endianness to Little Endian.
 * @tparam T A type satisfying the ByteSwappable concept.
 * @param value The value in host byte order.
 * @return The value in Little Endian byte order.
 */
template <ByteSwappable T> 
[[nodiscard]] constexpr T to_little(T value) noexcept {
    if constexpr (is_little) {
        return value;
    } else {
        return byte_swap(value);
    }
}

/**
 * @brief Converts a value from Little Endian to host native endianness.
 * @note Semantically identical to to_little.
 */
template <ByteSwappable T> [[nodiscard]] constexpr T from_little(T value) noexcept { return to_little(value); }

} // namespace saburou::platform::v2::bytes::endian
