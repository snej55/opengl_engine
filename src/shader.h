#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include "engine.h"

#include <GLFW/glfw3.h>

class Shader : public EngineObject
{
public:
    Shader(EngineObject* parent);
    virtual ~Shader();

    virtual void free();

    virtual void loadFromFile(const char* fragPath, const char* vertPath);

    // virtual void loadDefault();

    [[nodiscard]] unsigned int getID() const;

protected:
    unsigned int m_ID;
};

#endif