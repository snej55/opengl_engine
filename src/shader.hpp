#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>

#include <string>
#include <map>

#include "engine_types.hpp"

class Shader final : public EngineObject
{
public:
    explicit Shader(const std::string& name);

    void loadFromFile(const char* fragPath, const char* vertPath);

    void use() const;

    [[nodiscard]] unsigned int getShaderID() const;

    // shader uniforms
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

    // vectorz
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec2(const std::string& name, float x, float y) const;

    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;

    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setVec4(const std::string& name, float x, float y, float z, float w) const;

    // matrices
    void setMat2(const std::string& name, const glm::mat2& value) const;
    void setMat3(const std::string& name, const glm::mat3& value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;

protected:
    unsigned int m_ID{0};
};

class ShaderManager final : public EngineObject
{
public:
    explicit ShaderManager(EngineObject* parent);

    // load new shader
    void addShader(const std::string& name, const char* fragPath, const char* vertPath);
    [[nodiscard]] Shader* getShader(const std::string& name);

    void useShader(const std::string& name);

    [[nodiscard]] bool shaderExists(const std::string& name) const;

private:
    std::map<std::string, Shader> m_shaders{};
};

#endif
