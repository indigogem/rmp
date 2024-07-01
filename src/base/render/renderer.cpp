#include "renderer.h"
#include <SDL.h>
#include "base/memory/memory.h"
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

    Renderer *Renderer::Create()
    {
        return kmp::New<render::Renderer>();
    }

    void Renderer::Destroy(Renderer *renderer)
    {
        kmp::Delete(renderer);
    }

    bool Renderer::Initialize(void *window_handle, int width, int height)
    {

        // If it is called before to bgfx::init, render thread won't be created by bgfx::init
        bgfx::renderFrame();

        render_params_.window_handler = window_handle;

        // init window
        void *hwnd = GetNativeWindowHandle(window_handle);
        bgfx::Init init;
        init.type = bgfx::RendererType::Count;
        init.vendorId = BGFX_PCI_ID_NONE;
        init.platformData.nwh = hwnd;
        init.platformData.type = bgfx::NativeWindowHandleType::Default;
        init.resolution.width = width;
        init.resolution.height = height;
        init.resolution.reset = BGFX_RESET_VSYNC;

        bgfx::init(init);

        // reset to window params
        UpdateDeviceParams(window_handle);
        bgfx::reset(render_params_.viewport.w, render_params_.viewport.h, BGFX_RESET_VSYNC);

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

    void Renderer::UpdateDeviceParams(void *window)
    {
        SDL_Window *sdl_window = static_cast<SDL_Window *>(window);

        auto window_flags = SDL_GetWindowFlags(sdl_window);
        SDL_Rect bounds;
        if (window_flags & SDL_WINDOW_FULLSCREEN)
        {
            auto windx = SDL_GetDisplayForWindow(sdl_window);
            const SDL_DisplayMode *dm = SDL_GetDesktopDisplayMode(windx);

            // SDL_Log("dm: w= %d h= %d", dm.w, dm.h);
            bounds.w = dm->w;
            bounds.h = dm->h;
            SDL_SetWindowSize(sdl_window, bounds.w, bounds.h);
        }
        else
        {
            SDL_SetWindowSize(sdl_window, kVirtrualWindowsWidth, kVirtrualWindowsHeight);
            SDL_GetWindowSize(sdl_window, &bounds.w, &bounds.h);
        }

        render_params_.screen_width = bounds.w;
        render_params_.screen_height = bounds.h;
        render_params_.virtual_width = kVirtrualWindowsWidth;
        render_params_.virtual_height = kVirtrualWindowsHeight;
        render_params_.screen_factor = (float)render_params_.virtual_width / (float)render_params_.virtual_height;
        render_params_.scale = 1.f;
        render_params_.bg_color = 0x303030ff;

        GetViewportAndScale(render_params_.viewport, render_params_.scale);
    }

    void Renderer::GetViewportAndScale(Rect &_viewport, float &_scale)
    {
        float wantAspect = (float)render_params_.virtual_width / render_params_.virtual_height;
        float realAspect = (float)render_params_.screen_width / render_params_.screen_height;
        float scale = 1;
        Rect viewport;

        if (SDL_fabs(wantAspect - realAspect) < 0.0001)
        {
            /* The aspect ratios are the same, just scale appropriately */
            scale = (float)render_params_.screen_width / render_params_.virtual_width;
            viewport.x = 0;
            viewport.w = render_params_.screen_width;
            viewport.h = render_params_.screen_height;
            viewport.y = 0;
        }
        else if (wantAspect > realAspect)
        {
            /* We want a wider aspect ratio than is available - letterbox it */
            scale = (float)render_params_.screen_width / render_params_.virtual_width;
            viewport.x = 0;
            viewport.w = render_params_.screen_width;
            viewport.h = (int)SDL_ceil(render_params_.virtual_height * scale);
            viewport.y = (render_params_.screen_height - viewport.h) / 2;
        }
        else
        {
            /* We want a narrower aspect ratio than is available - use side-bars */
            scale = (float)render_params_.screen_height / render_params_.virtual_height;
            viewport.y = 0;
            viewport.h = render_params_.screen_height;
            viewport.w = (int)SDL_ceil(render_params_.virtual_width * scale);
            viewport.x = (render_params_.screen_width - viewport.w) / 2;
        }
        _scale = scale;
        _viewport = viewport;
    }
}