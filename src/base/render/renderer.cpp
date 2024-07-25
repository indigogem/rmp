#include "renderer.h"

#include <SDL.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/timer.h>
#include <bgfx/embedded_shader.h>

#include "generated/shaders/engine/all.h"
#include "base/memory/memory.h"
#include "base/filesystem/filesystem.h"
#include "thirdparty/stb_image.h"

static const bgfx::EmbeddedShader kEmbeddedShaders[] = {
    BGFX_EMBEDDED_SHADER(vs_basic),
    BGFX_EMBEDDED_SHADER(fs_basic),
    BGFX_EMBEDDED_SHADER_END(),
};

struct NormalColorVertex
{
    glm::vec2 position;
    uint32_t color;
};

NormalColorVertex kTriangleVertices[] = {
    {{-0.5f, -0.5f}, 0x339933FF},
    {{0.5f, -0.5f}, 0x993333FF},
    {{0.0f, 0.5f}, 0x333399FF},
};

const uint16_t kTriangleIndices[] = {
    0,
    1,
    2,
};

namespace kmp::render
{

    struct Renderer::RendererImpl
    {
        bgfx::ProgramHandle default_program = BGFX_INVALID_HANDLE;
        bgfx::VertexBufferHandle vertex_buffer = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle index_buffer = BGFX_INVALID_HANDLE;
    };

    namespace
    {
        ///////////////////////////////////////////////////////////////////////////////
        // bgfx specific

        bgfx::ProgramHandle CreateDefaultProgram()
        {
            bgfx::RendererType::Enum renderer_type = bgfx::getRendererType();
            bgfx::ProgramHandle program = bgfx::createProgram(
                bgfx::createEmbeddedShader(kEmbeddedShaders, renderer_type, "vs_basic"),
                bgfx::createEmbeddedShader(kEmbeddedShaders, renderer_type, "fs_basic"),
                true);

            return program;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // sdl specific

        void *GetNativeWindowHandle(void *window_handle)
        {
            void *hwnd = nullptr;
            void *window = static_cast<SDL_Window *>(window_handle);
            SDL_PropertiesID props = SDL_GetWindowProperties(static_cast<SDL_Window *>(window_handle));

#if BX_PLATFORM_WINDOWS
            hwnd = SDL_GetProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
#endif

            return hwnd;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////

    Renderer *Renderer::Create()
    {
        return kmp::New<render::Renderer>();
    }

    void Renderer::Destroy(Renderer *renderer)
    {
        kmp::Delete(renderer);
    }

    Renderer::Renderer() : impl_(kmp::New<RendererImpl>())
    {
    }

    Renderer::~Renderer()
    {
        kmp::Delete(impl_);
    }

    bool Renderer::Initialize(void *window_handle, int width, int height)
    {

        // If it is called before to bgfx::init, render thread won't be created by bgfx::init
        bgfx::renderFrame();

        render_params_.window_handler = window_handle;
        render_params_.is_full_screen = false;

        // init window
        void *hwnd = GetNativeWindowHandle(window_handle);

        bgfx::Init init;
        init.type = bgfx::RendererType::Count;
        init.vendorId = BGFX_PCI_ID_NONE;
        init.platformData.nwh = hwnd;
        init.platformData.type = bgfx::NativeWindowHandleType::Default;
        // init.platformData.nwh  = entry::getNativeWindowHandle(entry::kDefaultWindowHandle);
        // init.platformData.ndt  = entry::getNativeDisplayHandle();
        // init.platformData.type = entry::getNativeWindowHandleType();
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

        impl_->default_program = CreateDefaultProgram();

        bgfx::VertexLayout color_vertex_layout;
        color_vertex_layout.begin()
            .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();

        impl_->vertex_buffer = bgfx::createVertexBuffer(bgfx::makeRef(kTriangleVertices, sizeof(kTriangleVertices)), color_vertex_layout);
        impl_->index_buffer = bgfx::createIndexBuffer(bgfx::makeRef(kTriangleIndices, sizeof(kTriangleIndices)));

        Blob data;
        kmp::file::LoadFile("test.jpg", data);

        return true;
    }

    void Renderer::Shutdown()
    {
        if (bgfx::isValid(impl_->default_program))
        {
            bgfx::destroy(impl_->default_program);
        }

        if (bgfx::isValid(impl_->index_buffer))
        {
            bgfx::destroy(impl_->index_buffer);
        }

        if (bgfx::isValid(impl_->vertex_buffer))
        {
            bgfx::destroy(impl_->vertex_buffer);
        }

        bgfx::shutdown();
    }

    void Renderer::Begin()
    {
        if (need_reset_params_)
        {
            // bgfx::submit(0, BGFX_INVALID_HANDLE, BGFX_INVALID_HANDLE);
            UpdateDeviceParams(render_params_.window_handler);
            bgfx::reset(render_params_.viewport.w, render_params_.viewport.h, BGFX_RESET_VSYNC);
            need_reset_params_ = false;
        }

        bgfx::setViewRect(0, 0, 0, render_params_.viewport.w, render_params_.viewport.h);

        bgfx::touch(0);

        //   UpdateView(DEFAULT);
    }

    void Renderer::End()
    {
        bgfx::frame();
    }

    void Renderer::Draw()
    {
        static int c = 0;
        int64_t cnt = bx::getHPFrequency();

        bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(0, 1, 0x4f, "Counter:%d", ++c);
        bgfx::dbgTextPrintf(0, 2, 0x4f, "HP Counter:%d", cnt);
        // bgfx::touch(0);
        // bgfx::frame();

        bgfx::setState(
            BGFX_STATE_WRITE_R | BGFX_STATE_WRITE_G | BGFX_STATE_WRITE_B | BGFX_STATE_WRITE_A);

        bgfx::setVertexBuffer(0, impl_->vertex_buffer);
        bgfx::setIndexBuffer(impl_->index_buffer);
        bgfx::submit(0, impl_->default_program);
    }

    void Renderer::ToggleFullScreen()
    {
        render_params_.is_full_screen = !render_params_.is_full_screen;

        SDL_Window *sdl_window = static_cast<SDL_Window *>(render_params_.window_handler);
        SDL_SetWindowFullscreen(sdl_window, render_params_.is_full_screen ? SDL_WINDOW_FULLSCREEN : 0);
        NeedResetParams();
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