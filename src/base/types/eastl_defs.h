#pragma once
#include <EASTL/allocator.h>
#include "EASTL/string.h"

namespace eastl {
    class allocator;
    inline bool operator==(allocator const&, allocator const&) { return true; }
    inline bool operator!=(allocator const&, allocator const&) { return false; }

    inline int32_t VPrintf(char* pBuffer, uint32_t bufferSize, char const* pMessageFormat, va_list args) {
        return vsnprintf(pBuffer, size_t(bufferSize), pMessageFormat, args);
    }

    inline int32_t Printf(char* pBuffer, uint32_t bufferSize, char const* pMessageFormat, ...) {
        va_list args;
        va_start(args, pMessageFormat);
        int32_t const numChars = VPrintf(pBuffer, size_t(bufferSize), pMessageFormat, args);
        va_end(args);
        return numChars;
    }
}
namespace EA::StdC {
    inline int Vsnprintf(char16_t* EA_RESTRICT p, size_t n, const char16_t* EA_RESTRICT format, va_list args) {

        if (!p) {
            return _vscwprintf((const wchar_t*)format, args);
        }
        else {
            return _vsnwprintf_s((wchar_t*)p, n, _TRUNCATE, (const wchar_t*)format, args);
        }
    }

    inline int Vsnprintf(char* EA_RESTRICT p, size_t n, char const* EA_RESTRICT format, char* args) {

        // TODO: probably use SDL_snprintf();

        if (!p) {
            return _vscprintf(format, args);
        }
        else {
            return vsnprintf_s(p, n, _TRUNCATE, format, args);
        }


    }
}
