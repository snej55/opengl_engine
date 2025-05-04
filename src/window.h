#ifndef WINDOW_H
#define WINDOW_H

#include "engine.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window : public EngineObject
{
public:
    Window(EngineObject* parent);

    bool init(const int width, const int height);
    void free();

private:
    int m_width{0};
    int m_height{0};

    GLFWwindow* m_Window{nullptr};
};

#endif