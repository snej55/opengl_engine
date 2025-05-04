#include "window.h"

Window::Window(EngineObject* parent)
 : EngineObject{parent}
{
}

bool Window::init(const int width, const int height, const char* title)
{
    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
}