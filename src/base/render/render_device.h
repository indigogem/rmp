#pragma once

#include "defs.h"
#include "base/math/math.h"

namespace kmp::render
{

    class KMP_BASE_API RenderDevice
    {

    public:
        RenderDevice() = default;
        ~RenderDevice() = default;

        bool Initialize(void *_window_handle, int width, int height);
        void Shutdown();

        void PresentFrame();

    private:
        math::ivec2 resolution_;
    };
}