#pragma once

/**
 * @file detect.hpp
 * @brief Static environment detection engine for saburou-platform.
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


#include <saburou/platform/v2/core.hpp>
#include <version> // Defines feature-test macros for C++17/20+

// =============================================================================
// 1. C++ COMPILER
// -----------------------------------------------------------------------------
// Detects compiler type and exact version.
// Useful for compiler-specific optimizations, intrinsics, and standard support validation.
// =============================================================================

// --- NVIDIA CUDA (NVCC) ---
// Note: NVCC usually acts as a frontend for another compiler (GCC/MSVC).
#if defined(__NVCC__) || defined(__CUDACC__)
    #define SABUROU_PLATFORM_V2_NVCC 1
    #define SABUROU_PLATFORM_V2_CUDA_MAJOR   __CUDACC_VER_MAJOR
    #define SABUROU_PLATFORM_V2_CUDA_MINOR   __CUDACC_VER_MINOR
    #define SABUROU_PLATFORM_V2_CUDA_PATCH   __CUDACC_VER_BUILD
    #define SABUROU_PLATFORM_V2_CUDA_VERSION ((SABUROU_PLATFORM_V2_CUDA_MAJOR<<16) | (SABUROU_PLATFORM_V2_CUDA_MINOR<<8) | SABUROU_PLATFORM_V2_CUDA_PATCH)
#endif

// --------------------------------------------

// --- Intel (Classic and IntelLLVM) ---
// /** @brief Intel Classic C/C++ compiler (icc/icpc). */
#if defined(__INTEL_COMPILER) || defined(__ICC)
    #define SABUROU_PLATFORM_V2_INTEL_CLASSIC 1
    #define SABUROU_PLATFORM_V2_MAJOR   (__INTEL_COMPILER/100)
    #define SABUROU_PLATFORM_V2_MINOR   (__INTEL_COMPILER/10 % 10)
    #define SABUROU_PLATFORM_V2_PATCH   (__INTEL_COMPILER % 10)
    #define SABUROU_PLATFORM_V2_VERSION ((SABUROU_PLATFORM_V2_MAJOR<<16) | (SABUROU_PLATFORM_V2_MINOR<<8) | SABUROU_PLATFORM_V2_PATCH)

/** @brief Intel LLVM-based compiler (icx/icpx). */
#elif defined(__INTEL_LLVM_COMPILER)
    #define SABUROU_PLATFORM_V2_INTEL_LLVM 1
    #define SABUROU_PLATFORM_V2_MAJOR   __INTEL_LLVM_COMPILER
    #define SABUROU_PLATFORM_V2_MINOR   0
    #define SABUROU_PLATFORM_V2_PATCH   0
    #define SABUROU_PLATFORM_V2_VERSION ((SABUROU_PLATFORM_V2_MAJOR<<16) | (SABUROU_PLATFORM_V2_MINOR<<8) | SABUROU_PLATFORM_V2_PATCH)

// --- Clang / LLVM ---
#elif defined(__clang__)
    #define SABUROU_PLATFORM_V2_CLANG 1
    #define SABUROU_PLATFORM_V2_MAJOR   __clang_major__
    #define SABUROU_PLATFORM_V2_MINOR   __clang_minor__
    #define SABUROU_PLATFORM_V2_PATCH   __clang_patchlevel__
    #define SABUROU_PLATFORM_V2_VERSION ((SABUROU_PLATFORM_V2_MAJOR<<16) | (SABUROU_PLATFORM_V2_MINOR<<8) | SABUROU_PLATFORM_V2_PATCH)

// --- Embarcadero / Borland ---
#elif defined(__BORLANDC__) || defined(__CODEGEARC__)
    #define SABUROU_PLATFORM_V2_BORLAND 1

// --- GCC (GNU) ---
#elif defined(__GNUC__) || defined(__GNUG__)
    #define SABUROU_PLATFORM_V2_GCC 1
    #define SABUROU_PLATFORM_V2_MAJOR   __GNUC__
    #define SABUROU_PLATFORM_V2_MINOR   __GNUC_MINOR__
    #define SABUROU_PLATFORM_V2_PATCH   __GNUC_PATCHLEVEL__
    #define SABUROU_PLATFORM_V2_VERSION ((SABUROU_PLATFORM_V2_MAJOR<<16) | (SABUROU_PLATFORM_V2_MINOR<<8) | SABUROU_PLATFORM_V2_PATCH)

// --- MSVC (Microsoft) ---
#elif defined(_MSC_VER)
    #define SABUROU_PLATFORM_V2_MSVC 1
    #define SABUROU_PLATFORM_V2_MAJOR   (_MSC_VER/100)
    #define SABUROU_PLATFORM_V2_MINOR   (_MSC_VER%100)
    #define SABUROU_PLATFORM_V2_PATCH   0
    #define SABUROU_PLATFORM_V2_VERSION ((SABUROU_PLATFORM_V2_MAJOR<<16) | (SABUROU_PLATFORM_V2_MINOR<<8) | SABUROU_PLATFORM_V2_PATCH)

#else
    // @note When detection fails, we prioritize accuracy over guesswork; however, due to vast toolchain
    // fragmentation, this remains a best-effort approach.
    #define SABUROU_PLATFORM_V2_COMPILER_UNKNOWN 1
#endif


// =============================================================================
// 2. C++ STDLIB (Implementation)
// -----------------------------------------------------------------------------
// Detects the standard library implementation.
// Useful for SSO detection, format/span support, and container internal layout.
//
// [!] requires <version>
// =============================================================================

// libc++ (Clang ecosystem, Apple, Android NDK optional)
#if defined(_LIBCPP_VERSION)
    #define SABUROU_PLATFORM_V2_STDLIB_LIBCXX 1

