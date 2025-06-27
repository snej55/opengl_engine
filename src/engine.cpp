// gl libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// json libary
#include <JSON/json.hpp>

#include "glm/ext/matrix_clip_space.hpp"
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
    // fix framebuffer scaling issue
    m_window->updateDimensions();

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    std::cout << "ENGINE::INIT: Initialized global OpenGL state!\n";

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

    if (!createCamera())
    {
        std::cout << "ENGINE::INIT::ERROR: Failed to create Camera!" << std::endl;
        return false;
    }

    if (!createModelManager())
    {
        std::cout << "ENGINE::INIT::ERROR: Failed to create ModelManager!" << std::endl;
        return false;
    }

    if (!createPostProcessor())
    {
        std::cout << "ENGINE::INIT::ERROR: Failed to create PostProcessor!" << std::endl;
        return false;
    }
    m_postProcessor->init(getWidth(), getHeight());


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

    // update camera
    if (m_cameraEnabled)
    {
        if (getPressed(GLFW_KEY_W))
        {
            m_camera->processInput(CameraN::CameraMotion::FORWARD, getDeltaTime());
        }
        if (getPressed(GLFW_KEY_S))
        {
            m_camera->processInput(CameraN::CameraMotion::BACKWARD, getDeltaTime());
        }
        if (getPressed(GLFW_KEY_A))
        {
            m_camera->processInput(CameraN::CameraMotion::LEFT, getDeltaTime());
        }
        if (getPressed(GLFW_KEY_D))
        {
            m_camera->processInput(CameraN::CameraMotion::RIGHT, getDeltaTime());
        }
    }
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

// enable wireframe rendering
void Engine::enableWireframe() const
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Engine::disableWireframe() const
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// ------ IOHandler ------ //

