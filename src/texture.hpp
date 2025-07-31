#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <map>

#include "engine_types.hpp"
#include "arena.hpp"

// Basic texture wrapper class.
class Texture final : public EngineObject
{
public:
    explicit Texture(const std::string& name, EngineObject* manager = nullptr);

    // Loads texture data from path using stbi_image.h.
    bool loadFromFile(const char* path);

    // Activates texture at slot (GL_TEXTURE0 + slot).
    void activate(int slot) const;

    // Get GL_TEXTURE ID.
    [[nodiscard]] unsigned int getTex() const {return m_TEX;}

    // Get width of texture.
    [[nodiscard]] int getWidth() const {return m_width;}
    // Get height of texture.
    [[nodiscard]] int getHeight() const {return m_height;}
    // Get number of color channels of texture.
    [[nodiscard]] int getNumChannels() const {return m_numChannels;}

private:
    unsigned int m_TEX{0};
    int m_width{0};
    int m_height{0};
    int m_numChannels{0};
};

// Manager for loading and accessing textures.
class TextureManager final : public EngineObject
{
public:
    explicit TextureManager(EngineObject* parent);

    // load new texture
    void addTexture(const char* path, const char* name, Arena* arena);

    // Gets texture with name <name>.
    //
    // Returns nullptr if texture with name <name> does not exist.
    [[nodiscard]] Texture* getTexture(const std::string& name) const;

    // Activates texture <name> if texture exists at slot (GL_TEXTURE0 + slot).
    void activateTexture(const std::string& name, int slot) const;

    // Checks if texture <name> exists.
    [[nodiscard]] bool textureExists(const std::string& name) const;

private:
    std::map<std::string, Texture*> m_textures{};
};

#endif
