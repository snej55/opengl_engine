#ifndef TEXTURE_H
#define TEXTURE_H

#include "engine_types.hpp"

#include <string>

class Texture : EngineObject
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

#endif