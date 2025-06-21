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

    engine.addTexture("tomato", "data/images/tomato.png");

    while (!engine.getQuit())
    {
        // clear screen
        engine.clear();

        // do rendering...

        // update engine
        engine.update();
    }

    return 0;
}
