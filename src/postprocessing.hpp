#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

#include "engine_types.hpp"
#include "shader.hpp"

class PostProcessor final : public EngineObject
{
public:
    explicit PostProcessor(EngineObject* parent);

    ~PostProcessor() override;

    // free resources
    void free();
    // check framebuffer
    [[nodiscard]] bool check() const;

    // initialize framebuffer and texture
    void init(int width, int height);
    // regenerate framebuffer for framebuffer_size_callback()
    void generate(int width, int height);

    // render framebuffer to screen
    void render(const Shader* screenShader) const;

    // bind framebuffer
    void enable() const;
    // unbind framebuffer
    void disable() const;

private:
    // framebuffer dimensions
    int m_width{0};
    int m_height{0};

    // framebuffer
    unsigned int m_FBO{};
    unsigned int m_RBO{};
    unsigned int m_TEX{};

    // simple quad
    unsigned int m_VAO{};
    unsigned int m_VBO{};

    void generateFramebuffer();
    void generateFramebufferTexture();
    void generateRenderbuffer();
    void generateQuad();
};

#endif