// libstdc++ (GCC ecosystem, also used by Clang on Linux)
#elif defined(__GLIBCXX__)
    #define SABUROU_PLATFORM_V2_STDLIB_LIBSTDCXX 1

// MSVC STL (also used by clang-cl)
#elif defined(_MSVC_STL_VERSION)
    #define SABUROU_PLATFORM_V2_STDLIB_MSVC 1

#else
    #define SABUROU_PLATFORM_V2_STDLIB_UNKNOWN 1
#endif


// =============================================================================
// 3. PLATFORM & OS
// -----------------------------------------------------------------------------
// Detects operating system, device type, and environment (desktop, mobile, web).
// =============================================================================
// --- Windows ---
#if defined(_WIN32) || defined(_WIN16) // _WIN32 asume _WIN64
    #define SABUROU_PLATFORM_V2_OS_WINDOWS 1
    #define _SABUROU_PLATFORM_V2_PLATFORM_OS_DEFINED 1

    #define SABUROU_PLATFORM_V2_DEVICE_PC 1
    #define SABUROU_PLATFORM_V2_DEVICE_DESKTOP 1
    #define _SABUROU_PLATFORM_V2_DEVICE_DEFINED 1

    #if defined(_UCRT)
        #define SABUROU_PLATFORM_V2_LIBC_UCRT 1
    #elif defined(_MSC_VER)
        #define SABUROU_PLATFORM_V2_LIBC_MSVC 1
    #elif defined(__MINGW32__) || defined(__MINGW64__)
        #define SABUROU_PLATFORM_V2_LIBC_MINGW 1
    #elif defined(__MSVCRT__)
        #define SABUROU_PLATFORM_V2_LIBC_MSVCRT_DLL 1
    #elif defined(__BORLANDC__) || defined(__CODEGEARC__)
        #define SABUROU_PLATFORM_V2_LIBC_BORLAND 1
    #elif defined(__WATCOMC__)
        #define SABUROU_PLATFORM_V2_LIBC_WATCOM 1
    #else
        #define SABUROU_PLATFORM_V2_LIBC_UNKNOWN 1
    #endif
    #define _SABUROU_PLATFORM_V2_LIBC_DEFINED 1

    #define SABUROU_PLATFORM_V2_POSIX_LIKE 0


// --- Apple ---
#elif (defined(__APPLE__) && defined(__MACH__))
    #include <TargetConditionals.h>

    #define SABUROU_PLATFORM_V2_OS_DARWIN 1

    #define SABUROU_PLATFORM_V2_LIBC_APPLE 1
    #define _SABUROU_PLATFORM_V2_LIBC_DEFINED 1
    
    /* @note The Simulator is prioritized because Apple defines TARGET_OS_IPHONE as true for both physical
     * devices and simulated environments. */
    #if defined(TARGET_OS_SIMULATOR) && TARGET_OS_SIMULATOR
        #define SABUROU_PLATFORM_V2_OS_IOS 1
        #define SABUROU_PLATFORM_V2_OS_IOS_SIMULATOR 1

        #define SABUROU_PLATFORM_V2_DEVICE_MOBILE 1
        #define _SABUROU_PLATFORM_V2_DEVICE_DEFINED 1

    #elif defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
        #define SABUROU_PLATFORM_V2_OS_IOS 1

        #define SABUROU_PLATFORM_V2_DEVICE_MOBILE 1
        #define _SABUROU_PLATFORM_V2_DEVICE_DEFINED 1

    #else
        // TARGET_OS_OSX
        #define SABUROU_PLATFORM_V2_OS_MACOS 1

        #define SABUROU_PLATFORM_V2_DEVICE_MAC 1
        #define SABUROU_PLATFORM_V2_DEVICE_DESKTOP 1
        #define _SABUROU_PLATFORM_V2_DEVICE_DEFINED 1
    #endif
    #define _SABUROU_PLATFORM_V2_PLATFORM_OS_DEFINED 1
    #define SABUROU_PLATFORM_V2_POSIX_LIKE 1


// --- BSD ---
#elif (defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__))
    #define SABUROU_PLATFORM_V2_OS_BSD 1

    #define SABUROU_PLATFORM_V2_DEVICE_PC 1
    #define SABUROU_PLATFORM_V2_DEVICE_DESKTOP 1
    #define _SABUROU_PLATFORM_V2_DEVICE_DEFINED 1

    #define SABUROU_PLATFORM_V2_LIBC_BSD 1
    #define _SABUROU_PLATFORM_V2_LIBC_DEFINED 1

    #if defined(__FreeBSD__)
        #define SABUROU_PLATFORM_V2_OS_FREEBSD 1

    #elif defined(__NetBSD__)
        #define SABUROU_PLATFORM_V2_OS_NETBSD 1

    #elif defined(__OpenBSD__)
        #define SABUROU_PLATFORM_V2_OS_OPENBSD 1

    #elif defined(__DragonFly__)
        #define SABUROU_PLATFORM_V2_OS_DRAGONFLY 1

    #else
        #define SABUROU_PLATFORM_V2_OS_BSD_UNKNOWN 1
    #endif
    #define _SABUROU_PLATFORM_V2_PLATFORM_OS_DEFINED 1

    #define SABUROU_PLATFORM_V2_POSIX_LIKE 1


// --- Android ---
#elif defined(__ANDROID__)
    #define SABUROU_PLATFORM_V2_OS_ANDROID 1
    #define SABUROU_PLATFORM_V2_OS_LINUX 1
    #define _SABUROU_PLATFORM_V2_PLATFORM_OS_DEFINED 1

    #define SABUROU_PLATFORM_V2_DEVICE_MOBILE 1
    #define _SABUROU_PLATFORM_V2_DEVICE_DEFINED 1

    #define SABUROU_PLATFORM_V2_LIBC_BIONIC 1
    #define _SABUROU_PLATFORM_V2_LIBC_DEFINED 1

    #define SABUROU_PLATFORM_V2_POSIX_LIKE 1

