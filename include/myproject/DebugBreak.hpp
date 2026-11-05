#pragma once

#include <cstdlib>

#if defined(_MSC_VER)

#include <intrin.h>

#define MYPROJECT_DEBUG_BREAK() __debugbreak()

#elif defined(__clang__) || defined(__GNUC__)

#define MYPROJECT_DEBUG_BREAK() __builtin_trap()

#else

// Fallback to just terminate the program
#define MYPROJECT_DEBUG_BREAK() std::abort()

#endif
