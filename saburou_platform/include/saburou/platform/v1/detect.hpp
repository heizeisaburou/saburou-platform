#pragma once

/**
 * @file detect.hpp
 * @brief Static environment detection engine for saburou-platform v1.
 *
 * This file provides a comprehensive set of macros for compile-time detection 
 * of compilers, standard libraries, operating systems, hardware architectures, 
 * and ABI data models.
 *
 * DESIGN PHILOSOPHY:
 * 1. Trust: 99.9% accuracy. On ambiguity, UNKNOWN is defined.
 * 2. Zero Dependencies: Freestanding ready (does not assume a full STDLIB).
 * 3. Shadowing Priority: Detection order prevents false positives from 
 * emulated toolchains (e.g., Clang emulating GCC).
 *
 * @note helpfull reference: https://sourceforge.net/p/predef/wiki/OperatingSystems/
 */


#include <saburou/platform/v1/core.hpp>
#include <version> // Defines feature-test macros for C++17/20+

// =============================================================================
// 1. C++ COMPILER
// -----------------------------------------------------------------------------
// Detects compiler type and exact version.
// Useful for compiler-specific optimizations, intrinsics, and standard support validation.
// =============================================================================
#if !defined(SABUROU_PLATFORM_V1_COMPILER_DEFINED)

    // --- NVIDIA CUDA (NVCC) ---
    // Note: NVCC usually acts as a frontend for another compiler (GCC/MSVC).
    #if defined(__NVCC__) || defined(__CUDACC__)
        #define SABUROU_PLATFORM_V1_NVCC 1
        #define MAJOR   __CUDACC_VER_MAJOR
        #define MINOR   __CUDACC_VER_MINOR
        #define PATCH   __CUDACC_VER_BUILD
        #define VERSION ((MAJOR<<16) | (MINOR<<8) | PATCH)
    #endif

    // --------------------------------------------

    // --- Intel (Classic and IntelLLVM) ---
    // /** @brief Intel Classic C/C++ compiler (icc/icpc). */
    #if defined(__INTEL_COMPILER) || defined(__ICC)
        #define SABUROU_PLATFORM_V1_INTEL_CLASSIC 1
        #define MAJOR   (__INTEL_COMPILER/100)
        #define MINOR   (__INTEL_COMPILER/10 % 10)
        #define PATCH   (__INTEL_COMPILER % 10)
        #define VERSION ((MAJOR<<16) | (MINOR<<8) | PATCH)

    /** @brief Intel LLVM-based compiler (icx/icpx). */
    #elif defined(__INTEL_LLVM_COMPILER)
        #define SABUROU_PLATFORM_V1_INTEL_LLVM 1
        #define MAJOR   __INTEL_LLVM_COMPILER
        #define MINOR   0
        #define PATCH   0
        #define VERSION ((MAJOR<<16) | (MINOR<<8) | PATCH)

    // --- Clang / LLVM ---
    #elif defined(__clang__)
        #define SABUROU_PLATFORM_V1_CLANG 1
        #define MAJOR   __clang_major__
        #define MINOR   __clang_minor__
        #define PATCH   __clang_patchlevel__
        #define VERSION ((MAJOR<<16) | (MINOR<<8) | PATCH)

    // --- Embarcadero / Borland ---
    #elif defined(__BORLANDC__) || defined(__CODEGEARC__)
        #define SABUROU_PLATFORM_V1_BORLAND 1

    // --- GCC (GNU) ---
    #elif defined(__GNUC__) || defined(__GNUG__)
        #define SABUROU_PLATFORM_V1_GCC 1
        #define MAJOR   __GNUC__
        #define MINOR   __GNUC_MINOR__
        #define PATCH   __GNUC_PATCHLEVEL__
        #define VERSION ((MAJOR<<16) | (MINOR<<8) | PATCH)

    // --- MSVC (Microsoft) ---
    #elif defined(_MSC_VER)
        #define SABUROU_PLATFORM_V1_MSVC 1
        #define MAJOR   (_MSC_VER/100)
        #define MINOR   (_MSC_VER%100)
        #define PATCH   0
        #define VERSION ((MAJOR<<16) | (MINOR<<8) | PATCH)

    #else
        // @note When detection fails, we prioritize accuracy over guesswork; however, due to vast toolchain
        // fragmentation, this remains a best-effort approach.
        #define SABUROU_PLATFORM_V1_COMPILER_UNKNOWN 1
    #endif
    #define SABUROU_PLATFORM_V1_COMPILER_DEFINED 1
