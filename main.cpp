#include <iostream>
#include <vector>

#include <glm/ext/matrix_transform.hpp>
#include "src/engine.hpp"
#include "src/texture.hpp"
#include "src/util.hpp"

int main()
{
    // initialize engine
    Engine engine{};
    if (!engine.init(640, 480, "OpenGL Window"))
    {
        std::cout << "Failed to initialize engine!\n";
        return 1;
    }

    std::cout << "Initialized engine!\n";
    engine.setCameraEnabled(true);

    // use only gltf files for now
    engine.addModel("cube", "data/models/monkey.glb");
    engine.addModel("light", "data/models/rusty_sphere.glb");

    const Model* light{engine.getModel("light")};

    // engine.enableWireframe();
    const std::vector<glm::vec3> spheres = {{1.f, 4.f, 2.f}, {5.f, 1.f, -5.f}, {-2.f, 5.f, 4.f}, {-4.f, -3.f, 10.f}};

    engine.useShader("lightPBR");
    engine.setVec3("albedo", glm::vec3{0.5, 0.0f, 0.0f}, "lightPBR");
    engine.setFloat("ao", 1.0f, "lightPBR");

    glm::vec3 lightPos{1.0f, 1.0f, 1.0f};

    // hdr irradiance map
    bool success;
    unsigned int hdrMap{TextureN::loadHDRMap("data/skyboxes/golden_gate.hdr", &success)};
    if (!success)
    {
        Util::beginError();
        std::cout << "ERROR: Failed to load HDR map!" << std::endl;
        Util::endError();
    }

    while (!engine.getQuit())
    {
        engine.enablePostProcessing();
        // clear screen
        engine.clear();

        // rotate light pos
        lightPos = glm::vec3{glm::sin(engine.getTime()) * 5.0f, 0.0f, 0.0f};
        engine.useShader("lightPBR");
        engine.setVec3("viewPos", engine.getCameraPosition(), "lightPBR");
        engine.setMat4("view", engine.getViewMatrix(), "lightPBR");
        engine.setMat4("projection", engine.getProjectionMatrix(), "lightPBR");
        engine.setVec3("lightColor", glm::vec3{1.0f}, "lightPBR");

        engine.setVec3("albedo", glm::vec3{0.5, 0.1f, 0.7f}, "lightPBR");

        engine.setFloat("metallic", 0.0f, "lightPBR");
        engine.setFloat("roughness", 1.0, "lightPBR");
        glm::mat4 model{1.0f};
        model = glm::scale(model, glm::vec3{0.2f});
        model = glm::translate(model, lightPos);
        engine.setMat4("model", model, "lightPBR");
        engine.setMat3("normalMat", engine.getNormalMatrix(model), "lightPBR");
        engine.setVec3("albedo", glm::vec3{1.0f, 1.0f, 1.0f}, "lightPBR");
        engine.setVec3("lightPos", engine.getCameraPosition(), "lightPBR");
        light->render(engine.getShader("lightPBR"));

        engine.useShader("texturePBR");
        engine.setVec3("lightPos", lightPos, "texturePBR");
        engine.setVec3("lightColor", glm::vec3{1.0f}, "texturePBR");
        engine.setVec3("viewPos", engine.getCameraPosition(), "texturePBR");

        for (std::size_t i{0}; i < spheres.size(); ++i)
        {
            model = glm::mat4{1.0f};
            model = glm::scale(model, glm::vec3(0.2f));
            model = glm::translate(model, spheres[i]);
            // model = glm::rotate(model, engine.getTime() * 0.6f, {1.0f, 0.7f, 0.3f});
            engine.setMat4("model", model, "texturePBR");
            engine.setMat4("view", engine.getViewMatrix(), "texturePBR");
            engine.setMat4("projection", engine.getProjectionMatrix(), "texturePBR");
            engine.setMat3("normalMat", engine.getNormalMatrix(model), "texturePBR");
            light->renderPBR(engine.getShader("texturePBR"));
        }

        engine.disablePostProcessing();
        engine.renderPostProcessing();

        // update engine
        engine.displayFrameTime();
        engine.update();
    }

    return 0;
}
