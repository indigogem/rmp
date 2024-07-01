#pragma once

#include "defs.h"
#include "base/math/math.h"
// #include "base/memory/ptrs.h"

namespace kmp::render
{

    struct RenderParam
    {
        uint32_t screen_width;
        uint32_t screen_height;
        uint32_t bg_color;
        uint32_t virtual_width;
        uint32_t virtual_height;
        float screen_factor;
        float scale;
        Rect viewport;
        void *window_handler;
    };

    class KMP_BASE_API Renderer final
    {
    public:
        [[nodiscard]] static Renderer *Create();
        static void Destroy(Renderer *renderer);

    public:
        Renderer() = default;
        ~Renderer() = default;

        bool Initialize(void *_window_handle, int width, int height);
        void Shutdown();

        void PresentFrame();
        void ResetParams() { need_reset_params_ = true; }

    private:
        void UpdateDeviceParams(void *window);
        void GetViewportAndScale(Rect &viewport, float &scale);

    private:
        RenderParam render_params_;
        bool need_reset_params_ = false;

        // math::ivec2 resolution_;
    };

}