#endif


// =============================================================================
// 2. C++ STDLIB (Implementation)
// -----------------------------------------------------------------------------
// Detects the standard library implementation.
// Useful for SSO detection, format/span support, and container internal layout.
// =============================================================================
#if !defined(SABUROU_PLATFORM_V1_STDLIB_DEFINED)
    // [!] requires <version>

    // libc++ (Clang ecosystem, Apple, Android NDK optional)
    #if defined(_LIBCPP_VERSION)
        #define SABUROU_PLATFORM_V1_STDLIB_LIBCXX 1

    // libstdc++ (GCC ecosystem, also used by Clang on Linux)
    #elif defined(__GLIBCXX__)
        #define SABUROU_PLATFORM_V1_STDLIB_LIBSTDCXX 1

    // MSVC STL (also used by clang-cl)
    #elif defined(_MSVC_STL_VERSION)
        #define SABUROU_PLATFORM_V1_STDLIB_MSVC 1

    #else
        #define SABUROU_PLATFORM_V1_STDLIB_UNKNOWN 1
    #endif
    #define SABUROU_PLATFORM_V1_STDLIB_DEFINED 1
#endif


// =============================================================================
// 3. PLATFORM & OS
// -----------------------------------------------------------------------------
// Detects operating system, device type, and environment (desktop, mobile, web).
// =============================================================================
// --- Windows ---
#if !defined(SABUROU_PLATFORM_V1_PLATFORM_DEFINED)
    #if defined(_WIN32) || defined(_WIN16) // _WIN32 asume _WIN64
        #define SABUROU_PLATFORM_V1_OS_WINDOWS 1
        #define SABUROU_PLATFORM_V1_DEVICE_PC 1
        #define SABUROU_PLATFORM_V1_DEVICE_DESKTOP 1
        #define SABUROU_PLATFORM_V1_DEVICE_DEFINED 1

        #if defined(_MSC_VER)
            #define SABUROU_PLATFORM_V1_LIBC_MSVC 1
        #elif defined(_UCRT)
            #define SABUROU_PLATFORM_V1_LIBC_UCRT 1
        #elif defined(__MINGW32__) || defined(__MINGW64__)
            #define SABUROU_PLATFORM_V1_LIBC_MINGW 1
        #elif defined(__MSVCRT__)
            #define SABUROU_PLATFORM_V1_LIBC_MSVCRT_DLL 1
        #elif defined(__BORLANDC__) || defined(__CODEGEARC__)
            #define SABUROU_PLATFORM_V1_LIBC_BORLAND 1
        #elif defined(__WATCOMC__)
            #define SABUROU_PLATFORM_V1_LIBC_WATCOM 1
        #else
            #define SABUROU_PLATFORM_V1_LIBC_UNKNOWN 1
        #endif
        #define SABUROU_PLATFORM_V1_LIBC_DEFINED 1

        #if defined(_WIN16)
            /** @brief Legacy 16-bit Windows support. */
            #define SABUROU_PLATFORM_V1_BITNESS_16 1
        #elif defined(_WIN64)
            /** @brief Standard 64-bit Windows (x64/ARM64). */
            #define SABUROU_PLATFORM_V1_BITNESS_64 1
        #else
            /** @brief Standard 32-bit Windows (x86/ARM). */
            #define SABUROU_PLATFORM_V1_BITNESS_32 1
        #endif
        #define SABUROU_PLATFORM_V1_BITNESS_DEFINED 1

        #define SABUROU_PLATFORM_V1_IS_NOT_POSIX 1
        #define SABUROU_PLATFORM_V1_PLATFORM_DEFINED 1
    #endif
#endif


