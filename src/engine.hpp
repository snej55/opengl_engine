#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "engine_types.hpp"
#include "window.hpp"
#include "arena.hpp"
#include "iohandler.hpp"

class Engine final : public EngineObject
{
public:
    Engine();
    ~Engine() override;

    bool init(int width, int height, const char* title);

    // create window object
    bool createWindow(int width, int height, const char* title);
    [[nodiscard]] Window* getWindow() {return m_window;}

    // create iohandler for keyboard input
    bool createIOHandler();
    [[nodiscard]] IOHandler* getIOHandler() {return m_iohandler;}

    // Arena operations
    // add object to arena and update object ID
    void addObject(EngineObject*& object) const;
    // remove object from arena
    void removeObject(EngineObject*& object) const;
    // remove object at index from arena
    void removeObjectID(unsigned int id) const;

private:
    Arena* m_arena {nullptr};
    Window* m_window {nullptr};
    IOHandler* m_iohandler {nullptr};
};

#endif