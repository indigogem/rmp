#include "memory.h"
#include "defs.h"

#if KMP_USE_CUSTOM_ALLOCATOR
#include "rpmalloc/rpmalloc.h"
#else
#include <stdlib.h>
#endif

namespace kmp
{
        namespace memory
        {
                static bool is_memory_system_initialized_ = false;
                static rpmalloc_config_t rpmalloc_config_;

                static void CustomAssert(char const *message)
                {
                        KMP_ASSERT(false && "custom alloc mem error");
                }

                void Initialize()
                {
                        KMP_ASSERT(!is_memory_system_initialized_);

                        // Init config
                        memset(&rpmalloc_config_, 0, sizeof(rpmalloc_config_t));
                        rpmalloc_config_.error_callback = &CustomAssert;

#if KMP_USE_CUSTOM_ALLOCATOR
                        rpmalloc_initialize_config(&rpmalloc_config_);
#endif

                        is_memory_system_initialized_ = true;
                }

                void Shutdown()
                {
                        KMP_ASSERT(is_memory_system_initialized_);
                        is_memory_system_initialized_ = false;

#if KMP_USE_CUSTOM_ALLOCATOR
                        rpmalloc_finalize();
#endif
                }

                void InitializeThreadHeap()
                {
                        // Since our tasks are not bound to a specific thread and we may alloc on one and free on another. This prevents us from calling thread finalize when we shutdown a thread
                        // as we can not guarantee that we have freed everything that may have been allocated from this thread.
                        // This is not a problem since on application shutdown, we call rpmalloc_finalize, which will release the thread heaps
#if KMP_USE_CUSTOM_ALLOCATOR
                        rpmalloc_thread_initialize();
#endif
                }

                void ShutdownThreadHeap()
                {
#if KMP_USE_CUSTOM_ALLOCATOR
                        rpmalloc_thread_finalize(1);
#endif
                }

                size_t GetTotalRequestedMemory()
                {
#if KMP_USE_CUSTOM_ALLOCATOR
                        rpmalloc_global_statistics_t stats;
                        rpmalloc_global_statistics(&stats);
                        return stats.mapped;
#else
                        return 0;
#endif
                }

                size_t GetTotalAllocatedMemory()
                {
#if KMP_USE_CUSTOM_ALLOCATOR
                        rpmalloc_global_statistics_t stats;
                        rpmalloc_global_statistics(&stats);
                        return stats.mapped_total;
#else
                        return 0;
#endif
                }
        }

        void *Alloc(size_t size, size_t alignment)
        {
                KMP_ASSERT(memory::is_memory_system_initialized_);

                if (size == 0)
                        return nullptr;

                void *pMemory = nullptr;

#if KMP_USE_CUSTOM_ALLOCATOR
                pMemory = rpaligned_alloc(alignment, size);
#elif _WIN32
                pMemory = _aligned_malloc(size, alignment);
#endif

                KMP_ASSERT(memory::IsAligned(pMemory, alignment));
                return pMemory;
        }

        void *Realloc(void *memory, size_t new_size, size_t original_alignment)
        {
                KMP_ASSERT(memory::is_memory_system_initialized_);

                void *pReallocatedMemory = nullptr;

#if KMP_USE_CUSTOM_ALLOCATOR
                pReallocatedMemory = rprealloc(memory, new_size);
#elif _WIN32
                pReallocatedMemory = _aligned_realloc(memory, new_size, original_alignment);
#endif

                KMP_ASSERT(pReallocatedMemory != nullptr);
                return pReallocatedMemory;
        }

        void Free(void *&memory)
        {
                KMP_ASSERT(memory::is_memory_system_initialized_);

#if KMP_USE_CUSTOM_ALLOCATOR
                rpfree((uint8_t *)memory);
#elif _WIN32
                _aligned_free(memory);
#endif

                memory = nullptr;
        }

}