// --- Apple ---
#if !defined(SABUROU_PLATFORM_V1_PLATFORM_DEFINED)
    #if (defined(__APPLE__) && defined(__MACH__))
        #include <TargetConditionals.h>

        #define SABUROU_PLATFORM_V1_OS_DARWIN 1

        #define SABUROU_PLATFORM_V1_LIBC_APPLE 1
        #define SABUROU_PLATFORM_V1_LIBC_DEFINED 1
        
        /* @note The Simulator is prioritized because Apple defines TARGET_OS_IPHONE as true for both physical
         * devices and simulated environments. */
        #if defined(TARGET_OS_SIMULATOR) && TARGET_OS_SIMULATOR
            #define SABUROU_PLATFORM_V1_OS_IOS 1
            #define SABUROU_PLATFORM_V1_OS_IOS_SIMULATOR 1

            #define SABUROU_PLATFORM_V1_DEVICE_MOBILE 1
            #define SABUROU_PLATFORM_V1_DEVICE_DEFINED 1

        #elif defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
            #define SABUROU_PLATFORM_V1_OS_IOS 1

            #define SABUROU_PLATFORM_V1_DEVICE_MOBILE 1
            #define SABUROU_PLATFORM_V1_DEVICE_DEFINED 1

        #else
            #define TARGET_OS_OSX 1
            #define SABUROU_PLATFORM_V1_OS_MACOS 1

            #define SABUROU_PLATFORM_V1_DEVICE_MAC 1
            #define SABUROU_PLATFORM_V1_DEVICE_DESKTOP 1
            #define SABUROU_PLATFORM_V1_DEVICE_DEFINED 1
        #endif
        #define SABUROU_PLATFORM_V1_POSIX_LIKE 1
        #define SABUROU_PLATFORM_V1_PLATFORM_DEFINED 1
    #endif
#endif

// --- BSD ---
#if !defined(SABUROU_PLATFORM_V1_PLATFORM_DEFINED)
    #if (defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__))
        #define SABUROU_PLATFORM_V1_OS_BSD 1

        #define SABUROU_PLATFORM_V1_DEVICE_PC 1
        #define SABUROU_PLATFORM_V1_DEVICE_DESKTOP 1
        #define SABUROU_PLATFORM_V1_DEVICE_DEFINED 1

        #define SABUROU_PLATFORM_V1_LIBC_BSD 1
        #define SABUROU_PLATFORM_V1_LIBC_DEFINED 1

        #if defined(__FreeBSD__)
            #define SABUROU_PLATFORM_V1_OS_FREEBSD 1

        #elif defined(__NetBSD__)
            #define SABUROU_PLATFORM_V1_OS_NETBSD 1

        #elif defined(__OpenBSD__)
            #define SABUROU_PLATFORM_V1_OS_OPENBSD 1

        #elif defined(__DragonFly__)
            #define SABUROU_PLATFORM_V1_OS_DRAGONFLY 1

        #else
            #define SABUROU_PLATFORM_V1_OS_BSD_UNKNOWN 1
        #endif

        #define SABUROU_PLATFORM_V1_POSIX_LIKE 1
        #define SABUROU_PLATFORM_V1_PLATFORM_DEFINED 1
    #endif
#endif

// --- Android ---
#if !defined(SABUROU_PLATFORM_V1_PLATFORM_DEFINED)
    #if defined(__ANDROID__)
        #define SABUROU_PLATFORM_V1_OS_ANDROID 1
        #define SABUROU_PLATFORM_V1_OS_LINUX 1

        #define SABUROU_PLATFORM_V1_DEVICE_MOBILE 1
        #define SABUROU_PLATFORM_V1_DEVICE_DEFINED 1

        #define SABUROU_PLATFORM_V1_LIBC_BIONIC 1
        #define SABUROU_PLATFORM_V1_LIBC_DEFINED 1

        #define SABUROU_PLATFORM_V1_POSIX_LIKE 1
        #define SABUROU_PLATFORM_V1_PLATFORM_DEFINED 1
    #endif
#endif

