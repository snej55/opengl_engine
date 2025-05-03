#include "app.h"

App::App()
 : EngineObject{nullptr}
{
}

App::~App()
{
    free();
}

bool App::init()
{
    // initialize opengl context
    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    
}

void App::free()
{
    EngineObject::free();
}