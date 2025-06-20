#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

#include "texture.hpp"

Texture::Texture(const std::string& name, EngineObject* manager)
    : EngineObject{("TEXTURE_" + name).c_str(), manager}
{
}

void Texture::loadFromFile(const char* path)
{
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
        std::cout << "Failed to load texture: " << path << std::endl;
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
    }
    // free image data
    stbi_image_free(data);
    // set TEX ID
    m_TEX = tex;
}

// activate gl texture
void Texture::activate(const int slot) const
{
    // select texture slot
    glActiveTexture(GL_TEXTURE0 + slot);
    // activate texture
    glBindTexture(GL_TEXTURE_2D, m_TEX);
}
