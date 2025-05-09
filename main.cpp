#include "src/app.h"
#include "src/logging.h"

#include <iostream>

int main()
{
    // Logger logger{".logs"};
    App app{};
    if (!app.init(640, 480, "OpenGL window"))
    {
        std::cout << "Failed to initialize!" << std::endl;
        return 1;
    }
    std::cout << "Initialized!\n";

    Window* window{app.getWindow()};
    while (!window->getShouldClose())
    {
        window->update();
        window->clear();

        window->tick();
    }
    return 0;
}
