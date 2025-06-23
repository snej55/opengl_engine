#ifndef MESH_H
#define MESH_H

#include "engine_types.hpp"
#include "shader.h"

#include <vector>

#include <glm/glm.hpp>

namespace MeshN
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };
}

class Mesh final : public EngineObject
{
public:
    Mesh(const std::vector<MeshN::Vertex>& vertices, const std::vector<unsigned int>& indices, EngineObject* parent);

    void render(const Shader* shader) const;

    [[nodiscard]] const std::vector<MeshN::Vertex>& getVertices() const {return m_vertices;}
    [[nodiscard]] const std::vector<unsigned int>& getIncides() const {return m_indices;}

private:
    std::vector<MeshN::Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    unsigned int m_VAO{};
    unsigned int m_VBO{};
    unsigned int m_EBO{};

    void setupMesh();
};

#endif // MESH_H