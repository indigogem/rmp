#pragma once

#include "defs.h"
#include "base/math/math.h"
#include "eastl/unique_ptr.h"

namespace kmp::render
{
    class RendererUPtr;

    class KMP_BASE_API Renderer
    {
        typedef eastl::unique_ptr<Renderer> RendererUPtr;

    public:
        Renderer() = default;
        ~Renderer() = default;

        bool Initialize(void *_window_handle, int width, int height);
        void Shutdown();

        void PresentFrame();

        [[nodiscard]] static RendererUPtr Create() { return nullptr; }

    private:
        math::ivec2 resolution_;
    };

}