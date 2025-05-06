#include "renderer.h"

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

void Renderer::free()
{
    EngineObject::free();
}

void Renderer::renderQueue()
{
    
}