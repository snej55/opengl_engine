//
// Created by Jens Kromdijk on 23/06/25.
//

#include "model.hpp"

#include <assimp/postprocess.h>

Model::Model(const std::string& name, EngineObject* parent)
    : EngineObject{("MODEL_" + name).c_str(), parent}, m_modelName{name}
{
}

void Model::render(const Shader* shader) const
{
    for (std::size_t i{0}; i < m_meshes.size(); ++i)
    {
        m_meshes[i].render(shader);
    }
}

void Model::loadModel(const std::string& path)
{
    Assimp::Importer importer;

    const aiScene* scene{
        importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace)
    };

    // error handling
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        // if it isn't zero
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
    std::cout << "Loaded model at `" << path << "`" << std::endl;
}

void Model::processNode(const aiNode* node, const aiScene* scene)
{
    for (std::size_t i {0}; i < node->mNumMeshes; ++i)
    {
        // node->mMeshes is a list of indices for scene->mMeshes
        const aiMesh* mesh{scene->mMeshes[node->mMeshes[i]]};

        m_meshes.push_back(processMesh(mesh, scene));
    }

    // repeat recursively for all children
    for (std::size_t i {0}; i < node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(const aiMesh* mesh, const aiScene* scene)
{
    std::vector<MeshN::Vertex> vertices{};
    std::vector<unsigned int> indices{};

    for (std::size_t i {0}; i < mesh->mNumVertices; ++i)
    {
        MeshN::Vertex vertex{};
        // get vertex positions
        const glm::vec3 pos {
            mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z
        };
        // same for normals
        const glm::vec3 normal {
            mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z
        };
        // texture coordinates if mesh has them
        if (mesh->mTextureCoords[0])
        {
            const glm::vec2 texCoords {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
            vertex.texCoords = texCoords;
        } else
        {
            vertex.texCoords = glm::vec2{0.0f, 0.0f};
        }
        vertex.position = pos;
        vertex.normal = normal;
    }

    return Mesh{vertices, indices};
}

ModelManager::ModelManager(EngineObject* parent)
    : EngineObject{"ModelManager", parent}
{
}

// load new model
void ModelManager::addModel(const std::string& name, const std::string& path, Arena* arena)
{
    // create new model and add it to arena
    Model* model {new Model{name, this}};
    arena->addObject(model);
    m_models.insert(std::pair{name, model});
    // load model
    getModel(name)->loadModel(path);
}

Model* ModelManager::getModel(const std::string& name) const
{
    if (modelExists(name))
    {
        return m_models.find(name)->second;
    }
    std::cout << "MODEL_MANAGER::GET_MODEL::ERROR: Model `" << name << "` does not exist!\n";
    return nullptr;
}

void ModelManager::renderModel(const Shader* shader, const std::string& name) const
{
    if (modelExists(name))
    {
        getModel(name)->render(shader);
    } else
    {
        std::cout << "MODEL_MANAGER::GET_MODEL::ERROR: Model `" << name << "` does not exist!\n";
    }
}

bool ModelManager::modelExists(const std::string& name) const
{
    return m_models.find(name) != m_models.end();
}