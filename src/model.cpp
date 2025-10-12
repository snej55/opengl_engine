//
// Created by Jens Kromdijk on 23/06/25.
//

#include "model.hpp"
#include "assimp/material.h"
#include "mesh.hpp"
#include "util.hpp"
#include "texture.hpp"

#include <assimp/postprocess.h>

#include <string>
#include <sstream>

Model::Model(const std::string& name, EngineObject* parent)
    : EngineObject{("MODEL " + name).c_str(), parent}, m_modelName{name}
{
}

Model::~Model()
{
    for (std::size_t i{0}; i < m_meshes.size(); ++i)
    {
        m_meshes[i].free();
    }
}

void Model::render(const Shader* shader) const
{
    for (std::size_t i{0}; i < m_meshes.size(); ++i)
    {
        m_meshes[i].render(shader);
    }
}

bool Model::loadModel(const std::string& path)
{
    // check if model already exists
    if (!Util::fileExists(path))
    {
        Util::beginError();
        std::cout << "MODEL::LOAD_MODEL::ERROR: Failed to load model from `" << path << "` - file does not exist!";
        Util::endError();
        return false;
    }

    Assimp::Importer importer;

    const aiScene *scene{importer.ReadFile(
        path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate |
                  aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
                  aiProcess_CalcTangentSpace)};

    // error handling
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        // if it isn't zero
        Util::beginError();
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString();
        Util::endError();
        return false;
    }

    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);

    // overkill log
    int numVertices{};
    for (const Mesh& mesh : m_meshes)
    {
        numVertices += static_cast<int>(mesh.getVertices().size());
    }

    // just some useful info :)
    unsigned long vertSize {sizeof(MeshN::Vertex) * numVertices};
    std::stringstream ss{};
    if (vertSize > 1000 * 1000)
    {
        vertSize = vertSize / 1000 / 1000;
        ss << vertSize << " MB";
    } else if (vertSize > 1000)
    {
        vertSize = vertSize / 1000;
        ss << vertSize << " KB";
    } else
    {
        ss << vertSize << " B";
    }

    const std::string size = ss.str();
    std::cout << "Loaded model at `" << path << "`, " << numVertices << " vertices (" << size << ")" << std::endl;

    return true;
}

void Model::processNode(const aiNode* node, const aiScene* scene)
{
    for (std::size_t i {0}; i < node->mNumMeshes; ++i)
    {
        // node->mMeshes is a list of indices for scene->mMeshes
        const aiMesh* mesh{scene->mMeshes[node->mMeshes[i]]};

        m_meshes.emplace_back(processMesh(mesh, scene));
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
    std::vector<MeshN::Texture> textures{};

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

        // calculate tangent and bitangent for normal mapping
        const glm::vec3 tangent {
            mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z
        };

        const glm::vec3 biTangent {
            mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z
        };

        vertex.position = pos;
        vertex.normal = normal;
        vertex.tangent = tangent;
        vertex.biTangent = biTangent;
        vertices.push_back(vertex);
    }

    // indices
    for (unsigned int i{0}; i < mesh->mNumFaces; ++i)
    {
        const aiFace& face{mesh->mFaces[i]};
        // each face usually has like 3 indices or something
        for (unsigned int j{0}; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // materials
    aiMaterial* material{scene->mMaterials[mesh->mMaterialIndex]};

    std::vector<MeshN::Texture> aoMaps{loadMaterialTextures(material, aiTextureType_LIGHTMAP, MeshN::TEXTURE_AO)};
    textures.insert(textures.end(), aoMaps.begin(), aoMaps.end());
    std::vector<MeshN::Texture> albedoMaps{loadMaterialTextures(material, aiTextureType_BASE_COLOR, MeshN::TEXTURE_ALBEDO)};
    textures.insert(textures.end(), albedoMaps.begin(), albedoMaps.end());
    std::vector<MeshN::Texture> metallicMaps{loadMaterialTextures(material, aiTextureType_METALNESS, MeshN::TEXTURE_METALLIC)};
    textures.insert(textures.end(), metallicMaps.begin(), metallicMaps.end());
    std::vector<MeshN::Texture> roughnessMaps{loadMaterialTextures(material, aiTextureType_GLTF_METALLIC_ROUGHNESS, MeshN::TEXTURE_ROUGHNESS)};
    textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());
    std::vector<MeshN::Texture> normalMaps{loadMaterialTextures(material, aiTextureType_NORMALS, MeshN::TEXTURE_NORMAL)};
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    return Mesh{vertices, indices, {}};
}

std::vector<MeshN::Texture> Model::loadMaterialTextures(const aiMaterial* mat, const aiTextureType type, const MeshN::TextureType typeName)
{
    std::vector<MeshN::Texture> textures{};
    for (unsigned int i{0}; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->Get(AI_MATKEY_TEXTURE(type, i), str);
        bool skip{false};

        // check if we haven't already loaded this texture
        for (unsigned int j{0}; j < m_loadedTextures.size(); ++j)
        {
            // compare
            if (std::strcmp(m_loadedTextures[j].path, str.C_Str()) == 0)
            {
                // we found something
                // push back THAT texture instead
                textures.push_back(m_loadedTextures[j]);
                skip = true;
                break;
            }
        }

        // if we haven't already loaded the texture
        if (!skip)
        {
            // get texture path
            std::string filename{directory + '/' + str.C_Str()};
            // load texture id
            bool success;
            const unsigned int texID {TextureN::loadFromFile(filename.c_str(), nullptr, nullptr, nullptr, &success)};
            if (!success) // check if texture was loaded successfully (don't add bad texture)
            {
                continue;
            }

            // create texture object
            MeshN::Texture texture {
                texID, // texture id
                typeName, // MeshN::TextureType
                filename.c_str() // texture path
            };

            m_loadedTextures.emplace_back(texture);
            textures.push_back(texture);
        }
    }

    return textures;
}

// -------------- Model Manager -------------- //
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

    // add model
    if (!model->loadModel(path))
    {
        Util::beginError();
        std::cout << "MODEL_MANAGER::ADD_MODEL::ERROR: Failed to add model `" << name << "`";
        Util::endError();
    } else {
        m_models.insert(std::pair{name, model});
    }
}

Model* ModelManager::getModel(const std::string& name) const
{
    if (modelExists(name))
    {
        return m_models.find(name)->second;
    }
    Util::beginError();
    std::cout << "MODEL_MANAGER::GET_MODEL::ERROR: Model `" << name << "` does not exist!";
    Util::endError();
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