// --- WebAssembly / WASM ---
#if !defined(SABUROU_PLATFORM_V1_PLATFORM_DEFINED)
    // Emscripten (corre en navegador, hosted)
    #if defined(__EMSCRIPTEN__)
        #define SABUROU_PLATFORM_V1_OS_WASM 1
        #define SABUROU_PLATFORM_V1_OS_WASM_EMSCRIPTEN 1

        #define SABUROU_PLATFORM_V1_DEVICE_WEB 1
        #define SABUROU_PLATFORM_V1_DEVICE_DEFINED 1

        #define SABUROU_PLATFORM_V1_LIBC_MUSL 1
        #define SABUROU_PLATFORM_V1_LIBC_DEFINED 1

        #define SABUROU_PLATFORM_V1_ENV_HOSTED 1
        #define SABUROU_PLATFORM_V1_ENV_SANDBOXED 1
        #define SABUROU_PLATFORM_V1_ENV_DEFINED 1

        #define SABUROU_PLATFORM_V1_LITTLE_ENDIAN 1
        #define SABUROU_PLATFORM_V1_ENDIAN_DEFINED 1

        #define SABUROU_PLATFORM_V1_POSIX_LIKE 1
        #define SABUROU_PLATFORM_V1_PLATFORM_DEFINED 1
    // WASI standalone (server / freestanding)
    #elif defined(__wasm__) && defined(__wasi__)
        #define SABUROU_PLATFORM_V1_OS_WASM 1
        #define SABUROU_PLATFORM_V1_OS_WASM_WASI 1

        #define SABUROU_PLATFORM_V1_DEVICE_CLOUD 1
        #define SABUROU_PLATFORM_V1_DEVICE_DEFINED 1

        #define SABUROU_PLATFORM_V1_LIBC_WASI 1
        #define SABUROU_PLATFORM_V1_LIBC_DEFINED 1

        #define SABUROU_PLATFORM_V1_ENV_EMBEDDED 1
        #define SABUROU_PLATFORM_V1_ENV_DEFINED 1

        #define SABUROU_PLATFORM_V1_LITTLE_ENDIAN 1
        #define SABUROU_PLATFORM_V1_ENDIAN_DEFINED 1

        #define SABUROU_PLATFORM_V1_POSIX_LIKE 1
        #define SABUROU_PLATFORM_V1_PLATFORM_DEFINED 1
    #endif
#endif

// --- Linux (Requiere C++17) ---
#if !defined(SABUROU_PLATFORM_V1_PLATFORM_DEFINED)
    #if defined(__linux__)
        #define SABUROU_PLATFORM_V1_OS_LINUX 1

        #define SABUROU_PLATFORM_V1_DEVICE_GENERAL 1
        #define SABUROU_PLATFORM_V1_DEVICE_DEFINED 1

        #if __has_include(<features.h>)
            // Habilitar macros para detección (fallaría en mac/Windows)
            #include <features.h>
        #endif

        #if defined(__GLIBC__) || defined(__GNU_LIBRARY__)
            #define SABUROU_PLATFORM_V1_LIBC_GLIBC 1

        #elif defined(__LLVM_LIBC__) // nuevo ecosistema clang
            #define SABUROU_PLATFORM_V1_LIBC_LLVM 1

        #elif defined(__KLIBC__) // initramfs / early boot userspace
            #define SABUROU_PLATFORM_V1_LIBC_KLIBC 1

        #elif defined(__NEWLIB__) // normalmente híbridos raros
            #define SABUROU_PLATFORM_V1_LIBC_NEWLIB 1

        #elif defined(__UCLIBC__) // embebidos
            #define SABUROU_PLATFORM_V1_LIBC_UCLIBC 1

        #elif defined(__dietlibc__) // embebidos, raro
            #define SABUROU_PLATFORM_V1_LIBC_DIETLIBC 1

        #elif defined(__BIONIC__) // linux con la biblioteca de android (no android)
            #define SABUROU_PLATFORM_V1_LIBC_BIONIC 1

        #elif defined(__MUSL__) || __has_include(<musl/version.h>)
            #define SABUROU_PLATFORM_V1_LIBC_MUSL 1

        #else
            // #define SABUROU_PLATFORM_V1_LIBC_MUSL 1
            //                                  ^^^
            #define SABUROU_PLATFORM_V1_LIBC_PROBABLY_MUSL 1
            #define SABUROU_PLATFORM_V1_LIBC_UNKNOWN 1
        #endif
        #define SABUROU_PLATFORM_V1_LIBC_DEFINED 1
        #define SABUROU_PLATFORM_V1_POSIX_LIKE 1
        #define SABUROU_PLATFORM_V1_PLATFORM_DEFINED 1
    #endif
