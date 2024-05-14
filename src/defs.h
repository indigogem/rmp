#pragma once

#define KMP_VERSION "0.0.1"
#define KMP_DEVELOPMENT 1

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#if KMP_DEVELOPMENT
#define _CRTDBG_MAP_ALLOC
#endif

#include <stdlib.h>
#include <cstdint>
#include <crtdbg.h>

#define InvalidIndex -1

#define KMP_STATIC_ASSERT(cond, error) static_assert(cond, error)

#if KMP_DEVELOPMENT
#define KMP_DEBUG_BREAK() __debugbreak()
#define KMP_ASSERT(cond) _ASSERTE(cond)
#define KMP_TRACE_MSG(msgFormat, ...) log::TraceMessage(msgFormat, ##__VA_ARGS__)

#define KMP_HALT()
#define KMP_TRACE_HALT(msgFormat, ...)
#define KMP_UNIMPLEMENTED_FUNCTION()
#define KMP_UNREACHABLE_CODE()

#define KMP_BASE_API
#define KMP_SERIALIZE(...)

// #if EE_DLL
// #ifdef ESOTERICA_ENGINE_RUNTIME
// #define KMP_ENGINE_API __declspec(dllexport)
// #else
// #define KMP_ENGINE_API __declspec(dllimport)
// #endif
// #else
#define KMP_ENGINE_API

#else
#define KMP_DEBUG_BREAK()
#define KMP_ASSERT(cond)    \
    do                      \
    {                       \
        (void)sizeof(cond); \
    } while (0)
#define KMP_TRACE_MSG(msgFormat, ...)

#define KMP_HALT()
#define KMP_TRACE_HALT(msgFormat, ...)
#define KMP_UNIMPLEMENTED_FUNCTION()
#define KMP_UNREACHABLE_CODE()

#define KMP_BASE_API
#define KMP_SERIALIZE(...)

#define KMP_ENGINE_API

#endif

#define KMP_FORCE_INLINE __forceinline

namespace kmp
{

    inline constexpr uint16_t kVirtrualWindowsWidth = 1280;
    inline constexpr uint16_t kVirtrualWindowsHeight = 720;

} // kmp