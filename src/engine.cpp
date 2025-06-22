// gl libaries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// json libary
#include <JSON/json.hpp>
using json = nlohmann::json;

#include <iostream>
#include <fstream>

#include "engine.hpp"
#include "util.hpp"

Engine::Engine()
 : EngineObject{"Engine"}
{
    // memory manager
    m_arena = new Arena{this};
}

// free components
Engine::~Engine()
{
    // free memory
    delete m_arena;
    // quit glfw
    glfwTerminate();
    std::cout << "ENGINE::FREE: Terminated OpenGL context!" << std::endl;
}

// initialize components
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
    
    // create clock
    if (!createClock())
    {
        std::cout << "ENGINE::INIT::ERROR: Failed to create Clock!" << std::endl;
        return false;
    }

    // create shader manager
    if (!createShaderManager())
    {
        std::cout << "ENGINE::INIT::ERROR: Failed to create ShaderManager!" << std::endl;
        return false;
    }

    // check shaders
    if (!checkShaders())
    {
        std::cout << "ENGINE::INIT::ERROR: Failed to check all shaders!" << std::endl;
        return false;
    }
    loadShaders(); // load verified shaders

    // create texture manager
    if (!createTextureManager())
    {
        std::cout << "ENGINE::INIT::ERROR: Failed to create TextureManager!" << std::endl;
        return false;
    }

    if (!createShapeManager())
    {
        std::cout << "ENGINE::INIT::ERROR: Failed to create ShapeManager!" << std::endl;
        return false;
    }

    std::cout << "ENGINE::INIT: Successfully created components!\n";

    return true;
}

// update components
void Engine::update()
{
    // update delta time
    m_clock->update();
    // check for esc
    m_iohandler->update();
    m_window->setQuit(m_iohandler->getQuit());
    // swap buffers
    m_window->tick();
}

// ------ Window ------ //

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

// clear gl buffers
void Engine::clear() const
{
    m_window->clear();
}

// ------ IOHandler ------ //

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

bool Engine::getQuit() const
{
    return m_window->getShouldClose();
}

// ------ Clock ------ //

// create clock
bool Engine::createClock()
{
    if (m_clock != nullptr)
    {
        std::cout << "ENGINE::CREATE_CLOCK::ERROR: Clock already exists at `" << m_iohandler << "`!\n";
        return false;
    }
    // allocate memory for clock
    m_clock = new Clock{this};
    // add clock to arena
    m_arena->addObject(m_clock);
    return true;
}

// get delta time from clock
float Engine::getDeltaTime() const
{
    return m_clock->getDeltaTime();
}

// get time from clock
float Engine::getTime() const
{
    return m_clock->getTime();
}

// ------ Shader Manager ------ //
bool Engine::createShaderManager()
{
    if (m_shaderManager != nullptr)
    {
        std::cout << "ENGINE::CREATE_SHADER_MANAGER::ERROR: Shader manager already exists at `" << m_shaderManager << "`!\n";
        return false;
    }
    // allocate memory for shader manager
    m_shaderManager = new ShaderManager{this};
    // add shader manager to arena
    m_arena->addObject(m_shaderManager);
    return true;
}

void Engine::addShader(const std::string& name, const char* fragPath, const char* vertPath) const
{
    m_shaderManager->addShader(name, fragPath, vertPath, m_arena);
}

Shader* Engine::getShader(const std::string& name) const
{
    return m_shaderManager->getShader(name);
}

void Engine::useShader(const std::string& name) const
{
    m_shaderManager->useShader(name);
}

bool Engine::shaderExists(const std::string& name) const
{
    return m_shaderManager->shaderExists(name);
}

