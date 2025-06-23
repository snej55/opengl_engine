#include "mesh.hpp"

Mesh::Mesh(const std::vector<MeshN::Vertex>& vertices, const std::vector<unsigned int>& indices, EngineObject* parent)
 : EngineObject{"Mesh"}, m_vertices{vertices}, m_indices{indices}
{

}