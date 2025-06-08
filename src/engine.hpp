#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "engine_types.hpp"
#include "window.hpp"
#include "arena.hpp"
#include "iohandler.hpp"
#include "clock.hpp"

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
    bool createWindow(int width, int height, const char* title);
    [[nodiscard]] Window* getWindow() {return m_window;}
    
    // clear screen
    void clear();

    // ------ IOHandler ------ //

    // create iohandler for keyboard input
    bool createIOHandler();
    [[nodiscard]] IOHandler* getIOHandler() {return m_iohandler;}

    // check if ESC has been pressed from IOHandler
    bool getQuit() const;

    // ------ Clock ------ //

    // create clock
    bool createClock();
    [[nodiscard]] Clock* getClock() {return m_clock;}

    // get deltatime from clock
    [[nodiscard]] float getDeltaTime() const;
    // get time since start from clock in milliseconds
    [[nodiscard]] float getTime() const;

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
};

#endif