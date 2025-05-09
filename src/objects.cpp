#include "objects.h"

Object::Object(EngineObject* parent)
 : EngineObject{parent, "Object"}
{
}

Object::~Object()
{
    if (!m_freed)
    {
        free();
    }
}

void Object::init(const float* vertices)
{
    // vertex array & vertex buffer objects
    unsigned int vao, vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // buffer vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // load vertex array
    glBindVertexArray(vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    std::cout << "Loaded vertex data: " << sizeof(vertices) << " bytes\n";
}

void Object::free()
{
    EngineObject::free();
}