// --- WebAssembly / WASM ---
// Emscripten (corre en navegador, hosted)
#elif defined(__EMSCRIPTEN__)
    #define SABUROU_PLATFORM_V2_OS_WASM 1
    #define SABUROU_PLATFORM_V2_OS_WASM_EMSCRIPTEN 1
    #define _SABUROU_PLATFORM_V2_PLATFORM_OS_DEFINED 1

    #define SABUROU_PLATFORM_V2_DEVICE_WEB 1
    #define _SABUROU_PLATFORM_V2_DEVICE_DEFINED 1

    #define SABUROU_PLATFORM_V2_LIBC_MUSL 1
    #define _SABUROU_PLATFORM_V2_LIBC_DEFINED 1

    #define SABUROU_PLATFORM_V2_POSIX_LIKE 1


// WASI standalone (server / freestanding)
#elif defined(__wasm__) && defined(__wasi__)
    #define SABUROU_PLATFORM_V2_OS_WASM 1
    #define SABUROU_PLATFORM_V2_OS_WASM_WASI 1
    #define _SABUROU_PLATFORM_V2_PLATFORM_OS_DEFINED 1

    #define SABUROU_PLATFORM_V2_DEVICE_CLOUD 1
    #define _SABUROU_PLATFORM_V2_DEVICE_DEFINED 1

    #define SABUROU_PLATFORM_V2_LIBC_WASI 1
    #define _SABUROU_PLATFORM_V2_LIBC_DEFINED 1

    #define SABUROU_PLATFORM_V2_POSIX_LIKE 1


// --- Linux (Requiere C++17) ---
#elif defined(__linux__)
    #define SABUROU_PLATFORM_V2_OS_LINUX 1
    #define _SABUROU_PLATFORM_V2_PLATFORM_OS_DEFINED 1

    #define SABUROU_PLATFORM_V2_DEVICE_GENERAL 1
    #define _SABUROU_PLATFORM_V2_DEVICE_DEFINED 1

    #if __has_include(<features.h>)
        // Habilitar macros para detección (fallaría en mac/Windows)
        #include <features.h>
    #endif

    #if defined(__GLIBC__) || defined(__GNU_LIBRARY__)
        #define SABUROU_PLATFORM_V2_LIBC_GLIBC 1

    #elif defined(__LLVM_LIBC__) // nuevo ecosistema clang
        #define SABUROU_PLATFORM_V2_LIBC_LLVM 1

    #elif defined(__KLIBC__) // initramfs / early boot userspace
        #define SABUROU_PLATFORM_V2_LIBC_KLIBC 1

    #elif defined(__NEWLIB__) // normalmente híbridos raros
        #define SABUROU_PLATFORM_V2_LIBC_NEWLIB 1

    #elif defined(__UCLIBC__) // embebidos
        #define SABUROU_PLATFORM_V2_LIBC_UCLIBC 1

    #elif defined(__dietlibc__) // embebidos, raro
        #define SABUROU_PLATFORM_V2_LIBC_DIETLIBC 1

    #elif defined(__BIONIC__) // linux con la biblioteca de android (no android)
        #define SABUROU_PLATFORM_V2_LIBC_BIONIC 1

    #elif defined(__MUSL__) || __has_include(<musl/version.h>)
        #define SABUROU_PLATFORM_V2_LIBC_MUSL 1

    #else
        // #define SABUROU_PLATFORM_V2_LIBC_MUSL 1
        //                                  ^^^
        #define SABUROU_PLATFORM_V2_LIBC_PROBABLY_MUSL 1
        #define SABUROU_PLATFORM_V2_LIBC_UNKNOWN 1
    #endif
    #define _SABUROU_PLATFORM_V2_LIBC_DEFINED 1
    #define SABUROU_PLATFORM_V2_POSIX_LIKE 1

#endif

// [FALLBACKS + POSIX]
// -- OS --
#if !defined(_SABUROU_PLATFORM_V2_PLATFORM_OS_DEFINED)
    #define _SABUROU_PLATFORM_V2_PLATFORM_OS_DEFINED 1
    #define SABUROU_PLATFORM_V2_PLATFORM_OS_UNKNOWN 1
#endif

// -- Device --
#if !defined(_SABUROU_PLATFORM_V2_DEVICE_DEFINED)
    #define SABUROU_PLATFORM_V2_DEVICE_UNKNOWN 1
    #define _SABUROU_PLATFORM_V2_DEVICE_DEFINED 1
#endif

// -- LibC --
#if !defined(_SABUROU_PLATFORM_V2_LIBC_DEFINED)
    #define SABUROU_PLATFORM_V2_LIBC_UNKNOWN 1
    #define _SABUROU_PLATFORM_V2_LIBC_DEFINED 1
#endif

// -- Posix Fallback --
#if !defined(SABUROU_PLATFORM_V2_POSIX_LIKE)
    #if __has_include(<unistd.h>)
        #include <unistd.h>
        #if defined(_POSIX_VERSION)
            #define SABUROU_PLATFORM_V2_POSIX_LIKE 1
        #else
            #define SABUROU_PLATFORM_V2_POSIX_LIKE 0
        #endif
    #else
        // No hay unistd.h -> Asumimos que no es POSIX
        #define SABUROU_PLATFORM_V2_POSIX_LIKE 0
    #endif
#endif


