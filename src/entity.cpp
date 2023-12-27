#include "entity.h"
#include <atomic>

namespace rmp {
    
    EntityID EntityID::Generate() {
        static std::atomic_uint64_t entity_id = 1;

        EntityID id;
        id.value_ = entity_id++;
        RMP_ASSERT(id.value_ != UINT64_MAX);
        return id;
    }
}