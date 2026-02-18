/**
 * @file core_guard.hpp
 * @brief Integrity guard for saburou-platform core revisions.
 *
 * This header ensures that all translation units in a single compilation session 
 * use the same internal core revision. It prevents ODR (One Definition Rule) 
 * violations and inconsistent behavior that could arise from mixing different 
 * internal implementations of the same API version.
 */

#pragma once

#include <saburou/macros/pragma.hpp>

// Example Input
// #define SABUROU_PLATFORM_CORE_REVISION_LOCAL 1

#ifndef SABUROU_PLATFORM_CORE_REVISION_LOCAL
    #error "[!] Saburou: CORE_REVISION_LOCAL must be defined before invoking core_guard.hpp"
#endif

#ifdef SABUROU_PLATFORM_CORE_REVISION
    #if SABUROU_PLATFORM_CORE_REVISION != SABUROU_PLATFORM_CORE_REVISION_LOCAL
        SABUROU_MSG(
            "[!] ERROR: Core revision inconsistency detected in SABUROU_PLATFORM.\n" \
            "- Revision in use: " SABUROU_STRINGIFY(SABUROU_PLATFORM_CORE_REVISION) "\n" \
            "- Revision required: " SABUROU_STRINGIFY(SABUROU_PLATFORM_CORE_REVISION_LOCAL)
        )
        #error "[!] SABUROU_PLATFORM: Core revision mismatch detected in global namespace."
    #endif
#endif

#undef SABUROU_PLATFORM_CORE_REVISION_LOCAL
