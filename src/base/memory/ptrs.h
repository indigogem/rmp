#pragma once

#include <EASTL/shared_ptr.h>
#include <EASTL/unique_ptr.h>

//-------------------------------------------------------------------------

namespace kmp
{
    namespace memory
    {
        template <typename T>
        struct DefaultDeleter
        {
            EA_CONSTEXPR DefaultDeleter() EA_NOEXCEPT = default;

            void operator()(T *p) const EA_NOEXCEPT
            {
                kmp::Delete(p);
            }
        };

        template <typename T>
        struct DefaultDeleter<T[]> // Specialization for arrays.
        {
            EA_CONSTEXPR DefaultDeleter() EA_NOEXCEPT = default;

            void operator()(T *p) const EA_NOEXCEPT
            {
                kmp::DeleteArray(p);
            }
        };
    }

    //-------------------------------------------------------------------------

    template <typename T>
    using SharedPtr = eastl::shared_ptr<T>;
    template <typename T, typename D = memory::DefaultDeleter<T>>
    using UniquePtr = eastl::unique_ptr<T, D>;
}