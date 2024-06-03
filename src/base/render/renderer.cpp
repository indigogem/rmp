#include "renderer.h"
#include <SDL.h>
// #include <SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>

namespace kmp::render
{

    void *GetNativeWindowHandle(void *window_handle)
    {
        void *hwnd = nullptr;
        SDL_PropertiesID props = SDL_GetWindowProperties(static_cast<SDL_Window *>(window_handle));

#if BX_PLATFORM_WINDOWS
        hwnd = SDL_GetProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
#endif

        return hwnd;
    }

    bool Renderer::Initialize(void *window_handle, int width, int height)
    {

        resolution_.x = width;
        resolution_.y = height;

        // If it is called before to bgfx::init, render thread won't be created by bgfx::init
        bgfx::renderFrame();

        // SDL_SysWMinfo wmi;
        // SDL_VERSION(&wmi.version);
        // if (!SDL_GetWindowWMInfo(static_cast<SDL_Window *>(_window_handle), &wmi))
        // {
        //     return false;
        // }

        void *hwnd = GetNativeWindowHandle(window_handle);
        bgfx::Init init;
        init.type = bgfx::RendererType::Count;
        init.vendorId = BGFX_PCI_ID_NONE;
        // init.platformData.nwh = wmi.info.win.window;
        init.platformData.nwh = hwnd;

        // init.platformData.ndt = entry::getNativeDisplayHandle();
        init.platformData.type = bgfx::NativeWindowHandleType::Default;
        init.resolution.width = width;
        init.resolution.height = height;
        init.resolution.reset = BGFX_RESET_VSYNC;

        bgfx::init(init);

        bgfx::reset(width, height, BGFX_RESET_VSYNC);

        // Enable debug text.
        bgfx::setDebug(BGFX_DEBUG_TEXT /*| BGFX_DEBUG_STATS*/);

        // Set view 0 clear state.
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030FF, 1.0f, 0);

        return true;
    }

    void Renderer::Shutdown()
    {
        bgfx::shutdown();
    }

    void Renderer::PresentFrame()
    {
        static int c = 0;
        bgfx::setViewRect(0, 0, 0, kVirtrualWindowsWidth, kVirtrualWindowsHeight);
        bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(0, 1, 0x4f, "Counter:%d", ++c);
        bgfx::touch(0);
        bgfx::frame();
    }
}