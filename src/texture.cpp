#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

#include "texture.hpp"
#include "util.hpp"

Texture::Texture(const std::string& name, EngineObject* manager)
    : EngineObject{("TEXTURE " + name).c_str(), manager}
{
}

bool Texture::loadFromFile(const char* path)
{
    // check if texture exists
    if (!Util::fileExists(path))
    {
        Util::beginError();
        std::cout << "TEXTURE::LOAD_FROM_FILE::ERROR: Failed to load texture from path `" << path << "` - texture does not exist";
        Util::endError();
        return false;
    }

    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    // tex wrap params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // tex filtering params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // load image
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data {stbi_load(path, &m_width, &m_height, &m_numChannels, 0)};
    // check if image was successfully loaded
    if (!data)
    {
        std::cout << "Failed to load texture: `" << path << "`" << std::endl;
        stbi_image_free(data);
        m_TEX = 0;
        return false;
    } else
    {
        // get internal format for tex. data
        GLenum format {0};
        switch (m_numChannels)
        {
            case 1:
                format = GL_RED;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
            default:
                break;
        }
        // load opengl texture
        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format), m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        std::cout << "Successfully loaded texture from `" << path << "`\n";
    }
    // free image data
    stbi_image_free(data);
    // set TEX ID
    m_TEX = tex;

    return true;
}

// activate gl texture
void Texture::activate(const int slot) const
{
    // select texture slot
    glActiveTexture(GL_TEXTURE0 + slot);
    // activate texture
    glBindTexture(GL_TEXTURE_2D, m_TEX);
}

// ------- Texture Manager ------- //
TextureManager::TextureManager(EngineObject* parent)
    : EngineObject{"TextureManager", parent}
{
}

// load new texture
void TextureManager::addTexture(const char* path, const char* name, Arena* arena)
{
    // create new texture and add to arena
    Texture* texture {new Texture{name, this}};
    arena->addObject(texture);

    // load texture
    if (!texture->loadFromFile(path))
    {
        Util::beginError();
        std::cout << "TEXTURE_MANAGER::ADD_TEXTURE::ERROR: Failed to add texture `" << name << "`!";
        Util::endError();
    } else {
        m_textures.insert(std::pair{std::string{name}, texture});
    }
}

Texture* TextureManager::getTexture(const std::string& name) const
{
    if (textureExists(name))
    {
        return m_textures.find(name)->second;
    }
    std::cout << "TEXTURE_MANAGER::GET_TEXTURE::ERROR: Texture `" << name << "' does not exist!\n";
    return nullptr;
}

void TextureManager::activateTexture(const std::string& name, int slot) const
{
    if (textureExists(name))
    {
        getTexture(name)->activate(slot);
    } else
    {
        std::cout << "TEXTURE_MANAGER::ACTIVATE_TEXTURE::ERROR: Texture `" << name << "' does not exist!\n";
    }
}

bool TextureManager::textureExists(const std::string& name) const
{
    return m_textures.find(name) != m_textures.end();
}
