/**
 * @file recover.hpp
 * @brief Environment recovery for legacy platform macros.
 *
 * This file restores the legacy platform macros (linux, unix, apple, bsd) if they 
 * were previously defined and subsequently cleaned up by saburou-platform. 
 * Use this only if external legacy code requires these specific global macros to compile.
 */

#pragma once

#ifdef SABUROU_PLATFORM_LEGACY_LINUX
    #ifndef linux
        #define linux 1
    #endif
#endif

#ifdef SABUROU_PLATFORM_LEGACY_UNIX
    #ifndef unix
        #define unix 1
    #endif
#endif

#ifdef SABUROU_PLATFORM_LEGACY_APPLE
    #ifndef apple
        #define apple 1
    #endif
#endif

#ifdef SABUROU_PLATFORM_LEGACY_BSD
    #ifndef bsd
        #define bsd 1
    #endif
#endif
