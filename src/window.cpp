#include "window.hpp"

#include <iostream>

// initialize EngineObject
Window::Window(EngineObject* parent)
 : EngineObject{"Window", parent}
{
}

// free
Window::~Window()
{
    free();
}

// create glfw window
bool Window::init(const int width, const int height, const char* title)
{
    // create glfw window
    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    // validate window
    if (m_window == nullptr)
    {
        std::cout << "WINDOW::INIT::ERROR: Failed to create GLFW window!" << std::endl;
        return false;
    }

    // use this window
    glfwMakeContextCurrent(m_window);

    // setup rest of window
    setWidth(width);
    setHeight(height);
    setTitle(title); // implicit conversion

    std::cout << "WINDOW::INIT: Created GLFW window: {dimensions: " << width << " * " << height << ", title: " << title << "}\n";

    // success!
    return true;
}