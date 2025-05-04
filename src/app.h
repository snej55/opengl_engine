#ifndef APP_H
#define APP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine.h"
#include "window.h"

class App : public EngineObject
{
public:
    App();
    virtual ~App();

    virtual bool init();
    virtual void free();
};

#endif