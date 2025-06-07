#ifndef ENGINE_H
#define ENGINE_H

#include <string>

#include "engine_types.hpp"

class Engine
{
public:
    Engine(int width, int height, const std::string& title);

    bool init();
};

#endif