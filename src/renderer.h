#ifndef RENDERER_H
#define RENDERER_H

#include "engine.h"

class Renderer : public EngineObject
{
public:
    Renderer(EngineObject* parent);
    virtual ~Renderer();

    virtual void free();

    virtual void renderQueue();
    virtual void drawObject();
    virtual void addObject();
};

#endif