// =============================================================================
// 4. ARCHITECTURE (ISA)
// -----------------------------------------------------------------------------
// Detects the Instruction Set Architecture (x86, ARM, RISC-V, PPC, etc.).
// =============================================================================
#if defined(__x86_64__) || defined(_M_X64)
// --- x86 ---
    #define SABUROU_PLATFORM_V2_ARCH_X86_64 1
    #define SABUROU_PLATFORM_V2_ARCH_X86 1

#elif defined(__i386__) || defined(_M_IX86)
    #define SABUROU_PLATFORM_V2_ARCH_X86_32 1
    #define SABUROU_PLATFORM_V2_ARCH_X86 1

// --- ARM ---
#elif defined(__aarch64__) || defined(_M_ARM64)
    #define SABUROU_PLATFORM_V2_ARCH_ARM_64 1
    #define SABUROU_PLATFORM_V2_ARCH_ARM 1
#elif defined(__arm__) || defined(_M_ARM)
    #define SABUROU_PLATFORM_V2_ARCH_ARM_32 1
    #define SABUROU_PLATFORM_V2_ARCH_ARM 1

// --- RISC-V ---
#elif defined(__riscv) || defined(__riscv__)
    #define SABUROU_PLATFORM_V2_ARCH_RISCV 1
    #if defined(__riscv_xlen) && (__riscv_xlen == 64)
        #define SABUROU_PLATFORM_V2_ARCH_RISCV_64 1
    #else
        #define SABUROU_PLATFORM_V2_ARCH_RISCV_32 1
    #endif

// --- PowerPC ---
#elif defined(__ppc64__) || defined(_ARCH_PPC64)
    #define SABUROU_PLATFORM_V2_ARCH_PPC_64 1
    #define SABUROU_PLATFORM_V2_ARCH_PPC 1
#elif defined(__ppc__) || defined(_ARCH_PPC)
    #define SABUROU_PLATFORM_V2_ARCH_PPC_32 1
    #define SABUROU_PLATFORM_V2_ARCH_PPC 1

#else
    #define SABUROU_PLATFORM_V2_ARCH_UNKNOWN 1
#endif


// =============================================================================
// 5. CACHELINE TUNING
// -----------------------------------------------------------------------------
// Detects cache line size to prevent false sharing.
// =============================================================================
// --- C++20 Attempt (Requires <version> and STDLIB support) --
#if defined(__cpp_lib_hardware_interference_size)
    /** @note about __cpp_lib_hardware_interference_size):
    * - Presence depends on the Standard Library (STDLIB) implementation.
    * - Even if the compiler supports C++20, some STDLIBs may leave this undefined to avoid committing
    *   to a fixed ABI value that might change between different CPU generations. */
    #include <new> // IWYU pragma: keep
    #define SABUROU_PLATFORM_V2_CACHELINE std::hardware_destructive_interference_size

#else
    #define SABUROU_PLATFORM_V2_CACHELINE_UNKNOWN 1
#endif


// =============================================================================
// 6. BITNESS (32/64 bit)
// =============================================================================
#if defined(SABUROU_PLATFORM_V2_ARCH_X86_64) || \
    defined(SABUROU_PLATFORM_V2_ARCH_ARM_64) || \
    defined(SABUROU_PLATFORM_V2_ARCH_RISCV_64) || \
    defined(SABUROU_PLATFORM_V2_ARCH_PPC_64)
    #define SABUROU_PLATFORM_V2_BITNESS_64 1
#elif defined(__LP64__) || defined(_LP64) || defined(_WIN64) || defined(__x86_64__)
    #define SABUROU_PLATFORM_V2_BITNESS_64 1

#elif defined(SABUROU_PLATFORM_V2_ARCH_X86_32) || \
      defined(SABUROU_PLATFORM_V2_ARCH_ARM_32) || \
      defined(SABUROU_PLATFORM_V2_ARCH_PPC_32) || \
      defined(SABUROU_PLATFORM_V2_ARCH_RISCV_32) || \
      defined(_WIN32)
    #define SABUROU_PLATFORM_V2_BITNESS_32 1

#elif defined(__ILP32__) || defined(_ILP32) || defined(__i386__) || defined(_M_IX86)
    #define SABUROU_PLATFORM_V2_BITNESS_32 1

#elif defined(_WIN16)
    #define SABUROU_PLATFORM_V2_BITNESS_16 1

#else
    #define SABUROU_PLATFORM_V2_BITNESS_UNKNOWN 1
#endif


// =============================================================================
// 7. ENDIANNESS
// -----------------------------------------------------------------------------
// Detects byte order (Little vs Big Endian).
// =============================================================================
// ---- Modern compilers (GCC, Clang, ICC, etc.)
#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && defined(__ORDER_BIG_ENDIAN__)
    #if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
        #define SABUROU_PLATFORM_V2_LITTLE_ENDIAN 1
    #elif (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
        #define SABUROU_PLATFORM_V2_BIG_ENDIAN 1
    #endif

// ---- Traditional endian defines (older toolchains, BSD, etc.)
#elif defined(__LITTLE_ENDIAN__) || defined(_LITTLE_ENDIAN) || defined(__ARMEL__) || defined(__THUMBEL__)
    #define SABUROU_PLATFORM_V2_LITTLE_ENDIAN 1

#elif defined(__BIG_ENDIAN__) || defined(_BIG_ENDIAN) || defined(__ARMEB__) || defined(__THUMBEB__)
    #define SABUROU_PLATFORM_V2_BIG_ENDIAN 1

// ---- Architecture heuristics (last-resort but safe in practice)
// x86/x64 are ALWAYS little endian
#elif defined(SABUROU_PLATFORM_V2_ARCH_X86)
    #define SABUROU_PLATFORM_V2_LITTLE_ENDIAN 1

