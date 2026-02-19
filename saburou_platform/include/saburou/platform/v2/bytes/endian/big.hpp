#pragma once

/**
 * @file big.hpp
 * @brief Host to Big-Endian conversion utilities.
 */

#include <saburou/platform/v2/bytes/byte_swap.hpp>

#include <bit> // C++20 <bit>

namespace saburou::platform::v2::bytes::endian {

/** @brief True if the host is big-endian. */
constexpr bool is_big = (std::endian::native == std::endian::big);

#if !SABUROU_PLATFORM_V2_ENDIAN_UNKNOWN
    // Validamos que nuestra macro coincida con la realidad del compilador
    static_assert(
        is_big == (SABUROU_PLATFORM_V2_BIG_ENDIAN == 1),
        "SABUROU_PLATFORM: Big-endian macro mismatch with std::endian!"
    );
#endif

/**
 * @brief Converts a value from host native endianness to Big Endian.
 * @tparam T A type satisfying the ByteSwappable concept.
 * @param value The value in host byte order.
 * @return The value in Big Endian byte order.
 */
template <ByteSwappable T> 
[[nodiscard]] constexpr T to_big(T value) noexcept {
    if constexpr (is_big) {
        return value;
    } else {
        return byte_swap(value);
    }
}

/**
 * @brief Converts a value from Big Endian to host native endianness.
 * @note Semantically identical to to_big due to swap involution.
 */
template <ByteSwappable T> [[nodiscard]] constexpr T from_big(T value) noexcept { return to_big(value); }

} // namespace saburou::platform::v2::bytes::endian
