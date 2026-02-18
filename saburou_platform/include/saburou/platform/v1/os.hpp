/**
 * @file os.hpp
 * @brief Main umbrella header for operating system abstraction layers.
 *
 * This header provides access to OS family, type, and runtime info queries.
 * Note: Platform-specific headers (like linux.hpp) are excluded to maintain 
 * a generic interface and must be included explicitly if needed.
 */

#pragma once

#include <saburou/platform/v1/os/family.hpp> // IWYU pragma: export
#include <saburou/platform/v1/os/info.hpp>   // IWYU pragma: export
#include <saburou/platform/v1/os/type.hpp>   // IWYU pragma: export
