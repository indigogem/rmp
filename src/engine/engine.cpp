#include "engine.h"
#include "base/logging/log.h"
#include "base/time/timers.h"

namespace kmp
{
    void Engine::ShwowFatalError(const String &error)
    {
        error_handler_(error);
    }

    bool Engine::InitializeCoreSystems(void *_window_handle, int width, int height)
    {
        render_device_ = kmp::New<render::RenderDevice>();
        if (!render_device_->Initialize(_window_handle, width, height))
        {
            KMP_LOG_ERROR("Render", nullptr, "Failed to create render device");
            kmp::Delete(render_device_);
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

        if (render_device_ != nullptr)
        {
            render_device_->Shutdown();
            kmp::Delete(render_device_);
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

            render_device_->PresentFrame();
        }

        // KMP_PROFILE_FRAME("MainThread");

        return true;
    }
}