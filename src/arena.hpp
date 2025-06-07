/*
 * Basic memory manager
 * Process:
 * EngineObject* object;
 * // allocate memory for object
 * arena.getObject(object);
 * // setup object
 * *object = EngineObject{"foo", nullptr};
 * arena.addObject(object);
 */

#ifndef ARENA_H
#define ARENA_H

#include <vector>

#include "engine_types.hpp"

class Arena : public EngineObject
{
public:
    // set up arena
    explicit Arena(EngineObject* engine);
    ~Arena();

    // allocate raw memory for new object
    void getObject(EngineObject*& object) const;
    // add object to arena and update object ID
    void addObject(EngineObject*& object);
    // remove object from arena
    void removeObject(unsigned int id);

private:
    std::vector<EngineObject*> m_objects{};
};

#endif