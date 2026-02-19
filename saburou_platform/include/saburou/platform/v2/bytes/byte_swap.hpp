#pragma once

/**
 * @file bytes.hpp
 * @version 1.0
 * @brief High-performance byte manipulation utilities.
 *
 * This module provides generic, type-safe byte swapping functionality
 * leveraging C++20/23 features for optimal code generation across
 * different architectures and compilers.
 */

#include <saburou/platform/v2/detect.hpp>

#include <algorithm> // std::ranges::reverse (C++20)
#include <array>
#include <bit> // std::bit_cast, std::byteswap (C++23)
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace saburou::platform::v2::bytes {

/**
 * @concept ByteSwappable
 * @brief Requirements for types that can be safely byte-swapped.
 *
 * A type is ByteSwappable if:
 * 1. It is trivially copyable (can be moved/copied via bitwise copy).
 * 2. It has unique object representations (no padding bits that could cause UB
 * when swapped, ensuring consistent bit-casting).
 */
template <class T>
concept ByteSwappable = std::is_trivially_copyable_v<T> && std::has_unique_object_representations_v<T>;

/**
 * @brief Swaps the byte order of a given value.
 *
 * Performs a byte-order reversal (endianness swap).
 * - For standard sizes (16, 32, 64 bits), it uses compiler-optimized intrínsecs via std::byteswap.
 * - For non-standard sizes (like 128-bit structures or custom types), it employs a constant-expression safe
 *   memory reversal.
 *
 * @tparam T A type satisfying the ByteSwappable concept.
 * @param value The value to swap.
 * @return T The value with its byte order reversed.
 * @note Requires C++23 for std::byteswap support.
 * @note Zero-overhead: On modern compilers, the generic fallback for
 * large types is often optimized into specialized SIMD instructions.
 */
template <ByteSwappable T> [[nodiscard]] constexpr T byte_swap(T value) noexcept {
    // Implementation Detail: We use if constexpr to select the most
    // efficient path at compile-time based on the size of T.

    if constexpr (sizeof(T) == 1) {
        return value;
    } else if constexpr (sizeof(T) == 2) {
        auto v = std::bit_cast<std::uint16_t>(value);
        return std::bit_cast<T>(std::byteswap(v));
    } else if constexpr (sizeof(T) == 4) {
        auto v = std::bit_cast<std::uint32_t>(value);
        return std::bit_cast<T>(std::byteswap(v));
    } else if constexpr (sizeof(T) == 8) {
        auto v = std::bit_cast<std::uint64_t>(value);
        return std::bit_cast<T>(std::byteswap(v));
    } else {
        /**
         * GENERIC FALLBACK:
         * Uses std::bit_cast to treat the type as a raw byte array,
         * reverses it using C++20 ranges, and casts it back.
         */
        auto bytes = std::bit_cast<std::array<std::byte, sizeof(T)>>(value);
        std::ranges::reverse(bytes);
        return std::bit_cast<T>(bytes);
    }
}

} // namespace saburou::platform::v2::bytes
