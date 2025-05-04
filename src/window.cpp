#include "window.h"

#include <iostream>

Window::Window(EngineObject* parent)
 : EngineObject{parent}
{
}

bool Window::init(const int width, const int height, const char* title)
{
    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    // validate window
    if (m_window == nullptr)
    {
        std::cout << "Failed to create GLFW window!" << std::endl;
        return false;
    }
    glfwMakeContextCurrent(m_window);

    setWidth(width);
    setHeight(height);
    setTitle(title);

    return true;
}

void Window::createViewPort()
{
    glViewport(0, 0, m_width, m_height);
}

void Window::free()
{
    EngineObject::free();
}