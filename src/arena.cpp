#include "arena.hpp"

Arena::Arena(EngineObject* engine)
 : EngineObject{"Arena", engine}
{
}

void Arena::getObject(EngineObject*& object) const
{
    std::allocator<EngineObject> allocator;
    void* ptr {allocator.allocate(1)};
    object = static_cast<EngineObject*>(ptr);
}

void Arena::addObject(EngineObject*& object)
{
    object->setID(m_objects.size());
    m_objects.emplace_back(object);
}

void Arena::removeObject(unsigned int id)
{
    // move object to back
    std::swap(m_objects[id], m_objects.back());
    // update id of other moved object
    m_objects[id]->setID(id);
    
    // free object memory
    delete m_objects.back();
    m_objects.pop_back();
}