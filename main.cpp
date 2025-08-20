#include <iostream>
#include <vector>

#include "src/engine.hpp"

constexpr glm::vec3 lightPos { 0.0f, 5.0f, 30.f };

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

    engine.addTexture("tomato", "data/images/tomato.png");
    engine.addTexture("floor", "data/images/floor.png");
    engine.addModel("cube", "data/models/monkey.glb");
    engine.addModel("light", "data/models/big_torus.obj");

    const Model* cube { engine.getModel("cube") };
    const Model* light { engine.getModel("light") };

    // engine.enableWireframe();
    const std::vector<glm::vec3> spheres = {
        { 10.f, 4.f, 2.f },
        { 5.f, 10.f, -10.f },
        { -2.f, 5.f, 4.f },
        { -4.f, -3.f, 10.f }
    };

    engine.useShader("lightPBR");
    engine.setVec3("albedo", glm::vec3 { 0.5, 0.0f, 0.0f }, "lightPBR");
    engine.setFloat("ao", 1.0f, "lightPBR");

    while (!engine.getQuit()) {
        engine.enablePostProcessing();
        // clear screen
        engine.clear();

        engine.useShader("lightPBR");
        engine.setVec3("viewPos", engine.getCameraPosition(), "lightPBR");
        glm::mat4 model { 1.0f };
        // model = glm::rotate(model, glm::radians(static_cast<float>(glfwGetTime() * 20.f)), glm::vec3(0.7f, 1.0f, 0.2f));
        engine.setMat4("model", model, "lightPBR");
        engine.setMat4("view", engine.getViewMatrix(), "lightPBR");
        engine.setMat4("projection", engine.getProjectionMatrix(), "lightPBR");
        engine.setMat3("normalMat", engine.getNormalMatrix(model), "lightPBR");
        engine.setFloat("metallic", 0.6f, "lightPBR");
        engine.setFloat("roughness", 0.3f, "lightPBR");
        engine.setVec3("lightPos", lightPos, "lightPBR");
        engine.setVec3("lightColor", glm::vec3 { 1.0f }, "lightPBR");
        engine.setVec3("albedo", glm::vec3 { 0.5, 0.0f, 0.0f }, "lightPBR");

        cube->render(engine.getShader("lightPBR"));

        engine.setFloat("metallic", 1.0f, "lightPBR");
        engine.setFloat("roughness", 0.2, "lightPBR");
        engine.setVec3("albedo", glm::vec3 { 0.5, 0.1f, 0.7f }, "lightPBR");

        for (std::size_t i { 0 }; i < spheres.size(); ++i) {
            model = glm::mat4 { 1.0f };
            model = glm::scale(model, glm::vec3(0.2f));
            model = glm::translate(model, spheres[i]);
            engine.setMat4("model", model, "lightPBR");
            engine.setMat3("normalMat", engine.getNormalMatrix(model), "lightPBR");
            light->render(engine.getShader("lightPBR"));
        }

        engine.setFloat("metallic", 0.0f, "lightPBR");
        engine.setFloat("roughness", 1.0, "lightPBR");
        model = glm::mat4 { 1.0f };
        model = glm::scale(model, glm::vec3 { 0.2f });
        model = glm::translate(model, lightPos);
        engine.setMat4("model", model, "lightPBR");
        engine.setMat3("normalMat", engine.getNormalMatrix(model), "lightPBR");
        engine.setVec3("albedo", glm::vec3 { 1.0f, 1.0f, 1.0f }, "lightPBR");
        engine.setVec3("lightPos", engine.getCameraPosition(), "lightPBR");
        light->render(engine.getShader("lightPBR"));

        engine.disablePostProcessing();
        engine.renderPostProcessing();

        // update engine
        engine.update();
    }

    return 0;
}
