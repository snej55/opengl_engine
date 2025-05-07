#include "renderer.h"

#include <cassert>
#include <utility>
#include <iostream>

Renderer::Renderer(EngineObject* parent)
 : EngineObject{parent, "Renderer"}
{

}

Renderer::~Renderer()
{
    if (!m_freed)
    {
        free();
    }
}

bool Renderer::init()
{
    if (!createRenderQueue())
    {
        std::cout << "RENDERER::INIT::ERROR: Error creating RenderQueue!" << std::endl;
        return false;
    }
    return true;
}

void Renderer::free()
{
    EngineObject::free();
}

bool Renderer::createRenderQueue()
{
    if (m_renderQueue == nullptr)
    {
        m_renderQueue = new RenderQueue{this};
        return true;
    }
    std::cout << "RENDERER::CREATE_RENDER_QUEUE::ERROR: Error, RenderQueue already exists!" << std::endl;
    return false;
}

void Renderer::renderQueue()
{
    // render queue is required
    assert(m_renderQueue != nullptr);

    // object to render
    std::pair<Shader*, void*> item;

    // loop until all objects have been rendered from the stack
    do {
        // get item from top of the stack
        item = m_renderQueue->getNext();
        // render the object
        drawObject(item.first, item.second);

        // pop from stack
        m_renderQueue->popBack();
    } while (!m_renderQueue->isEmpty());
}

void Renderer::drawObject(const Shader* shader, void* object)
{
    return;
}