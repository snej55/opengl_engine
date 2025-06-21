#ifndef SHAPES_H
#define SHAPES_H

#include <glm/glm.hpp>

// use FRect most of the time
// integer rect
struct IRect
{
    int x{0};
    int y{0};
    int w{0};
    int h{0};
};

// floating point rect
struct FRect
{
    float x{0.f};
    float y{0.f};
    float w{0.f};
    float h{0.f};
};

// Generic rect template
template <typename T>
struct Rect
{
    T x{};
    T y{};
    T w{};
    T h{};
};

struct Color
{
    int r{0};
    int g{0};
    int b{0};
    int a{255};
};

inline glm::vec3 color2vec3(const Color color)
{
    return {
        static_cast<float>(color.r) / 255.0f, static_cast<float>(color.g) / 255.0f, static_cast<float>(color.b) / 255.0f
    };
}

inline glm::vec4 color2vec4(const Color color)
{
    return {
        static_cast<float>(color.r) / 255.0f, static_cast<float>(color.g) / 255.0f, static_cast<float>(color.b) / 255.0f,
        static_cast<float>(color.a) / 255.0f
    };
}

#endif