// check builtin shaders
bool Engine::checkShaders()
{
    // check if config file exists
    if (!Util::fileExists("shaders/shaders.json"))
    {
        std::cout << "ENGINE::CHECK_SHADERS::ERROR: Could not find `shaders.json` at `shaders/shaders.json`!";
        return false;
    }

    // load json
    std::ifstream file {"shaders/shaders.json"};
    json data = json::parse(file); // NOTE: brace initialization doesn't work

    // check builtin shaders
    for (const auto& shader : data["builtin"])
    {
        std::string name {shader["name"]};
        std::string vertPath {"shaders/builtin/" + std::string(shader["shader"]["vert"])};
        std::string fragPath {"shaders/builtin/" + std::string(shader["shader"]["frag"])};

        // check if vertex shader exists
        if (!Util::fileExists(vertPath))
        {
            std::cout << "ENGINE::CHECK_SHADERS::ERROR: Could not find vertex shader for *" << name << "* at: `" << vertPath << "`!\n";
            file.close();
            return false;
        }

        // same for fragment shader
        if (!Util::fileExists(fragPath))
        {
            std::cout << "ENGINE::CHECK_SHADERS::ERROR: Could not find fragment shader for *" << name << "* at: `" << fragPath << "`!\n";
            file.close();
            return false;
        }

        std::cout << "Found builtin shader *" << name << "* at {vert: " << vertPath << ", frag: " << fragPath << "}\n";
    }

    // repeat for custom
    for (const auto& shader : data["custom"])
    {
        std::string name {shader["name"]};
        std::string vertPath {"shaders/" + std::string(shader["shader"]["vert"])};
        std::string fragPath {"shaders/" + std::string(shader["shader"]["frag"])};

        // check if vertex shader exists
        if (!Util::fileExists(vertPath))
        {
            std::cout << "ENGINE::CHECK_SHADERS::ERROR: Could not find vertex shader for *" << name << "* at: `" << vertPath << "`!\n";
            file.close();
            return false;
        }

        // same for fragment shader
        if (!Util::fileExists(fragPath))
        {
            std::cout << "ENGINE::CHECK_SHADERS::ERROR: Could not find fragment shader for *" << name << "* at: `" << fragPath << "`!\n";
            file.close();
            return false;
        }

        std::cout << "Found custom shader *" << name << "* at {vert: " << vertPath << ", frag: " << fragPath << "}\n";
    }

    // close fstream
    file.close();
    // set flag
    m_checkedShaders = true;
    return true;
}

// load shaders from shaders.json
void Engine::loadShaders()
{
    if (!m_checkedShaders)
    {
        std::cout << "ENGINE::LOAD_SHADERS::ERROR: Cannot load shaders: shader files have not been verified!\n";
        return;
    }

    std::ifstream file {"shaders/shaders.json"};
    json data = json::parse(file);
    // load builtin shaders
    for (const auto& shader : data["builtin"])
    {
        std::string name {shader["name"]};
        std::string vertPath {"shaders/builtin/" + std::string(shader["shader"]["vert"])};
        std::string fragPath {"shaders/builtin/" + std::string(shader["shader"]["frag"])};
        addShader(name, fragPath.c_str(), vertPath.c_str());
    }
    // repeat for custom shaders
    for (const auto& shader : data["custom"])
    {
        std::string name {shader["name"]};
        std::string vertPath {"shaders/" + std::string(shader["shader"]["vert"])};
        std::string fragPath {"shaders/" + std::string(shader["shader"]["frag"])};
        addShader(name, fragPath.c_str(), vertPath.c_str());
    }

    m_loadedShaders = true;
}


// ------ Texture Manager ------ //
bool Engine::createTextureManager()
{
    if (m_textureManager != nullptr)
    {
        std::cout << "ENGINE::CREATE_TEXTURE_MANAGER::ERROR: Texture manager already exists at `" << m_textureManager << "`!\n";
        return false;
    }
    // allocate memory for texture manager
    m_textureManager = new TextureManager{this};
    // add texture manager to arena
    m_arena->addObject(m_textureManager);
    return true;
}

// load new texture
void Engine::addTexture(const std::string& name, const char* path) const
{
    m_textureManager->addTexture(path, name.c_str(), m_arena);
}

Texture* Engine::getTexture(const std::string& name) const
{
    return m_textureManager->getTexture(name);
}

void Engine::activateTexture(const std::string& name, const int slot) const
{
    m_textureManager->activateTexture(name, slot);
}

bool Engine::textureExists(const std::string& name) const
{
    return m_textureManager->textureExists(name);
}

// ------ Shape Manager ------ //
bool Engine::createShapeManager()
{
    if (m_shapeManager != nullptr)
    {
        std::cout << "ENGINE::CREATE_SHAPE_MANAGER::ERROR: Shape manager already exists at `" << m_shapeManager << "`!\n";
        return false;
    }
    // create new shape manager
    m_shapeManager = new ShapeManager{this};
    // add manager to arena
    m_arena->addObject(m_shapeManager);
    return true;
}

void Engine::drawRect(const FRect& rect, const Color& color) const
{
    m_shapeManager->drawRect<float>(rect, color, m_shaderManager);
}


// ------ Arena ------ //

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