// create iohandler for keyboard input
bool Engine::createIOHandler()
{
    if (m_iohandler != nullptr)
    {
        std::cout << "ENGINE::CREATE_IOHANDLER::ERROR: IOHandler already exists at `" << m_iohandler << "`!\n";
        return false;
    }
    if (m_window == nullptr)
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

bool Engine::getPressed(const int key) const
{
    return m_iohandler->getPressed(key);
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

void Engine::setBool(const std::string& name, const bool value, const std::string& shaderName) const
{
    const Shader* shader {getShader(shaderName)};
    if (shader != nullptr)
    {
        shader->setBool(name, value);
    }
}

void Engine::setInt(const std::string& name, const int value, const std::string& shaderName) const
{
    const Shader* shader {getShader(shaderName)};
    if (shader != nullptr)
    {
        shader->setInt(name, value);
    }
}

void Engine::setFloat(const std::string& name, const float value, const std::string& shaderName) const
{
    const Shader* shader {getShader(shaderName)};
    if (shader != nullptr)
    {
        shader->setFloat(name, value);
    }
}

// vectors
void Engine::setVec2(const std::string& name, const glm::vec2& value, const std::string& shaderName) const
{
    const Shader* shader {getShader(shaderName)};
    if (shader != nullptr)
    {
        shader->setVec2(name, value);
    }
}

void Engine::setVec2(const std::string& name, const float x, const float y, const std::string& shaderName) const
{
    const Shader* shader {getShader(shaderName)};
    if (shader != nullptr)
    {
        shader->setVec2(name, x, y);
    }
}

void Engine::setVec3(const std::string& name, const glm::vec3& value, const std::string& shaderName) const
{
    const Shader* shader {getShader(shaderName)};
    if (shader != nullptr)
    {
        shader->setVec3(name, value);
    }
}

void Engine::setVec3(const std::string& name, const float x, const float y, const float z, const std::string& shaderName) const
{
    const Shader* shader {getShader(shaderName)};
    if (shader != nullptr)
    {
        shader->setVec3(name, x, y, z);
    }
}

void Engine::setVec4(const std::string& name, const glm::vec4& value, const std::string& shaderName) const
{
    const Shader* shader {getShader(shaderName)};
    if (shader != nullptr)
    {
        shader->setVec4(name, value);
    }
}

void Engine::setVec4(const std::string& name, const float x, const float y, const float z, const float w, const std::string& shaderName) const
{
    const Shader* shader {getShader(shaderName)};
    if (shader != nullptr)
    {
        shader->setVec4(name, x, y, z, w);
    }
}

// matrices
void Engine::setMat2(const std::string& name, const glm::mat2& value, const std::string& shaderName) const
{
    const Shader* shader {getShader(shaderName)};
    if (shader != nullptr)
    {
        shader->setMat2(name, value);
    }
}

void Engine::setMat3(const std::string& name, const glm::mat3& value, const std::string& shaderName) const
{
    const Shader* shader {getShader(shaderName)};
    if (shader != nullptr)
    {
        shader->setMat3(name, value);
    }
}

void Engine::setMat4(const std::string& name, const glm::mat4& value, const std::string& shaderName) const
{
    const Shader* shader {getShader(shaderName)};
    if (shader != nullptr)
    {
        shader->setMat4(name, value);
    }
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

// ------ Camera ------ //

bool Engine::createCamera()
{
    if (m_camera != nullptr)
    {
        std::cout << "ENGINE::CREATE_CAMERA::ERROR: Camera already exists at `" << m_camera << "`!\n";
        return false;
    }
    // create new camera
    m_camera = new Camera{this};
    m_arena->addObject(m_camera);
    return true;
}

glm::mat4 Engine::getViewMatrix() const
{
    return m_camera->getViewMatrix();
}

glm::mat4 Engine::getProjectionMatrix() const
{
    return glm::perspective(glm::radians(m_camera->getZoom()),
                            static_cast<float>(getWidth()) / static_cast<float>(getHeight()), 0.1f, 100000.0f);
}

glm::vec3 Engine::getCameraPosition() const
{
    return m_camera->getPosition();
}

glm::mat4 Engine::getNormalMatrix(const glm::mat4& model) const
{
    return glm::transpose(glm::inverse(model));
}

void Engine::setCameraEnabled(const bool value)
{
    m_cameraEnabled = value;
    glfwSetInputMode(m_window->getWindow(), GLFW_CURSOR, value ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

// ------ Models ------ //

bool Engine::createModelManager()
{
    if (m_modelManager != nullptr)
    {
        std::cout << "ENGINE::CREATE_MODEL_MANAGER::ERROR: Model manager already exists at `" << m_modelManager << "`!\n";
        return false;
    }

    m_modelManager = new ModelManager{this};
    m_arena->addObject(m_modelManager);
    return true;
}

void Engine::addModel(const std::string& name, const std::string& path) const
{
    m_modelManager->addModel(name, path, m_arena);
}

Model* Engine::getModel(const std::string& name) const
{
    return m_modelManager->getModel(name);
}

void Engine::renderModel(const std::string& name, const Shader* shader) const
{
    m_modelManager->renderModel(shader, name);
}

bool Engine::modelExists(const std::string& name) const
{
    return m_modelManager->modelExists(name);
}

// ------ Post Processor ------ //

bool Engine::createPostProcessor()
{
    if (m_postProcessor != nullptr)
    {
        std::cout << "ENGINE::CREATE_POST_PROCESSOR::ERROR: Post processor already exists at `" << m_postProcessor << "`!\n";
        return false;
    }

    m_postProcessor = new PostProcessor{this};
    m_arena->addObject(m_postProcessor);
    return true;
}

void Engine::enablePostProcessing() const
{
    m_postProcessor->enable();
}

void Engine::disablePostProcessing() const
{
    m_postProcessor->disable();
}

void Engine::renderPostProcessing() const
{
    m_postProcessor->render(getShader("screenShader"));
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

// ---- Window callbacks ---- //
void Engine::mouse_callback(const double xPosIn, const double yPosIn)
{
    const float xPos {static_cast<float>(xPosIn)};
    const float yPos {static_cast<float>(yPosIn)};

    if (m_camFirstMouse)
    {
        m_camLastX = xPos;
        m_camLastY = yPos;
        m_camFirstMouse = false;
    }

    const float xOffset {xPos - m_camLastX};
    const float yOffset {m_camLastY - yPos}; // remember to reverse because of reversed coordinates

    m_camLastX = xPos;
    m_camLastY = yPos;

    m_camera->processMouseMovement(xOffset, yOffset);
}

void Engine::scroll_callback(const double yOffset) const
{
    m_camera->processMouseScroll(static_cast<float>(yOffset));
}