#endif

// [FALLBACKS + POSIX]
// -- Platform --
#if !defined(SABUROU_PLATFORM_V1_PLATFORM_DEFINED)
    #define SABUROU_PLATFORM_V1_PLATFORM_UNKNOWN 1
    #define SABUROU_PLATFORM_V1_PLATFORM_DEFINED 1
#endif

// -- Device --
#if !defined(SABUROU_PLATFORM_V1_DEVICE_DEFINED)
    #define SABUROU_PLATFORM_V1_DEVICE_UNKNOWN 1
    #define SABUROU_PLATFORM_V1_DEVICE_DEFINED 1
#endif

// -- LibC --
#if (defined(SABUROU_PLATFORM_V1_OS_WINDOWS) || defined(SABUROU_PLATFORM_V1_POSIX_LIKE)) && \
    !defined(SABUROU_PLATFORM_V1_LIBC_DEFINED)
    #define SABUROU_PLATFORM_V1_LIBC_UNKNOWN 1
    #define SABUROU_PLATFORM_V1_LIBC_DEFINED 1
#endif

// -- Posix --
#if !defined(SABUROU_PLATFORM_V1_POSIX_LIKE) && !defined(SABUROU_PLATFORM_V1_IS_NOT_POSIX)
    #if __has_include(<unistd.h>)
        #include <unistd.h>
        #if defined(_POSIX_VERSION)
            #define SABUROU_PLATFORM_V1_POSIX_LIKE 1
        #endif
    #else
        #define SABUROU_PLATFORM_V1_IS_NOT_POSIX 1
    #endif
#endif


// =============================================================================
// 4. ARCHITECTURE (ISA)
// -----------------------------------------------------------------------------
// Detects the Instruction Set Architecture (x86, ARM, RISC-V, PPC, etc.).
// =============================================================================
#if !defined(SABUROU_PLATFORM_V1_ARCH_DEFINED)
    #if defined(__x86_64__) || defined(_M_X64)
    // --- x86 ---
        #define SABUROU_PLATFORM_V1_ARCH_X86_64 1
        #define SABUROU_PLATFORM_V1_ARCH_X86 1
    #elif defined(__i386__) || defined(_M_IX86)
        #define SABUROU_PLATFORM_V1_ARCH_X86_32 1
        #define SABUROU_PLATFORM_V1_ARCH_X86 1
    // --- ARM ---
    #elif defined(__aarch64__) || defined(_M_ARM64)
        #define SABUROU_PLATFORM_V1_ARCH_ARM_64 1
        #define SABUROU_PLATFORM_V1_ARCH_ARM 1
    #elif defined(__arm__) || defined(_M_ARM)
        #define SABUROU_PLATFORM_V1_ARCH_ARM_32 1
        #define SABUROU_PLATFORM_V1_ARCH_ARM 1
    // --- RISC-V ---
    #elif defined(__riscv) || defined(__riscv__)
        #define SABUROU_PLATFORM_V1_ARCH_RISCV 1
        #if defined(__riscv_xlen) && (__riscv_xlen == 64)
            #define SABUROU_PLATFORM_V1_ARCH_RISCV_64 1
        #else
            #define SABUROU_PLATFORM_V1_ARCH_RISCV_32 1
        #endif
    // --- PowerPC ---
    #elif defined(__ppc64__) || defined(_ARCH_PPC64)
        #define SABUROU_PLATFORM_V1_ARCH_PPC_64 1
        #define SABUROU_PLATFORM_V1_ARCH_PPC 1
    #elif defined(__ppc__) || defined(_ARCH_PPC)
        #define SABUROU_PLATFORM_V1_ARCH_PPC_32 1
        #define SABUROU_PLATFORM_V1_ARCH_PPC 1
    #else
        #define SABUROU_PLATFORM_V1_ARCH_UNKNOWN 1
    #endif
    #define SABUROU_PLATFORM_V1_ARCH_DEFINED 1
