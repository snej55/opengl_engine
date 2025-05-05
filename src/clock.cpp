#include "clock.h"

Clock::Clock(EngineObject* parent)
 : EngineObject{parent, "Clock"}
{
    update();
}

void Clock::update()
{
    const float time{static_cast<float>(glfwGetTime())};
    m_deltaTime = time - m_lastTime;
    m_lastTime = time;
}

float Clock::getDeltaTime() const
{
    return m_deltaTime;
}