#ifndef MODEL_H
#define MODEL_H

#include "mesh.hpp"
#include "shader.hpp"
#include "engine_types.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <string>
#include <vector>
#include <map>

class Model final : public EngineObject
{
public:
    explicit Model(const std::string& name, EngineObject* parent);

    bool loadModel(const std::string& path);

    void render(const Shader* shader) const;

private:
    std::vector<Mesh> m_meshes{};
    std::string directory{};
    std::string m_modelName;

    void processNode(const aiNode* node, const aiScene* scene);
    Mesh processMesh(const aiMesh* mesh, const aiScene* scene);
};

class ModelManager final : public EngineObject
{
public:
    explicit ModelManager(EngineObject* parent);

    // load new model
    void addModel(const std::string& name, const std::string& path, Arena* arena);

    [[nodiscard]] Model* getModel(const std::string& name) const;

    void renderModel(const Shader* shader, const std::string& name) const;

    [[nodiscard]] bool modelExists(const std::string& name) const;

private:
    std::map<std::string, Model*> m_models{};
};

#endif