// AArch64 in all mainstream OSes is little endian
#elif defined(SABUROU_PLATFORM_V2_ARCH_ARM_64)
    #define SABUROU_PLATFORM_V2_LITTLE_ENDIAN 1

// RISC-V is little endian in every real-world deployment
#elif defined(SABUROU_PLATFORM_V2_ARCH_RISCV)
    #define SABUROU_PLATFORM_V2_LITTLE_ENDIAN 1

// PowerPC is the only one where big endian is still common
#elif defined(SABUROU_PLATFORM_V2_ARCH_PPC)
    #define SABUROU_PLATFORM_V2_BIG_ENDIAN 1

#else
    #define SABUROU_PLATFORM_V2_ENDIAN_UNKNOWN 1

#endif


// =============================================================================
// 8. ABI (Data Model)
// -----------------------------------------------------------------------------
// Differentiates LP64, LLP64, and ILP32 data models.
// @note Differentiates between LP64 (POSIX) and LLP64 (Windows) to ensure safe pointer arithmetic and
// predictable memory layout.
// =============================================================================
// --- 1. Microsoft Exception ---
#if defined(SABUROU_PLATFORM_V2_OS_WINDOWS) && defined(SABUROU_PLATFORM_V2_BITNESS_64)
    #define SABUROU_PLATFORM_V2_ABI_LLP64 1

// --- 2.A Operating System Truth ---
// Most 64-bit POSIX-compliant systems use the LP64 data model.
#elif defined(SABUROU_PLATFORM_V2_POSIX_LIKE) && defined(SABUROU_PLATFORM_V2_BITNESS_64)
    #define SABUROU_PLATFORM_V2_ABI_LP64 1

// --- 2.B Operating System Truth (Standard Macros) ---
// Direct detection via compiler predefined macros.
#elif defined(__LP64__) || defined(_LP64)
    #define SABUROU_PLATFORM_V2_ABI_LP64 1
    
#elif defined(__ILP32__) || defined(_ILP32)
    #define SABUROU_PLATFORM_V2_ABI_ILP32 1

// --- 3. 32-bit Legacy/Standard (Windows and POSIX converge here) ---
// Both Windows and POSIX typically align on ILP32 for 32-bit architectures.
#elif defined(SABUROU_PLATFORM_V2_BITNESS_32)
    #define SABUROU_PLATFORM_V2_ABI_ILP32 1

// --- 4. Pointer Size Fallback (Final Heuristic) ---
// Final detection layer based on the physical byte size of a data pointer.
// @note Caution: This heuristic depends on compiler macros and may yield false 
// positives if previous platform checks are bypassed or undefined.
#elif defined(__SIZEOF_POINTER__)
    #if (__SIZEOF_POINTER__ == 8)
        #define SABUROU_PLATFORM_V2_ABI_LP64 1 
    #elif (__SIZEOF_POINTER__ == 4)
        #define SABUROU_PLATFORM_V2_ABI_ILP32 1
    #else
        #define SABUROU_PLATFORM_V2_ABI_UNKNOWN 1
    #endif
#else
    #define SABUROU_PLATFORM_V2_ABI_UNKNOWN 1
#endif


// =============================================================================
// 9. ENVIRONMENT (Hosted, Freestanding, Sandboxed, Embedded)
// -----------------------------------------------------------------------------
// Detects the runtime environment characteristics.
// =============================================================================
// --- 1. Web / Browser Sandboxing (Emscripten) ---
#if defined(SABUROU_PLATFORM_V2_OS_WASM_EMSCRIPTEN)
    #define SABUROU_PLATFORM_V2_ENV_HOSTED 1
    #define SABUROU_PLATFORM_V2_ENV_SANDBOXED 1

// --- 2. Server-side WASM / Cloud (WASI) ---
#elif defined(SABUROU_PLATFORM_V2_OS_WASM_WASI)
    #define SABUROU_PLATFORM_V2_ENV_EMBEDDED 1
    // Nota: WASI suele ser hosted-like pero con capabilities limitadas.
    // Se puede definir HOSTED aquí si se asume acceso a archivos/red.

// --- 3. Standard C Detection (Hosted vs Freestanding) ---
#elif defined(__STDC_HOSTED__)
    #if (__STDC_HOSTED__ == 1)
        /** @brief Full OS services available. */
        #define SABUROU_PLATFORM_V2_ENV_HOSTED 1
    #else
        /** @brief Bare-metal, kernel, or restricted environment. */
        #define SABUROU_PLATFORM_V2_ENV_FREESTANDING 1
        #define SABUROU_PLATFORM_V2_ENV_EMBEDDED 1
    #endif

// --- 4. OS Heuristics ---
#elif defined(SABUROU_PLATFORM_V2_OS_WINDOWS) || \
      defined(SABUROU_PLATFORM_V2_OS_LINUX)   || \
      defined(SABUROU_PLATFORM_V2_OS_DARWIN)
    #define SABUROU_PLATFORM_V2_ENV_HOSTED 1

#else
    /** @brief Fallback for unidentified environments. */
    #define SABUROU_PLATFORM_V2_ENV_UNKNOWN 1
#endif


// =============================================================================
// 10. SANITIZATION & FALLBACKS
// -----------------------------------------------------------------------------
// Ensures all SABUROU_PLATFORM macros are defined.
// This section guarantees that every detection macro exists and is assigned 
// a boolean value (0), preventing 'undefined macro' errors in downstream 
// code and ensuring compatibility with strict compiler flags like -Wundef.
// =============================================================================
#ifdef _SABUROU_PLATFORM_V2_PLATFORM_OS_DEFINED
    #undef _SABUROU_PLATFORM_V2_PLATFORM_OS_DEFINED
