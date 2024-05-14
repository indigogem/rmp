#pragma once
#include "defs.h"
#include <cstring>
#include <malloc.h>
#include <utility>

#define KMP_USE_CUSTOM_ALLOCATOR 1

#ifdef _WIN32

#define KMP_STACK_ALLOC(x) alloca(x)
#define KMP_STACK_ARRAY_ALLOC(type, numElements) reinterpret_cast<type *>(alloca(sizeof(type) * numElements));

#else

#define KMP_STACK_ALLOC(x)
#define KMP_STACK_ARRAY_ALLOC(type, numElements)

#endif

namespace kmp
{
    namespace memory
    {
        constexpr size_t kDefaultAlignment = 8;

        void Initialize();
        void Shutdown();

        KMP_BASE_API size_t GetTotalRequestedMemory();
        KMP_BASE_API size_t GetTotalAllocatedMemory();

        KMP_BASE_API void InitializeThreadHeap();
        KMP_BASE_API void ShutdownThreadHeap();

        inline bool IsAligned(void const *p, size_t n)
        {
            return (reinterpret_cast<uintptr_t>(p) % n) == 0;
        }

        template <typename T>
        inline bool IsAligned(T const *p)
        {
            return (reinterpret_cast<uintptr_t>(p) % alignof(T)) == 0;
        }

        KMP_FORCE_INLINE void MemsetZero(void *ptr, size_t size)
        {
            memset(ptr, 0, size);
        }

        template <typename T>
        KMP_FORCE_INLINE void MemsetZero(T *ptr)
        {
            memset(ptr, 0, sizeof(T));
        }

        KMP_FORCE_INLINE size_t CalculatePaddingForAlignment(uintptr_t address_offset, size_t required_alignment)
        {
            return (required_alignment - (address_offset % required_alignment)) % required_alignment;
        }

        KMP_FORCE_INLINE size_t CalculatePaddingForAlignment(void *address, size_t required_alignment)
        {
            return CalculatePaddingForAlignment(reinterpret_cast<uintptr_t>(address), required_alignment);
        }
    }

    //-------------------------------------------------------------------------
    // Global Memory Management Functions
    //-------------------------------------------------------------------------

    [[nodiscard]] void *Alloc(size_t size, size_t alignment = memory::kDefaultAlignment);
    [[nodiscard]] void *Realloc(void *memory, size_t new_size, size_t original_alignment);
    void Free(void *&memory);

    template <typename T, typename... ConstructorParams>
    [[nodiscard]] KMP_FORCE_INLINE T *New(ConstructorParams &&...params)
    {
        void *pMemory = Alloc(sizeof(T), alignof(T));
        KMP_ASSERT(pMemory != nullptr);
        return new (pMemory) T(std::forward<ConstructorParams>(params)...);
    }

    template <typename T>
    KMP_FORCE_INLINE void Delete(T *&type)
    {
        if (type != nullptr)
        {
            type->~T();
            Free((void *&)type);
        }
    }

    template <typename T>
    KMP_FORCE_INLINE void Free(T *&tpe)
    {
        Free((void *&)tpe);
    }

    //-------------------------------------------------------------------------

    template <typename T, typename... ConstructorParams>
    [[nodiscard]] KMP_FORCE_INLINE T *NewArray(size_t const num_elements, ConstructorParams &&...params)
    {
        size_t const requiredAlignment = std::max(alignof(T), size_t(16));
        size_t const requiredExtraMemory = std::max(requiredAlignment, size_t(4));
        size_t const requiredMemory = sizeof(T) * num_elements + requiredExtraMemory;

        uint8_t *pOriginalAddress = pOriginalAddress = (uint8_t *)Alloc(requiredMemory, requiredAlignment);
        KMP_ASSERT(pOriginalAddress != nullptr);

        // Call required type constructors
        T *array_address = reinterpret_cast<T *>(pOriginalAddress + requiredExtraMemory);
        for (size_t i = 0; i < num_elements; i++)
        {
            new (&array_address[i]) T(std::forward<ConstructorParams>(params)...);
        }

        // Record the number of array elements
        uint32_t *pNumElements = reinterpret_cast<uint32_t *>(array_address) - 1;
        *pNumElements = uint32_t(num_elements);

        return reinterpret_cast<T *>(array_address);
    }

    template <typename T>
    [[nodiscard]] KMP_FORCE_INLINE T *NewArray(size_t const num_elements, T const &value)
    {
        size_t const requiredAlignment = std::max(alignof(T), size_t(16));
        size_t const requiredExtraMemory = std::max(requiredAlignment, size_t(4));
        size_t const requiredMemory = sizeof(T) * num_elements + requiredExtraMemory;

        uint8_t *pOriginalAddress = pOriginalAddress = (uint8_t *)Alloc(requiredMemory, requiredAlignment);
        KMP_ASSERT(pOriginalAddress != nullptr);

        // Call required type constructors
        T *pArrayAddress = reinterpret_cast<T *>(pOriginalAddress + requiredExtraMemory);
        for (size_t i = 0; i < num_elements; i++)
        {
            new (&pArrayAddress[i]) T(value);
        }

        // Record the number of array elements
        uint32_t *pNumElements = reinterpret_cast<uint32_t *>(pArrayAddress) - 1;
        *pNumElements = uint32_t(num_elements);

        return pArrayAddress;
    }

    template <typename T>
    KMP_FORCE_INLINE void DeleteArray(T *&array)
    {
        size_t const requiredAlignment = std::max(alignof(T), size_t(16));
        size_t const requiredExtraMemory = std::max(requiredAlignment, size_t(4));

        // Get number of elements in array and call destructor on each entity
        uint32_t const numElements = *(reinterpret_cast<uint32_t *>(array) - 1);
        for (uint32_t i = 0; i < numElements; i++)
        {
            array[i].~T();
        }

        uint8_t *pOriginalAddress = reinterpret_cast<uint8_t *>(array) - requiredExtraMemory;
        Free((void *&)pOriginalAddress);
        array = nullptr;
    }
}