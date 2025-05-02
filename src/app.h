#ifndef APP_H
#define APP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.h"

class App
{
public:
    explicit App();
    ~App();

    bool init();
    void free();

    void clear();
    void tick();

    // creates a new window
    Window* createWindow() const;
    // allows app to manage window
    void addWindow(Window* window);

private:
    int m_width{0};
    int m_height{0};

    float m_deltaTime{1.0f};
    float m_lastTime{1.0f};
};

#endif
