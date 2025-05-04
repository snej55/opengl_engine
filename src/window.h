#ifndef WINDOW_H
#define WINDOW_H

#include "engine.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

class Window : public EngineObject
{
public:
    Window(EngineObject* parent);

    bool init(const int width, const int height, const char* title);
    void free();

    [[nodiscard]] int getWidth() const;
    [[nodiscard]] int getHeight() const;

private:
    int m_width{0};
    int m_height{0};

    std::string m_title{};

    GLFWwindow* m_Window{nullptr};
};

#endif