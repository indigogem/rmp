#pragma once
#include "defs.h"
#include "base/types/string.h"
#include "base/types/array.h"
#include "base/input/input_system.h"
#include "base/render/renderer.h"
#include "engine/update_context.h"

namespace kmp
{
    //-------------------------------------------------------------------------

    using ErrorHandlingCb = Function<void(const String &error)>;

    //-------------------------------------------------------------------------

    class Engine final
    {
    public:
        Engine(ErrorHandlingCb &&error_handler);
        ~Engine();

        bool Initialize(void *window_handle, int width, int height);
        bool Shutdown();
        bool Update();

        inline input::InputSystem *GetInputSystem() { return &input_system_; }

    private:
        void ShwowFatalError(const String &error);

        bool InitializeCoreSystems(void *_window_handle, int width, int height);
        bool ShutdownCoreSystems();

    private:
        ErrorHandlingCb error_handler_;

        input::InputSystem input_system_;

        UpdateContext update_context_;

        // render
        render::Renderer *renderer_ = nullptr;

        bool initialized_ = false;
    };

}