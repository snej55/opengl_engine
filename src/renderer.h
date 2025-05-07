#ifndef RENDERER_H
#define RENDERER_H

#include "engine.h"
#include "queue.h"

class Renderer : public EngineObject
{
public:
    Renderer(EngineObject* parent);
    virtual ~Renderer();

    virtual bool init();
    virtual void free();

    virtual bool createRenderQueue();

    // clears the queue and renders all the objects from it
    virtual void renderQueue();
    virtual void drawObject(const Shader* shader, void* object);
    virtual void addObject();

protected:
    RenderQueue* m_renderQueue{nullptr};
};

#endif