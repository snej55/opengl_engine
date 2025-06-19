#ifndef ENGINE_H
#define ENGINE_H

#include "engine_types.hpp"
#include "window.hpp"
#include "arena.hpp"
#include "iohandler.hpp"
#include "clock.hpp"
#include "shader.hpp"

class Engine final : public EngineObject
{
public:
    Engine();
    // free components
    ~Engine() override;

    // initialize components
    bool init(unsigned int width, unsigned int height, const char* title);
    // update components
    void update();

    // ------ Window ------ //

    // create window object
    bool createWindow(unsigned int width, unsigned int height, const char* title);
    [[nodiscard]] Window* getWindow() const {return m_window;}
    
    // clear screen
    void clear();

    // ------ IOHandler ------ //

    // create iohandler for keyboard input
    bool createIOHandler();
    [[nodiscard]] IOHandler* getIOHandler() const {return m_iohandler;}

    // check if ESC has been pressed from IOHandler
    [[nodiscard]] bool getQuit() const;

    // ------ Clock ------ //

    // create clock
    bool createClock();
    [[nodiscard]] Clock* getClock() const {return m_clock;}

    // get delta time from clock
    [[nodiscard]] float getDeltaTime() const;
    // get time since start from clock in milliseconds
    [[nodiscard]] float getTime() const;

    // ------ Shaders ------ //

    // create shader manager
    bool createShaderManager();
    [[nodiscard]] ShaderManager* getShaderManager() const {return m_shaderManager;}

    // shader manager methods
    void addShader(const std::string& name, const char* fragPath, const char* vertPath) const;
    [[nodiscard]] Shader* getShader(const std::string& name) const;
    void useShader(const std::string& name) const;
    [[nodiscard]] bool shaderExists(const std::string& name) const;

    // ------ Arena ------ //

    // Arena operations
    // add object to arena and update object ID
    void addObject(EngineObject*& object) const;
    // remove object from arena
    void removeObject(EngineObject*& object) const;
    // remove object at index from arena
    void removeObjectID(unsigned int id) const;

private:
    // memory manager
    Arena* m_arena {nullptr};
    // engine components
    Window* m_window {nullptr};
    IOHandler* m_iohandler {nullptr};
    Clock* m_clock {nullptr};
    ShaderManager* m_shaderManager {nullptr};
};

#endif