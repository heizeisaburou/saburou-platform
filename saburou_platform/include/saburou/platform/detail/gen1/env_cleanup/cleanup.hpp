/**
 * @file cleanup.hpp
 * @brief Environment cleanup for legacy platform macros.
 *
 * This file detects and undefines common legacy platform macros (e.g., linux, unix, apple, bsd) 
 * that are often pre-defined by compilers in the global namespace. 
 * Undefining these allows saburou-platform to use these identifiers in modern C++ structures 
 * like enums or namespaces without syntax errors.
 */

#pragma once

#ifdef linux
    #define SABUROU_PLATFORM_LEGACY_LINUX 1
    #undef linux
#endif

#ifdef unix
    #define SABUROU_PLATFORM_LEGACY_UNIX 1
    #undef unix
#endif

#ifdef apple
    #define SABUROU_PLATFORM_LEGACY_APPLE 1
    #undef apple
#endif

#ifdef bsd
    #define SABUROU_PLATFORM_LEGACY_BSD 1
    #undef bsd
#endif
