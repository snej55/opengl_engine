#ifndef WINDOW_H
#define WINDOW_H

#include "engine.h"

class Window : public EngineObject
{
public:
    Window();

    bool init(const int width, const int height);
    void free();

private:
    int m_width{0};
    int m_height{0};
};

#endif