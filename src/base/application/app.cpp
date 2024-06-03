#include "app.h"
#include "base/memory/memory.h"
#include "base/types/array.h"

#include "base/logging/log.h"
#include "base/input/input_state.h"

#include "base/memory/ptrs.h"

namespace kmp
{

    //-------------------------------------------------------------------------

    AppGlobalState::AppGlobalState()
    {

        memory::Initialize();
        // // threading::Initialize
        log::Initialize();
        // // typesystem

        initialized_ = true;
    }

    AppGlobalState::~AppGlobalState()
    {
        KMP_ASSERT(initialized_);

        // memory::GetTotalAllocatedMemory();
        log::Shutdown();
        memory::Shutdown();

        initialized_ = false;
    }

    //-------------------------------------------------------------------------

    bool App::Initialize()
    {

        KMP_LOG_INFO("System", nullptr, "Engine Application Startup");

        if (!engine_.Initialize(ws_.window_handler, ws_.width, ws_.height))
        {
            return false;
        }

        return true;
    }

    bool App::ProcessCommandline(int32_t argc, char **argv)
    {

        return true;
    }

    void App::ProcessWindowResizeMessage(int x, int y)
    {
    }

    void App::ProcessInputMessage(const SDL_Event &input_event)
    {
        engine_.GetInputSystem()->ForwardInputMessageToInputDevices(input_event);
    }

    bool App::ApplicationLoop()
    {
        return engine_.Update();
    }

    void App::ShowFatalError(const String &error)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", error.c_str(), NULL);
    }

    bool App::InitiliseWindow()
    {
        int err = SDL_Init(0);
        if (err != 0)
        {
            return false;
        }

        const auto width = kVirtrualWindowsWidth;
        const auto heigth = kVirtrualWindowsHeight;

        SDL_Window *window = SDL_CreateWindow(
            "kmp",
            width,
            heigth,
            0);

        if (window == nullptr)
        {
            return false;
        }

        int w, h;
        SDL_GetWindowSize(window, &w, &h);

        ws_.window_handler = window;
        ws_.width = w;
        ws_.height = h;

        return true;
    }

    bool App::ShutdownWindow()
    {
        SDL_DestroyWindow(static_cast<SDL_Window *>(ws_.window_handler));
        SDL_Quit();

        return true;
    }

    App::App()
        : engine_(ErrorHandlingCb([this](const String &error) -> void
                                  { ShowFatalError(error); }))
    {
    }

    App::~App()
    {
    }

    void App::OnEvent(const SDL_Event &event)
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
        {
            application_requested_exit_ = true;
            break;
        }

        case SDL_EVENT_TERMINATING:
        {
            application_requested_exit_ = true;
            break;
        }

        case SDL_EVENT_WINDOW_RESIZED:
        {
            if (IsInitialize())
            {
                int32_t width = event.window.data1;
                int32_t height = event.window.data2;
                ProcessWindowResizeMessage(width, height);
            }
            break;
        }

        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        {
            application_requested_exit_ = true;
            break;
        }

        case SDL_EVENT_WINDOW_FOCUS_GAINED:
        case SDL_EVENT_WINDOW_FOCUS_LOST:
        {
            if (IsInitialize())
            {
                ProcessInputMessage(event);
            }

            break;
        }

        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
        case SDL_EVENT_MOUSE_MOTION:
        case SDL_EVENT_MOUSE_BUTTON_UP:
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_WHEEL:
        {
            if (IsInitialize())
            {
                ProcessInputMessage(event);
            }
            break;
        }
        }
    }

    int App::Run(int argc, char *argv[])
    {

        // TODO: initilise log filepath

        if (!ProcessCommandline(argc, argv))
        {
            ShowFatalError("Application failed to read settings correctly!");
            return EXIT_FAILURE;
        }

        if (!InitiliseWindow())
        {
            ShowFatalError("Application failed to create window!");
            return EXIT_FAILURE;
        }

        if (!Initialize())
        {
            Shutdown();
            ShowFatalError("Application failed to initialize correctly!");
            return EXIT_FAILURE;
        }

        initialized_ = true;

        bool exit = false;
        SDL_Event event;
        while (!exit)
        {

            while (SDL_PollEvent(&event))
            {
                OnEvent(event);

                if (application_requested_exit_)
                {
                    exit = true;
                }
                else
                {
                    exit = !ApplicationLoop();
                }
            }
        }

        Shutdown();

        ShutdownWindow();

        initialized_ = false;

        return EXIT_SUCCESS;
    }

    bool App::Shutdown()
    {
        engine_.Shutdown();
        return true;
    }

    //-------------------------------------------------------------------------

} // kmp