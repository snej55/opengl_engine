#include <iostream>

#include "src/engine.hpp"

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

    // // capture mouse
    engine.setCameraEnabled(true);
    //
    engine.addTexture("tomato", "data/images/tomato.png");
    engine.addModel("cube", "data/models/cube.obj");

    const Model* cube {engine.getModel("cube")};

    while (!engine.getQuit())
    {
        // clear screen
        engine.clear();

        engine.useShader("cube");
        auto model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(static_cast<float>(glfwGetTime() * 100.f)), glm::vec3(1.0f, 0.5f, 0.2f));
        engine.setMat4("model", model, "cube");
        engine.setMat4("view", engine.getViewMatrix(), "cube");
        engine.setMat4("projection", engine.getProjectionMatrix(), "cube");
        engine.setMat3("normalMat", engine.getNormalMatrix(model), "cube");
        engine.setVec3("albedo", glm::vec3{1.0f, 0.5f, 1.0f}, "cube");
        engine.setVec3("viewPos", engine.getCameraPosition(), "cube");
        cube->render(engine.getShader("cube"));

        // update engine
        engine.update();
    }

    return 0;
}