#endif


// =============================================================================
// 5. CACHELINE TUNING
// -----------------------------------------------------------------------------
// Detects cache line size to prevent false sharing.
// =============================================================================
// --- C++20 Attempt (Requires <version> and STDLIB support) --
#if !defined(SABUROU_PLATFORM_V1_CACHELINE_DEFINED)
    #if defined(__cpp_lib_hardware_interference_size)
        /** @note about __cpp_lib_hardware_interference_size):
        * - Presence depends on the Standard Library (STDLIB) implementation.
        * - Even if the compiler supports C++20, some STDLIBs may leave this undefined to avoid committing
        *   to a fixed ABI value that might change between different CPU generations. */
        #include <new> // IWYU pragma: keep
        #define SABUROU_PLATFORM_V1_CACHELINE std::hardware_destructive_interference_size

    #else
        #define SABUROU_PLATFORM_V1_CACHELINE_UNKNOWN 1
    #endif

    #define SABUROU_PLATFORM_V1_CACHELINE_DEFINED 1
#endif


// =============================================================================
// 6. BITNESS (32/64 bit)
// =============================================================================
#if !defined(SABUROU_PLATFORM_V1_BITNESS_DEFINED)
    #if defined(SABUROU_PLATFORM_V1_ARCH_X86_64) || \
        defined(SABUROU_PLATFORM_V1_ARCH_ARM_64) || \
        defined(SABUROU_PLATFORM_V1_ARCH_RISCV_64) || \
        defined(SABUROU_PLATFORM_V1_ARCH_PPC_64)
        #define SABUROU_PLATFORM_V1_BITNESS_64 1
    #elif defined(__LP64__) || defined(_LP64) || defined(_WIN64) || defined(__x86_64__)
        #define SABUROU_PLATFORM_V1_BITNESS_64 1
    #elif defined(SABUROU_PLATFORM_V1_ARCH_X86_32) || \
          defined(SABUROU_PLATFORM_V1_ARCH_ARM_32) || \
          defined(SABUROU_PLATFORM_V1_ARCH_PPC_32) || \
          defined(SABUROU_PLATFORM_V1_ARCH_RISCV_32)
        #define SABUROU_PLATFORM_V1_BITNESS_32 1
    #elif defined(__ILP32__) || defined(_ILP32) || defined(__i386__) || defined(_M_IX86)
        #define SABUROU_PLATFORM_V1_BITNESS_32 1
    #else
        #define SABUROU_PLATFORM_V1_BITNESS_UNKNOWN 1
    #endif
    #define SABUROU_PLATFORM_V1_BITNESS_DEFINED 1
#endif


// =============================================================================
// 7. ENDIANNESS
// -----------------------------------------------------------------------------
// Detects byte order (Little vs Big Endian).
// =============================================================================
#if !defined(SABUROU_PLATFORM_V1_ENDIAN_DEFINED)

    // ---- Modern compilers (GCC, Clang, ICC, etc.)
    #if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && defined(__ORDER_BIG_ENDIAN__)
        #if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
            #define SABUROU_PLATFORM_V1_LITTLE_ENDIAN 1
        #elif (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
            #define SABUROU_PLATFORM_V1_BIG_ENDIAN 1
        #endif

    // ---- Traditional endian defines (older toolchains, BSD, etc.)
    #elif defined(__LITTLE_ENDIAN__) || defined(_LITTLE_ENDIAN) || defined(__ARMEL__) || defined(__THUMBEL__)
        #define SABUROU_PLATFORM_V1_LITTLE_ENDIAN 1

    #elif defined(__BIG_ENDIAN__) || defined(_BIG_ENDIAN) || defined(__ARMEB__) || defined(__THUMBEB__)
        #define SABUROU_PLATFORM_V1_BIG_ENDIAN 1

    // ---- Architecture heuristics (last-resort but safe in practice)
    // x86/x64 are ALWAYS little endian
    #elif defined(SABUROU_PLATFORM_V1_ARCH_X86)
        #define SABUROU_PLATFORM_V1_LITTLE_ENDIAN 1

    // AArch64 in all mainstream OSes is little endian
    #elif defined(SABUROU_PLATFORM_V1_ARCH_ARM_64)
        #define SABUROU_PLATFORM_V1_LITTLE_ENDIAN 1

    // RISC-V is little endian in every real-world deployment
    #elif defined(SABUROU_PLATFORM_V1_ARCH_RISCV)
        #define SABUROU_PLATFORM_V1_LITTLE_ENDIAN 1

    // PowerPC is the only one where big endian is still common
    #elif defined(SABUROU_PLATFORM_V1_ARCH_PPC)
        #define SABUROU_PLATFORM_V1_BIG_ENDIAN 1

    #else
        #define SABUROU_PLATFORM_V1_ENDIAN_UNKNOWN 1

    #endif

    #define SABUROU_PLATFORM_V1_ENDIAN_DEFINED 1
