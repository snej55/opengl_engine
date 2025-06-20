#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <map>

#include "engine_types.hpp"
#include "arena.hpp"

class Texture final : public EngineObject
{
public:
    explicit Texture(const std::string& name, EngineObject* manager = nullptr);

    void loadFromFile(const char* path);

    void activate(int slot) const;

    // get gl texture id
    [[nodiscard]] unsigned int getTex() const {return m_TEX;}

    // member getters
    [[nodiscard]] int getWidth() const {return m_width;}
    [[nodiscard]] int getHeight() const {return m_height;}
    [[nodiscard]] int getNumChannels() const {return m_numChannels;}

private:
    unsigned int m_TEX{0};
    int m_width{0};
    int m_height{0};
    int m_numChannels{0};
};

class TextureManager final : public EngineObject
{
public:
    explicit TextureManager(EngineObject* parent);

    // load new texture
    void addTexture(const char* path, const char* name, Arena* arena);

    [[nodiscard]] Texture* getTexture(const std::string& name) const;

    void activateTexture(const std::string& name, int slot) const;

    [[nodiscard]] bool textureExists(const std::string& name) const;

private:
    std::map<std::string, Texture*> m_textures{};
};

#endif