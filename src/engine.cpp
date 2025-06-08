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
    if (!m_arena->alloc(m_window))
    {
        std::cout << "ENGINE::CREATE_WINDOW::ERROR: Failed to allocate memory for window!" << std::endl;
        return false;
    }
    *m_window = Window{this};
    m_arena->addObject(m_window);
    return true;
}

template <typename T>
bool Engine::alloc(T*& object) const
{
    return m_arena->alloc<T>(object);
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