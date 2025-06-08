//
// Created by Jens Kromdijk on 08/06/25.
//

#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <GLFW/glfw3.h>

#include "engine_types.hpp"

class IOHandler : public EngineObject
{
public:
    explicit IOHandler(EngineObject* engine, GLFWwindow* window);

    // check whether we need to quit
    void update();

    [[nodiscard]] bool getPressed(int key) const;
    [[nodiscard]] bool getQuit() const {return m_quit;}

private:
    // for glfwGetKey
    GLFWwindow* m_window;

    bool m_quit{false};

};

#endif