#endif
#ifdef _SABUROU_PLATFORM_V2_DEVICE_DEFINED
    #undef _SABUROU_PLATFORM_V2_DEVICE_DEFINED
#endif
#ifdef _SABUROU_PLATFORM_V2_LIBC_DEFINED
    #undef _SABUROU_PLATFORM_V2_LIBC_DEFINED
#endif


#ifndef SABUROU_PLATFORM_V2_NVCC
    #define SABUROU_PLATFORM_V2_NVCC 0
#endif
#ifndef SABUROU_PLATFORM_V2_CUDA_MAJOR
    #define SABUROU_PLATFORM_V2_CUDA_MAJOR 0
#endif
#ifndef SABUROU_PLATFORM_V2_CUDA_MINOR
    #define SABUROU_PLATFORM_V2_CUDA_MINOR 0
#endif
#ifndef SABUROU_PLATFORM_V2_CUDA_PATCH
    #define SABUROU_PLATFORM_V2_CUDA_PATCH 0
#endif
#ifndef SABUROU_PLATFORM_V2_CUDA_VERSION
    #define SABUROU_PLATFORM_V2_CUDA_VERSION 0
#endif
#ifndef SABUROU_PLATFORM_V2_INTEL_CLASSIC
    #define SABUROU_PLATFORM_V2_INTEL_CLASSIC 0
#endif
#ifndef SABUROU_PLATFORM_V2_MAJOR
    #define SABUROU_PLATFORM_V2_MAJOR 0
#endif
#ifndef SABUROU_PLATFORM_V2_MINOR
    #define SABUROU_PLATFORM_V2_MINOR 0
#endif
#ifndef SABUROU_PLATFORM_V2_PATCH
    #define SABUROU_PLATFORM_V2_PATCH 0
#endif
#ifndef SABUROU_PLATFORM_V2_VERSION
    #define SABUROU_PLATFORM_V2_VERSION 0
#endif
#ifndef SABUROU_PLATFORM_V2_INTEL_LLVM
    #define SABUROU_PLATFORM_V2_INTEL_LLVM 0
#endif
#ifndef SABUROU_PLATFORM_V2_CLANG
    #define SABUROU_PLATFORM_V2_CLANG 0
#endif
#ifndef SABUROU_PLATFORM_V2_BORLAND
    #define SABUROU_PLATFORM_V2_BORLAND 0
#endif
#ifndef SABUROU_PLATFORM_V2_GCC
    #define SABUROU_PLATFORM_V2_GCC 0
#endif
#ifndef SABUROU_PLATFORM_V2_MSVC
    #define SABUROU_PLATFORM_V2_MSVC 0
#endif
#ifndef SABUROU_PLATFORM_V2_COMPILER_UNKNOWN
    #define SABUROU_PLATFORM_V2_COMPILER_UNKNOWN 0
#endif
#ifndef SABUROU_PLATFORM_V2_STDLIB_LIBCXX
    #define SABUROU_PLATFORM_V2_STDLIB_LIBCXX 0
#endif
#ifndef SABUROU_PLATFORM_V2_STDLIB_LIBSTDCXX
    #define SABUROU_PLATFORM_V2_STDLIB_LIBSTDCXX 0
#endif
#ifndef SABUROU_PLATFORM_V2_STDLIB_MSVC
    #define SABUROU_PLATFORM_V2_STDLIB_MSVC 0
#endif
#ifndef SABUROU_PLATFORM_V2_STDLIB_UNKNOWN
    #define SABUROU_PLATFORM_V2_STDLIB_UNKNOWN 0
#endif
#ifndef SABUROU_PLATFORM_V2_OS_WINDOWS
    #define SABUROU_PLATFORM_V2_OS_WINDOWS 0
#endif
#ifndef SABUROU_PLATFORM_V2_PLATFORM_OS_DEFINED
    #define SABUROU_PLATFORM_V2_PLATFORM_OS_DEFINED 0
#endif
#ifndef SABUROU_PLATFORM_V2_DEVICE_PC
    #define SABUROU_PLATFORM_V2_DEVICE_PC 0
#endif
#ifndef SABUROU_PLATFORM_V2_DEVICE_DESKTOP
    #define SABUROU_PLATFORM_V2_DEVICE_DESKTOP 0
#endif
#ifndef SABUROU_PLATFORM_V2_DEVICE_DEFINED
    #define SABUROU_PLATFORM_V2_DEVICE_DEFINED 0
#endif
#ifndef SABUROU_PLATFORM_V2_LIBC_UCRT
    #define SABUROU_PLATFORM_V2_LIBC_UCRT 0
#endif
#ifndef SABUROU_PLATFORM_V2_LIBC_MSVC
    #define SABUROU_PLATFORM_V2_LIBC_MSVC 0
#endif
#ifndef SABUROU_PLATFORM_V2_LIBC_MINGW
    #define SABUROU_PLATFORM_V2_LIBC_MINGW 0
#endif
#ifndef SABUROU_PLATFORM_V2_LIBC_MSVCRT_DLL
    #define SABUROU_PLATFORM_V2_LIBC_MSVCRT_DLL 0
#endif
#ifndef SABUROU_PLATFORM_V2_LIBC_BORLAND
    #define SABUROU_PLATFORM_V2_LIBC_BORLAND 0
#endif
#ifndef SABUROU_PLATFORM_V2_LIBC_WATCOM
    #define SABUROU_PLATFORM_V2_LIBC_WATCOM 0
#endif
#ifndef SABUROU_PLATFORM_V2_LIBC_UNKNOWN
    #define SABUROU_PLATFORM_V2_LIBC_UNKNOWN 0
#endif
#ifndef SABUROU_PLATFORM_V2_LIBC_DEFINED
    #define SABUROU_PLATFORM_V2_LIBC_DEFINED 0
