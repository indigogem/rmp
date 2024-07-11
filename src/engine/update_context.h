#pragma once
#include "defs.h"
#include "base/time/timers.h"
#include "base/math/math.h"

namespace kmp
{
    enum class UpdateStage : uint8_t
    {
        kFrameStart = 0,
        kPrePhysics,
        kPhysics,
        kPostPhysics,
        kFrameEnd,

        kPaused, // Special stage that runs only when the game is paused

        NumStages,
    };

    class UpdateContext
    {

    public:
        // Update info
        //-------------------------------------------------------------------------

        KMP_FORCE_INLINE Seconds GetDeltaTime() const { return delta_time_; }
        KMP_FORCE_INLINE uint64_t GetFrameID() const { return frame_id_; }
        KMP_FORCE_INLINE UpdateStage GetUpdateStage() const { return stage_; }

        // Systems
        //-------------------------------------------------------------------------

        // Frame rate limiter
        //-------------------------------------------------------------------------

        inline bool HasFrameRateLimit() const { return frame_rate_limit_fps_ > 0; }
        inline void SetFrameRateLimit(float FPS) { frame_rate_limit_fps_ = math::fmax(0.0f, FPS); }
        inline float GetFrameRateLimit() const
        {
            KMP_ASSERT(HasFrameRateLimit());
            return frame_rate_limit_fps_;
        }
        inline Milliseconds GetLimitedFrameTime() const
        {
            KMP_ASSERT(HasFrameRateLimit());
            return Milliseconds(1000) / frame_rate_limit_fps_;
        }

    public:
        // Set the time delta for this update
        inline void UpdateDeltaTime(Milliseconds delta_time)
        {
            KMP_ASSERT(delta_time >= 0);

            // Update internal time
            delta_time_ = delta_time.ToSeconds();
            frame_id_++;
        }

    public:
        float frame_rate_limit_fps_ = 60; // 144.0f;
        Seconds delta_time_ = 1.0f / 60.0f;
        uint64_t frame_id_ = 0;
        UpdateStage stage_ = UpdateStage::kFrameStart;
    };

}