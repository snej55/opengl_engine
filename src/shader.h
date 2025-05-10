#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

#include "engine.h"

class Shader : public EngineObject
{
public:
    Shader(EngineObject* parent);
    virtual ~Shader();

    virtual void free();

    virtual void loadFromFile(const char* fragPath, const char* vertPath);

    virtual void use() const;

    [[nodiscard]] unsigned int getID() const;

    // shader uniforms
    void setBool(const std::string& name, const bool value) const;
    void setInt(const std::string& name, const int value) const;
    void setFloat(const std::string& name, const float value) const;

    // vectorz
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec2(const std::string& name, const float x, const float y) const;

    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, const float x, const float y, const float z) const;

    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setVec4(const std::string& name, const float x, const float y, const float z, const float w) const;

    // matrices
    void setMat2(const std::string& name, const glm::mat2& value) const;
    void setMat3(const std::string& name, const glm::mat3& value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;

protected:
    unsigned int m_ID;
};

#endif