#endif
#ifndef SABUROU_PLATFORM_V2_POSIX_LIKE
    #define SABUROU_PLATFORM_V2_POSIX_LIKE 0
#endif
#ifndef SABUROU_PLATFORM_V2_OS_DARWIN
    #define SABUROU_PLATFORM_V2_OS_DARWIN 0
#endif
#ifndef SABUROU_PLATFORM_V2_LIBC_APPLE
    #define SABUROU_PLATFORM_V2_LIBC_APPLE 0
#endif
#ifndef SABUROU_PLATFORM_V2_OS_IOS
    #define SABUROU_PLATFORM_V2_OS_IOS 0
#endif
#ifndef SABUROU_PLATFORM_V2_OS_IOS_SIMULATOR
    #define SABUROU_PLATFORM_V2_OS_IOS_SIMULATOR 0
#endif
#ifndef SABUROU_PLATFORM_V2_DEVICE_MOBILE
    #define SABUROU_PLATFORM_V2_DEVICE_MOBILE 0
#endif
#ifndef SABUROU_PLATFORM_V2_OS_MACOS
    #define SABUROU_PLATFORM_V2_OS_MACOS 0
#endif
#ifndef SABUROU_PLATFORM_V2_DEVICE_MAC
    #define SABUROU_PLATFORM_V2_DEVICE_MAC 0
#endif
#ifndef SABUROU_PLATFORM_V2_OS_BSD
    #define SABUROU_PLATFORM_V2_OS_BSD 0
#endif
#ifndef SABUROU_PLATFORM_V2_LIBC_BSD
    #define SABUROU_PLATFORM_V2_LIBC_BSD 0
#endif
#ifndef SABUROU_PLATFORM_V2_OS_FREEBSD
    #define SABUROU_PLATFORM_V2_OS_FREEBSD 0
#endif
#ifndef SABUROU_PLATFORM_V2_OS_NETBSD
    #define SABUROU_PLATFORM_V2_OS_NETBSD 0
#endif
#ifndef SABUROU_PLATFORM_V2_OS_OPENBSD
    #define SABUROU_PLATFORM_V2_OS_OPENBSD 0
#endif
#ifndef SABUROU_PLATFORM_V2_OS_DRAGONFLY
    #define SABUROU_PLATFORM_V2_OS_DRAGONFLY 0
#endif
#ifndef SABUROU_PLATFORM_V2_OS_BSD_UNKNOWN
    #define SABUROU_PLATFORM_V2_OS_BSD_UNKNOWN 0
#endif
#ifndef SABUROU_PLATFORM_V2_OS_ANDROID
    #define SABUROU_PLATFORM_V2_OS_ANDROID 0
#endif
#ifndef SABUROU_PLATFORM_V2_OS_LINUX
    #define SABUROU_PLATFORM_V2_OS_LINUX 0
#endif
#ifndef SABUROU_PLATFORM_V2_LIBC_BIONIC
    #define SABUROU_PLATFORM_V2_LIBC_BIONIC 0
#endif
#ifndef SABUROU_PLATFORM_V2_OS_WASM
    #define SABUROU_PLATFORM_V2_OS_WASM 0
#endif
#ifndef SABUROU_PLATFORM_V2_OS_WASM_EMSCRIPTEN
    #define SABUROU_PLATFORM_V2_OS_WASM_EMSCRIPTEN 0
#endif
#ifndef SABUROU_PLATFORM_V2_DEVICE_WEB
    #define SABUROU_PLATFORM_V2_DEVICE_WEB 0
#endif
#ifndef SABUROU_PLATFORM_V2_LIBC_MUSL
    #define SABUROU_PLATFORM_V2_LIBC_MUSL 0
#endif
#ifndef SABUROU_PLATFORM_V2_OS_WASM_WASI
    #define SABUROU_PLATFORM_V2_OS_WASM_WASI 0
#endif
#ifndef SABUROU_PLATFORM_V2_DEVICE_CLOUD
    #define SABUROU_PLATFORM_V2_DEVICE_CLOUD 0
#endif
#ifndef SABUROU_PLATFORM_V2_LIBC_WASI
    #define SABUROU_PLATFORM_V2_LIBC_WASI 0
#endif
#ifndef SABUROU_PLATFORM_V2_DEVICE_GENERAL
    #define SABUROU_PLATFORM_V2_DEVICE_GENERAL 0
#endif
#ifndef SABUROU_PLATFORM_V2_LIBC_GLIBC
    #define SABUROU_PLATFORM_V2_LIBC_GLIBC 0
#endif
#ifndef SABUROU_PLATFORM_V2_LIBC_LLVM
    #define SABUROU_PLATFORM_V2_LIBC_LLVM 0
#endif
#ifndef SABUROU_PLATFORM_V2_LIBC_KLIBC
    #define SABUROU_PLATFORM_V2_LIBC_KLIBC 0
#endif
#ifndef SABUROU_PLATFORM_V2_LIBC_NEWLIB
    #define SABUROU_PLATFORM_V2_LIBC_NEWLIB 0
#endif
#ifndef SABUROU_PLATFORM_V2_LIBC_UCLIBC
    #define SABUROU_PLATFORM_V2_LIBC_UCLIBC 0
#endif
#ifndef SABUROU_PLATFORM_V2_LIBC_DIETLIBC
    #define SABUROU_PLATFORM_V2_LIBC_DIETLIBC 0
#endif
#ifndef SABUROU_PLATFORM_V2_LIBC_PROBABLY_MUSL
    #define SABUROU_PLATFORM_V2_LIBC_PROBABLY_MUSL 0
#endif
#ifndef SABUROU_PLATFORM_V2_PLATFORM_OS_UNKNOWN
    #define SABUROU_PLATFORM_V2_PLATFORM_OS_UNKNOWN 0
