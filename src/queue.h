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

    void addObject(const std::pair<Shader*, void*>&& object);
    const std::vector<std::pair<Shader*, void*>>& getObjects() const;

    // gets last element from the stack
    std::pair<Shader*, void*> getNext() const;
    void popBack();

private:
    std::vector<std::pair<Shader*, void*>> m_objects{};
};

#endif