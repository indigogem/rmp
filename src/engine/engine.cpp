#include "engine.h"
#include "base/logging/log.h"
#include "base/time/timers.h"

#include <chrono>
#include <thread>

namespace kmp
{
    void Engine::ShwowFatalError(const String &error)
    {
        error_handler_(error);
    }

    bool Engine::InitializeCoreSystems(void *_window_handle, int width, int height)
    {
        renderer_ = render::Renderer::Create();

        if (!renderer_->Initialize(_window_handle, width, height))
        {
            KMP_LOG_ERROR("Render", nullptr, "Failed to create render device");
            kmp::Delete(renderer_);
            return false;
        }

        if (!input_system_.Initialize())
        {
            KMP_LOG_ERROR("Input", nullptr, "Failed to initialize input");
            return false;
        }

        return true;
    }

    bool Engine::ShutdownCoreSystems()
    {
        input_system_.Shutdown();

        if (renderer_ != nullptr)
        {
            renderer_->Shutdown();
            render::Renderer::Destroy(renderer_);
        }

        return true;
    }

    Engine::Engine(ErrorHandlingCb &&error_handler)
        : error_handler_(error_handler)
    {
    }

    Engine::~Engine()
    {
    }

    bool Engine::Initialize(void *window_handle, int width, int height)
    {
        if (!InitializeCoreSystems(window_handle, width, height))
        {
            ShwowFatalError("Failed to initialize engine core systems!");
            return false;
        }

        // update_context_.SetFrameRateLimit(60);

        initialized_ = true;

        InlineStringSized ver_str(InlineStringSized::CtorSprintf(), "kmp engine version: %s", KMP_VERSION);
        KMP_LOG(ver_str.c_str());

        return initialized_;
    }

    bool Engine::Shutdown()
    {

        ShutdownCoreSystems();
        initialized_ = false;
        return true;
    }

    bool Engine::Update()
    {
        KMP_ASSERT(initialized_);

        Milliseconds delta_time = 0;
        {
            ScopedTimer<PlatformClock> frame_timer(delta_time);

            // Frame Start
            //-------------------------------------------------------------------------
            {
                update_context_.stage_ = UpdateStage::kFrameStart;

                input_system_.Update(update_context_.GetDeltaTime());
            }

            // Frame End
            //-------------------------------------------------------------------------
            {
                update_context_.stage_ = UpdateStage::kFrameEnd;

                renderer_->PresentFrame();
                input_system_.ClearFrameState();
            }
        }

        // Update Time
        //-------------------------------------------------------------------------

        // Ensure we dont get crazy time delta's when we hit breakpoints
#if KMP_DEVELOPMENT
        if (delta_time.ToSeconds() > 1.0f)
        {
            delta_time = update_context_.GetDeltaTime(); // Keep last frame delta
        }
#endif
        // Frame rate limiter
        if (update_context_.HasFrameRateLimit())
        {
            float const minimumFrameTime = update_context_.GetLimitedFrameTime();
            if (delta_time < minimumFrameTime)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(delta_time));
                delta_time = minimumFrameTime;
                KMP_LOG("1");
            }
        }

        update_context_.UpdateDeltaTime(delta_time);

        // KMP_PROFILE_FRAME("MainThread");

        return true;
    }
}