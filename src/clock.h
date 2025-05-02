#ifndef CLOCK_H
#define CLOCK_H

#include <GLFW/glfw3.h>

class Clock
{
public:
    Clock(void* parent);

    void tick();

    float getDeltaTime() const;
    void* getParent() const;

private:
    float m_deltaTime{1.0f};
    float m_lastTime{1.0f};

    void* m_parent{nullptr};
};

#endif
