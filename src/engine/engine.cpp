#include "engine.h"
#include "base/logging/log.h"

namespace kmp
{
    void Engine::ShwowFatalError(const String &error)
    {
        error_handler_(error);
    }

    bool Engine::InitializeCoreSystems(void *_window_handle, int width, int height)
    {
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

        // KMP_PROFILE_FRAME("MainThread");

        return true;
    }
}