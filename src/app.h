#ifndef APP_H
#define APP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class App
{
public:
    explicit App();
    virtual ~App();

    virtual bool init();
    virtual void free();

    [[nodiscard]] void* get_parent() const;
};

#endif