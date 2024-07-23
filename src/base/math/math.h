#pragma once
// #include <glm/vec3.hpp> // glm::vec3
//  #include <glm/vec4.hpp>                  // glm::vec4
//  #include <glm/mat4x4.hpp>                // glm::mat4
//  #include <glm/ext/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale
//  #include <glm/ext/matrix_clip_space.hpp> // glm::perspective
// #include <glm/ext/scalar_common.hpp>
// #include <glm/ext/vector_relational.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int2.hpp>

#include <glm/ext/scalar_common.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/constants.hpp>
namespace kmp
{
    namespace math
    {
        using glm::fmax;
        using glm::ivec2;
        using glm::vec2;
        using glm::zero;
    }

    struct Rect
    {
        Rect() = default;
        Rect(float _x, float _y, float _w, float _h) : x(_x), y(_y), w(_w), h(_h) {}

        bool IsContain(float px, float py) const
        {
            return (px >= x && px <= x + w) && (py >= y && py <= y + h);
        }
        math::vec2 GetCenter() const { return math::vec2(x + w / 2.f, y + h / 2.f); }

        float x = 0;
        float y = 0;
        float w = 0;
        float h = 0;
    };
}