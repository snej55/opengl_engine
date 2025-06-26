#include <iostream>

#include "src/engine.hpp"

constexpr glm::vec3 lightPos {0.0f, 5.0f, 30.f};

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

    engine.addTexture("tomato", "data/images/tomato.png");
    engine.addTexture("floor", "data/images/floor.png");
    engine.addModel("cube", "data/models/monkey.glb");
    engine.addModel("light", "data/models/sphere.obj");

    const Model* cube {engine.getModel("cube")};
    const Model* light {engine.getModel("light")};

    while (!engine.getQuit())
    {
        // clear screen
        engine.clear();

        engine.useShader("cube");
        glm::mat4 model {1.0f};
        // model = glm::rotate(model, glm::radians(static_cast<float>(glfwGetTime() * 20.f)), glm::vec3(0.7f, 1.0f, 0.2f));
        engine.setMat4("model", model, "lightPBR");
        engine.setMat4("view", engine.getViewMatrix(), "lightPBR");
        engine.setMat4("projection", engine.getProjectionMatrix(), "lightPBR");
        engine.setMat3("normalMat", engine.getNormalMatrix(model), "lightPBR");
        engine.setVec3("albedo", glm::vec3{1.0f, 0.5f, 1.0f}, "lightPBR");
        engine.setVec3("viewPos", lightPos, "lightPBR");
        cube->render(engine.getShader("cube"));
        model = glm::scale(model, glm::vec3{0.2f});
        model = glm::translate(model, lightPos);
        engine.setMat4("model", model, "lightPBR");
        engine.setVec3("viewPos", engine.getCameraPosition(), "lightPBR");
        engine.setVec3("albedo", glm::vec3{1.0f}, "lightPBR");
        light->render(engine.getShader("cube"));

        // update engine
        engine.update();
    }

    return 0;
}
