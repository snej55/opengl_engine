#ifndef MODEL_H
#define MODEL_H

#include "mesh.hpp"
#include "shader.hpp"
#include "engine_types.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <string>
#include <vector>

class Model final : public EngineObject
{
public:
    explicit Model(const std::string& name, EngineObject* parent);

    void loadModel(const std::string& path);
    
    void render(const Shader* shader) const;

private:
    std::vector<Mesh> m_meshes{};
    std::string directory{};
    std::string m_modelName;

    void processNode(const aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};

#endif