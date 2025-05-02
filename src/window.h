#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

class Window
{
public:
    Window(const int width, const int height, void* parent);
    virtual ~Window();

    virtual void tick();
    virtual void clear();
    virtual void free();

private:
    int m_width{0};
    int m_height{0};
    void* parent;
};

#endif
