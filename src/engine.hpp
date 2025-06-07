#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "engine_types.hpp"
#include "window.hpp"
#include "arena.hpp"

class Engine : public EngineObject
{
public:
    Engine();

    bool init(int width, int height, const char* title);

    // create window object
    void createWindow();
};

#endif