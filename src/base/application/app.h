#pragma once

#include "defs.h"
// #include "base/types/array.h"
// #include "engine/engine.h"
#include <SDL.h>
//#include <SDL_syswm.h>


namespace kmp {

    //-------------------------------------------------------------------------
    
	struct WindowParams {
		uint16_t width = 0;
		uint16_t height = 0;
		void* window_handler = nullptr;
	};

    //-------------------------------------------------------------------------

    class AppGlobalState final {
    public:
        AppGlobalState();
        ~AppGlobalState();

    private:
        AppGlobalState(const AppGlobalState&) = delete;
        AppGlobalState& operator=(const AppGlobalState&) = delete;

        bool initialized_ = false;
    };

    //-------------------------------------------------------------------------

    class App final {
    public:
        App();
        ~App();

        int Run(int argc, char* argv[]);

    private:
        App(const App&) = delete;
        App& operator=(const App&) = delete;

        bool Initialize();
        bool Shutdown();
        bool ProcessCommandline(int32_t argc, char** argv);
        void ProcessWindowResizeMessage(int x, int y);
        void ProcessInputMessage(const SDL_Event& input_event);

        bool ApplicationLoop();

        bool InitiliseWindow();
        bool ShutdownWindow();

        //void ShowFatalError(String const& error);

        void OnEvent(const SDL_Event& event);

        bool IsInitialize() const { return initialized_; }

    private:
        WindowParams ws_;
        //Engine engine_;

        bool initialized_ = false;
        bool application_requested_exit_ = false;

        

    };

    //-------------------------------------------------------------------------

} // kmp
