#include "app.h"

#include <iostream>

App::App()
 : EngineObject{nullptr, "App"}
{
}

App::~App()
{
    if (!m_freed)
    {
        free();
    }
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

    std::cout << "Successfully initialized GLFW!\n";

    // initialize glad
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD!" << std::endl;
        return false;
    }

    std::cout << "Successfully initialized GLAD!\n";

    // create view port and add window callbacks
    m_window->createViewPort();

    return true;
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
    glfwTerminate();
    std::cout << "Terminated OpenGL context!" << std::endl;
}