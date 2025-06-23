#ifndef MODEL_H
#define MODEL_H

#include "mesh.hpp"
#include "engine_types.hpp"

#include <string>
#include <vector>

class Model final : public EngineObject
{
public:
    explicit Model(EngineObject* parent);
};

#endif