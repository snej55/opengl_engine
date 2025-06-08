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
    // quit glfw
    glfwTerminate();
    std::cout << "ENGINE::FREE: Terminated OpenGL context!" << std::endl;
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

    if (!createWindow(width, height, title))
    {
        std::cout << "ENGINE::INIT::ERROR: Failed to create window!" << std::endl;
        glfwTerminate();
        return false;
    }

    std::cout << "ENGINE::INIT: Successfully initialized GLFW!\n";

    // initialize glad
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "ENGINE::INIT::ERROR: Failed to initialize GLAD!" << std::endl;
        return false;
    }

    std::cout << "ENGINE::INIT: Successfully initialized GLAD!\n";

    // create view port
    m_window->createViewPort();

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // ----- create objects ----- //

    // create IOHandler
    if (!createIOHandler())
    {
        std::cout << "ENGINE::INIT::ERROR: Failed to create IOHandler!" << std::endl;
        return false;
    }
    
    std::cout << "ENGINE::INIT: Successfully created IOHandler!\n";

    return true;
}

// create window object
bool Engine::createWindow(const int width, const int height, const char* title)
{
    // check if window already exists
    if (m_window != nullptr)
    {
        std::cout << "ENGINE::CREATE_WINDOW::ERROR: Window already exists at `" << m_window << "`!\n";
        return false;
    }
    // allocate memory for window
    m_window = new Window{this};
    // add window to arena
    m_arena->addObject(m_window);
    // initialize window
    return m_window->init(width, height, title);
}

// create iohandler for keyboard input
bool Engine::createIOHandler()
{
    if (m_iohandler != nullptr)
    {
        std::cout << "ENGINE::CREATE_IOHANDLER::ERROR: IOHandler already exists at `" << m_iohandler << "`!\n";
        return false;
    } else if (m_window == nullptr)
    {
        std::cout << "ENGINE::CREATE_IOHANDLER::ERROR: Window is required to be created before IOHandler!\n";
        return false;
    }
    // allocate memory for iohandler
    m_iohandler = new IOHandler{this, m_window->getWindow()};
    // add iohandler to arena
    m_arena->addObject(m_iohandler);
    return true; // success!
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

void Engine::removeObjectID(const unsigned int id) const
{
    m_arena->removeObject(id);
}