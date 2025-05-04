#include "window.h"

#include <iostream>

Window::Window(EngineObject* parent)
 : EngineObject{parent}
{
}

Window::~Window()
{
    free();
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

    glfwSetWindowUserPointer(m_window, this);

    glfwSetFramebufferSizeCallback(m_window, win_framebuffer_size_callback);
    glfwSetCursorPosCallback(m_window, win_mouse_callback);
    glfwSetScrollCallback(m_window, win_scroll_callback);
}

void Window::free()
{
    EngineObject::free();
    glfwDestroyWindow(m_window);
    m_window = nullptr;
}

// window callbacks
void Window::framebuffer_size_callback(int width, int height)
{
    setWidth(width);
    setHeight(height);
    glViewport(0, 0, width, height);
}

void Window::mouse_callback(double xpos, double ypos)
{
    // pass
    return;
}

void Window::scroll_callback(double xoffset, double yoffset)
{
    // pass
    return;
}

// glfw callbacks
void Window::win_framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    if (Window* handler{static_cast<Window*>(glfwGetWindowUserPointer(window))})
    {
        handler->framebuffer_size_callback(width, height);
    }
}

void Window::win_mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (Window* handler{static_cast<Window*>(glfwGetWindowUserPointer(window))})
    {
        handler->mouse_callback(xpos, ypos);
    }
}

void Window::win_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (Window* handler{static_cast<Window*>(glfwGetWindowUserPointer(window))})
    {
        handler->scroll_callback(xoffset, yoffset);
    }
}