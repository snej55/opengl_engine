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

    virtual bool init(const int width, const int height, const char* title);
    virtual bool createWindow(const int width, const int height, const char* title); // initializes window
    virtual void free();

    [[nodiscard]] EngineObject* getWindow() const {return m_window;}

private:
    Window* m_window{nullptr};
};

#endif