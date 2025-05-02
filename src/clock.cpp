#include "clock.h"

Clock::Clock(void* parent)
 : m_parent{parent}
{
}

void Clock::tick()
{
    const float currentFrame{static_cast<float>(glfwGetTime())};
    m_deltaTime = currentFrame - m_lastTime;
    m_lastTime = currentFrame;
}

float Clock::getDeltaTime() const
{
    return m_deltaTime;
}

void * Clock::getParent() const
{
    return m_parent;
}
