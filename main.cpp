#include <iostream>
#include <vector>

#include "glm/ext/matrix_transform.hpp"
#include "src/engine.hpp"

int main()
{
    // initialize engine
    Engine engine {};
    if (!engine.init(640, 480, "OpenGL Window")) {
        std::cout << "Failed to initialize engine!\n";
        return 1;
    }
    std::cout << "Initialized engine!\n";
    engine.setCameraEnabled(true);

    engine.addTexture("albedo", "data/materials/rustediron/rustediron2_basecolor.png");
    engine.addTexture("ao", "data/materials/rustediron/rustediron2_ao.png");
    engine.addTexture("metallic", "data/materials/rustediron/rustediron2_metallic.png");
    engine.addTexture("normal", "data/materials/rustediron/rustediron2_normal.png");
    engine.addTexture("roughness", "data/materials/rustediron/rustediron2_roughness.png");

    engine.addModel("cube", "data/models/monkey.glb");
    engine.addModel("light", "data/models/textured_sphere.obj");

    const Model* cube { engine.getModel("cube") };
    const Model* light { engine.getModel("light") };

    // engine.enableWireframe();
    const std::vector<glm::vec3> spheres = {
        { 1.f, 4.f, 2.f },
        { 5.f, 1.f, -5.f },
        { -2.f, 5.f, 4.f },
        { -4.f, -3.f, 10.f }
    };

    engine.useShader("lightPBR");
    engine.setVec3("albedo", glm::vec3 { 0.5, 0.0f, 0.0f }, "lightPBR");
    engine.setFloat("ao", 1.0f, "lightPBR");

    engine.useShader("texturePBR");
    engine.activateTexture("albedo", 0);
    engine.setInt("albedoMap", 0, "texturePBR");
    engine.activateTexture("ao", 1);
    engine.setInt("aoMap", 1, "texturePBR");
    engine.activateTexture("metallic", 2);
    engine.setInt("metallicMap", 2, "texturePBR");
    engine.activateTexture("normal", 3);
    engine.setInt("normalMap", 3, "texturePBR");
    engine.activateTexture("roughness", 4);
    engine.setInt("roughnessMap", 4, "texturePBR");

    glm::vec3 lightPos { 1.0f, 1.0f, 1.0f };

    while (!engine.getQuit()) {
        engine.enablePostProcessing();
        // clear screen
        engine.clear();

        // rotate light pos
        lightPos = glm::vec3{glm::sin(engine.getTime()) * 5.0f, 0.0f, 0.0f};
        engine.useShader("lightPBR");
        engine.setVec3("viewPos", engine.getCameraPosition(), "lightPBR");
        engine.setMat4("view", engine.getViewMatrix(), "lightPBR");
        engine.setMat4("projection", engine.getProjectionMatrix(), "lightPBR");
        engine.setVec3("lightPos", lightPos, "lightPBR");
        engine.setVec3("lightColor", glm::vec3 { 1.0f }, "lightPBR");

        engine.setFloat("metallic", 1.0f, "lightPBR");
        engine.setFloat("roughness", 0.2, "lightPBR");
        engine.setVec3("albedo", glm::vec3 { 0.5, 0.1f, 0.7f }, "lightPBR");

        engine.setFloat("metallic", 0.0f, "lightPBR");
        engine.setFloat("roughness", 1.0, "lightPBR");
        glm::mat4 model { 1.0f };
        model = glm::scale(model, glm::vec3 { 0.2f });
        model = glm::translate(model, lightPos);
        engine.setMat4("model", model, "lightPBR");
        engine.setMat3("normalMat", engine.getNormalMatrix(model), "lightPBR");
        engine.setVec3("albedo", glm::vec3 { 1.0f, 1.0f, 1.0f }, "lightPBR");
        engine.setVec3("lightPos", engine.getCameraPosition(), "lightPBR");
        light->render(engine.getShader("lightPBR"));

        engine.useShader("texturePBR");
        engine.setVec3("lightPos", lightPos, "texturePBR");
        engine.setVec3("lightColor", glm::vec3 { 1.0f }, "texturePBR");
        engine.setVec3("viewPos", engine.getCameraPosition(), "texturePBR");
        engine.activateTexture("albedo", 0);
        engine.setInt("albedoMap", 0, "texturePBR");
        engine.activateTexture("ao", 1);
        engine.setInt("aoMap", 1, "texturePBR");
        engine.activateTexture("metallic", 2);
        engine.setInt("metallicMap", 2, "texturePBR");
        engine.activateTexture("normal", 3);
        engine.setInt("normalMap", 3, "texturePBR");
        engine.activateTexture("roughness", 4);
        engine.setInt("roughnessMap", 4, "texturePBR");

        for (std::size_t i { 0 }; i < spheres.size(); ++i) {
            model = glm::mat4 { 1.0f };
            model = glm::scale(model, glm::vec3(0.2f));
            model = glm::translate(model, spheres[i]);
            // model = glm::rotate(model, engine.getTime() * 0.6f, {1.0f, 0.7f, 0.3f});
            engine.setMat4("model", model, "texturePBR");
            engine.setMat4("view", engine.getViewMatrix(), "texturePBR");
            engine.setMat4("projection", engine.getProjectionMatrix(), "texturePBR");
            engine.setMat3("normalMat", engine.getNormalMatrix(model), "texturePBR");
            light->render(engine.getShader("texturePBR"));
        }

        // engine.drawTexture("albedo", {0.f, 0.f, 1.f, 1.f});
        // engine.drawRect({0.f, 0.f, 1.f, 1.f}, {255, 0, 0});

        engine.disablePostProcessing();
        engine.renderPostProcessing();

        // update engine
        engine.update();
    }

    return 0;
}
