#pragma once

#include <saburou/macros/pragma.hpp> // IWYU pragma: keep

// Input de ejemplo
// #define SABUROU_PLATFORM_MIN_CPP_VER 201703L
// #define SABUROU_PLATFORM_MIN_CPP_NAME "C++17"

#ifndef SABUROU_PLATFORM_MIN_CPP_VER
#error "[!] Saburou: Debes definir SABUROU_PLATFORM_MIN_CPP_VER antes de invocar a cppxx_guard.hpp"
#endif
#ifndef SABUROU_PLATFORM_MIN_CPP_NAME
#error "[!] Saburou: Debes definir SABUROU_PLATFORM_MIN_CPP_NAME antes de invocar a cppxx_guard.hpp"
#endif

#if __cplusplus < SABUROU_PLATFORM_MIN_CPP_VER && !defined(_MSVC_LANG)
SABUROU_MSG("[!] SABUROU: Requires " SABUROU_PLATFORM_MIN_CPP_NAME)
#error "[!] Saburou: Invalid cpp version"
#elif defined(_MSVC_LANG) && _MSVC_LANG < SABUROU_PLATFORM_MIN_CPP_VER
SABUROU_MSG("[!] SABUROU: Requires " SABUROU_PLATFORM_MIN_CPP_NAME " (MSVC)")
#error "[!] Saburou: Invalid cpp version"
#endif