#endif


// =============================================================================
// 8. ABI (Data Model)
// -----------------------------------------------------------------------------
// Differentiates LP64, LLP64, and ILP32 data models.
// @note Differentiates between LP64 (POSIX) and LLP64 (Windows) to ensure safe pointer arithmetic and
// predictable memory layout.
// =============================================================================
#if !defined(SABUROU_PLATFORM_V1_ABI_DEFINED)

    // --- 1. Microsoft Exception ---
    #if defined(SABUROU_PLATFORM_V1_OS_WINDOWS) && defined(SABUROU_PLATFORM_V1_BITNESS_64)
        #define SABUROU_PLATFORM_V1_ABI_LLP64 1

    // --- 2.A Operating System Truth ---
    // Most 64-bit POSIX-compliant systems use the LP64 data model.
    #elif defined(SABUROU_PLATFORM_V1_POSIX_LIKE) && defined(SABUROU_PLATFORM_V1_BITNESS_64)
        #define SABUROU_PLATFORM_V1_ABI_LP64 1

    // --- 2.B Operating System Truth (Standard Macros) ---
    // Direct detection via compiler predefined macros.
    #elif defined(__LP64__) || defined(_LP64)
        #define SABUROU_PLATFORM_V1_ABI_LP64 1
        
    #elif defined(__ILP32__) || defined(_ILP32)
        #define SABUROU_PLATFORM_V1_ABI_ILP32 1

    // --- 3. 32-bit Legacy/Standard (Windows and POSIX converge here) ---
    // Both Windows and POSIX typically align on ILP32 for 32-bit architectures.
    #elif defined(SABUROU_PLATFORM_V1_BITNESS_32)
        #define SABUROU_PLATFORM_V1_ABI_ILP32 1

    // --- 4. Pointer Size Fallback (Final Heuristic) ---
    // Final detection layer based on the physical byte size of a data pointer.
    // @note Caution: This heuristic depends on compiler macros and may yield false 
    // positives if previous platform checks are bypassed or undefined.
    #elif defined(__SIZEOF_POINTER__)
        #if (__SIZEOF_POINTER__ == 8)
            #define SABUROU_PLATFORM_V1_ABI_LP64 1 
        #elif (__SIZEOF_POINTER__ == 4)
            #define SABUROU_PLATFORM_V1_ABI_ILP32 1
        #else
            #define SABUROU_PLATFORM_V1_ABI_UNKNOWN 1
        #endif
    #else
        #define SABUROU_PLATFORM_V1_ABI_UNKNOWN 1
    #endif

    #define SABUROU_PLATFORM_V1_ABI_DEFINED 1
#endif


// =============================================================================
// 9. ENVIRONMENT (Hosted vs Freestanding)
// =============================================================================
#if !defined(SABUROU_PLATFORM_V1_ENV_DEFINED)
    #if defined(__STDC_HOSTED__) && (__STDC_HOSTED__ == 1)
        /** @brief Hosted environment with a full standard C library and OS. */
        #define SABUROU_PLATFORM_V1_ENV_HOSTED 1
    #else
        /** @brief Bare-metal or kernel environment without standard OS services. */
        #define SABUROU_PLATFORM_V1_ENV_FREESTANDING 1
    #endif

    #define SABUROU_PLATFORM_V1_ENV_DEFINED 1
#endif


