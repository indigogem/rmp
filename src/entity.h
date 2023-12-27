#pragma once
#include "defs.h"

namespace rmp {

    struct EntityID final {
        [[nodiscard]] static EntityID Generate();

    public:

        EntityID() = default;
        explicit EntityID(uint64_t v) : value_(v) { RMP_ASSERT(v != 0); }

        bool IsValid() const { return value_ != 0; }
        void Clear() { value_ = 0; }
        bool operator==(EntityID const& rhs) const = default;

    public:

        uint64_t value_ = 0;
    };

}
