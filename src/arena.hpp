/*
 * Basic memory manager
 * Process:
 * EngineObject* object;
 * // allocate memory for object
 * arena.alloc(object);
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

    ~Arena() override;
    void free();

    // allocate raw memory for new object
    // returns object != nullptr
    bool alloc(EngineObject*& object) const;
    // add object to arena and update object ID
    void addObject(EngineObject*& object);
    // remove object from arena
    void removeObject(unsigned int id);

private:
    std::vector<EngineObject*> m_objects{};
};

#endif