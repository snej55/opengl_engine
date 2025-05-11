#ifndef CLOCK_H
#define CLOCK_H

#include <GLFW/glfw3.h>

#include "engine.h"

class Clock : public EngineObject
{
public:
    explicit Clock(EngineObject* parent);

    virtual void update();
    [[nodiscard]] float getDeltaTime() const;

private:
    float m_deltaTime{1.0f};
    float m_lastTime{0.0f};
};

#endif
