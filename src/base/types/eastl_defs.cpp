#include "base/types/eastl_defs.h"
#include "base/memory/memory.h"
// #include <EASTL/internal/config.h>
// #include <EASTL/allocator.h>

// void* __cdecl operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
//{
//     return new uint8_t[size];
// }
//
// void* __cdecl operator new[](unsigned __int64 size, unsigned __int64 alignment, unsigned __int64 offset, char const* pName, int flags, unsigned int debugFlags, char const* file, int line)
//{
//
//     return new uint8_t[size];
// }

namespace eastl
{

    allocator g_defaultAllocator;

    //-------------------------------------------------------------------------

    allocator *GetDefaultAllocator()
    {
        return &g_defaultAllocator;
    }

    allocator *SetDefaultAllocator(allocator *pAllocator)
    {
        return &g_defaultAllocator;
    }

    //-------------------------------------------------------------------------

    allocator::allocator(const char *EASTL_NAME(pName))
    {
    }

    allocator::allocator(const allocator &EASTL_NAME(alloc))
    {
    }

    allocator::allocator(const allocator &, const char *EASTL_NAME(pName))
    {
    }

    allocator &allocator::operator=(const allocator &EASTL_NAME(alloc))
    {
        return *this;
    }

    const char *allocator::get_name() const
    {
        return EASTL_ALLOCATOR_DEFAULT_NAME;
    }

    void allocator::set_name(const char *EASTL_NAME(pName))
    {
    }

    void *allocator::allocate(size_t n, int flags)
    {
        return kmp::Alloc(n, EASTL_ALLOCATOR_MIN_ALIGNMENT);
    }

    void *allocator::allocate(size_t n, size_t alignment, size_t offset, int flags)
    {
        return kmp::Alloc(n, alignment);
    }

    void allocator::deallocate(void *p, size_t)
    {
        kmp::Free(p);
    }
}