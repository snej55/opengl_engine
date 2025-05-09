#ifndef OBJECTS_H
#define OBJECTS_H

#include "engine.h"

class Object : EngineObject
{
public:
    Object(EngineObject* parent);
    virtual ~Object();

    virtual void init();
    virtual void free();
};

#endif