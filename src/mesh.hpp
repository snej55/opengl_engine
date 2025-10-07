#ifndef MESH_H
#define MESH_H

#include "shader.hpp"

#include <vector>

#include <glm/glm.hpp>

namespace MeshN
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
        glm::vec3 tangent; // TBN matrix
        glm::vec3 biTangent; // "" ""
    };

    struct Texture
    {
        unsigned int id;
        std::string type;
    };
}

class Mesh
{
public:
    Mesh(const std::vector<MeshN::Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<MeshN::Texture>& textures);

    void render(const Shader* shader) const;

    void free() const;

    [[nodiscard]] const std::vector<MeshN::Vertex>& getVertices() const {return m_vertices;}
    [[nodiscard]] const std::vector<unsigned int>& getIndices() const {return m_indices;}

private:
    std::vector<MeshN::Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<MeshN::Texture> m_textures;

    unsigned int m_VAO{};
    unsigned int m_VBO{};
    unsigned int m_EBO{};

    void setupMesh();
};

#endif // MESH_H
