#ifndef QUEUE_H
#define QUEUE_H

#include "engine.h"
#include "shader.h"

#include <vector>
#include <utility> // for std::pair

class RenderQueue : EngineObject
{
public:
    RenderQueue(EngineObject* parent);
};

#endif