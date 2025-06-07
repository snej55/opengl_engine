#ifndef ENGINE_H
#define ENGINE_H

#include <string>

#include "engine_types.hpp"
#include "window.hpp"

class Engine : EngineObject
{
public:
    Engine();

    bool init(int width, int height, const char* title);
};

#endif