#endif
#ifndef SABUROU_PLATFORM_V2_DEVICE_UNKNOWN
    #define SABUROU_PLATFORM_V2_DEVICE_UNKNOWN 0
#endif
#ifndef SABUROU_PLATFORM_V2_ARCH_X86_64
    #define SABUROU_PLATFORM_V2_ARCH_X86_64 0
#endif
#ifndef SABUROU_PLATFORM_V2_ARCH_X86
    #define SABUROU_PLATFORM_V2_ARCH_X86 0
#endif
#ifndef SABUROU_PLATFORM_V2_ARCH_X86_32
    #define SABUROU_PLATFORM_V2_ARCH_X86_32 0
#endif
#ifndef SABUROU_PLATFORM_V2_ARCH_ARM_64
    #define SABUROU_PLATFORM_V2_ARCH_ARM_64 0
#endif
#ifndef SABUROU_PLATFORM_V2_ARCH_ARM
    #define SABUROU_PLATFORM_V2_ARCH_ARM 0
#endif
#ifndef SABUROU_PLATFORM_V2_ARCH_ARM_32
    #define SABUROU_PLATFORM_V2_ARCH_ARM_32 0
#endif
#ifndef SABUROU_PLATFORM_V2_ARCH_RISCV
    #define SABUROU_PLATFORM_V2_ARCH_RISCV 0
#endif
#ifndef SABUROU_PLATFORM_V2_ARCH_RISCV_64
    #define SABUROU_PLATFORM_V2_ARCH_RISCV_64 0
#endif
#ifndef SABUROU_PLATFORM_V2_ARCH_RISCV_32
    #define SABUROU_PLATFORM_V2_ARCH_RISCV_32 0
#endif
#ifndef SABUROU_PLATFORM_V2_ARCH_PPC_64
    #define SABUROU_PLATFORM_V2_ARCH_PPC_64 0
#endif
#ifndef SABUROU_PLATFORM_V2_ARCH_PPC
    #define SABUROU_PLATFORM_V2_ARCH_PPC 0
#endif
#ifndef SABUROU_PLATFORM_V2_ARCH_PPC_32
    #define SABUROU_PLATFORM_V2_ARCH_PPC_32 0
#endif
#ifndef SABUROU_PLATFORM_V2_ARCH_UNKNOWN
    #define SABUROU_PLATFORM_V2_ARCH_UNKNOWN 0
#endif
#ifndef SABUROU_PLATFORM_V2_CACHELINE
    #define SABUROU_PLATFORM_V2_CACHELINE 0
#endif
#ifndef SABUROU_PLATFORM_V2_CACHELINE_UNKNOWN
    #define SABUROU_PLATFORM_V2_CACHELINE_UNKNOWN 0
#endif
#ifndef SABUROU_PLATFORM_V2_BITNESS_64
    #define SABUROU_PLATFORM_V2_BITNESS_64 0
#endif
#ifndef SABUROU_PLATFORM_V2_BITNESS_32
    #define SABUROU_PLATFORM_V2_BITNESS_32 0
#endif
#ifndef SABUROU_PLATFORM_V2_BITNESS_16
    #define SABUROU_PLATFORM_V2_BITNESS_16 0
#endif
#ifndef SABUROU_PLATFORM_V2_BITNESS_UNKNOWN
    #define SABUROU_PLATFORM_V2_BITNESS_UNKNOWN 0
#endif
#ifndef SABUROU_PLATFORM_V2_LITTLE_ENDIAN
    #define SABUROU_PLATFORM_V2_LITTLE_ENDIAN 0
#endif
#ifndef SABUROU_PLATFORM_V2_BIG_ENDIAN
    #define SABUROU_PLATFORM_V2_BIG_ENDIAN 0
#endif
#ifndef SABUROU_PLATFORM_V2_ENDIAN_UNKNOWN
    #define SABUROU_PLATFORM_V2_ENDIAN_UNKNOWN 0
#endif
#ifndef SABUROU_PLATFORM_V2_ABI_LLP64
    #define SABUROU_PLATFORM_V2_ABI_LLP64 0
#endif
#ifndef SABUROU_PLATFORM_V2_ABI_LP64
    #define SABUROU_PLATFORM_V2_ABI_LP64 0
#endif
#ifndef SABUROU_PLATFORM_V2_ABI_ILP32
    #define SABUROU_PLATFORM_V2_ABI_ILP32 0
#endif
#ifndef SABUROU_PLATFORM_V2_ABI_UNKNOWN
    #define SABUROU_PLATFORM_V2_ABI_UNKNOWN 0
#endif
#ifndef SABUROU_PLATFORM_V2_ENV_HOSTED
    #define SABUROU_PLATFORM_V2_ENV_HOSTED 0
#endif
#ifndef SABUROU_PLATFORM_V2_ENV_SANDBOXED
    #define SABUROU_PLATFORM_V2_ENV_SANDBOXED 0
#endif
#ifndef SABUROU_PLATFORM_V2_ENV_EMBEDDED
    #define SABUROU_PLATFORM_V2_ENV_EMBEDDED 0
#endif
#ifndef SABUROU_PLATFORM_V2_ENV_FREESTANDING
    #define SABUROU_PLATFORM_V2_ENV_FREESTANDING 0
#endif
#ifndef SABUROU_PLATFORM_V2_ENV_UNKNOWN
    #define SABUROU_PLATFORM_V2_ENV_UNKNOWN 0
#endif

// =============================================================================
// 11. COMPILE-TIME VALIDATION
// -----------------------------------------------------------------------------
// Safety checks to prevent impossible environment states.
// =============================================================================
// TODO
