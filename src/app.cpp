#include "app.h"

#include <iostream>

App::App()
 : EngineObject{nullptr}
{
}

App::~App()
{
    free();
}

bool App::init(const int width, const int height, const char* title)
{
    // initialize opengl context
    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // initialize window
    if (!createWindow(width, height, title))
    {
        std::cout << "APP::INIT::ERROR: Unable to initialize new window!" << std::endl;
        glfwTerminate();
        return false;
    }

    // initialize glad
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD!" << std::endl;
        return false;
    }
}

bool App::createWindow(const int width, const int height, const char* title)
{
    if (m_window == nullptr)
    {
        m_window = new Window{this};
        return m_window->init(width, height, title);
    } else {
        std::cout << "APP::CREATE_WINDOW::ERROR: Window already exists!" << std::endl;
        return false;
    }
}

void App::free()
{
    EngineObject::free();
}