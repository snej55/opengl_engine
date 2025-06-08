#include "engine.hpp"

Engine::Engine()
 : EngineObject{"Engine"}
{
    m_arena = new Arena{this};
}

Engine::~Engine()
{
    // free memory
    delete m_arena;
}

bool Engine::init(const int width, const int height, const char* title)
{
    // initialize opengl context
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    return true;
}

bool Engine::createWindow()
{
    
}

bool Engine::alloc(EngineObject*& object) const
{
    return m_arena->alloc(object);
}

void Engine::addObject(EngineObject*& object) const
{
    m_arena->addObject(object);
}

void Engine::removeObject(EngineObject*& object) const
{
    if (object != nullptr)
    {
        m_arena->removeObject(object->getID());
    }
}