#include "src/app.h"

#include <iostream>

int main()
{
    App app{};
    if (!app.init(640, 480, "OpenGL window"))
    {
        std::cout << "Failed to initialize!" << std::endl;
        return 1;
    }
    std::cout << "Initialized!\n";
    return 0;
}
