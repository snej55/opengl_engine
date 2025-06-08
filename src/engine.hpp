#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "engine_types.hpp"
#include "window.hpp"
#include "arena.hpp"

class Engine final : public EngineObject
{
public:
    Engine();
    ~Engine() override;

    bool init(int width, int height, const char* title);

    // create window object
    bool createWindow(int width, int height, const char* title);

    // Arena operations
    // add object to arena and update object ID
    void addObject(EngineObject*& object) const;
    // remove object from arena
    void removeObject(EngineObject*& object) const;

private:
    Window* m_window {nullptr};
    Arena* m_arena {nullptr};
};

#endif