#include "objects.h"

Object::Object(EngineObject* parent)
 : EngineObject{parent, "Object"}
{
}

Object::~Object()
{
    if (!m_freed)
    {
        Object::free();
    }
}

void Object::init(const std::vector<float>& vertices)
{
    // vertex array & vertex buffer objects
    unsigned int vao, vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // buffer vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size()), vertices.data(), GL_STATIC_DRAW);

    // load vertex array
    glBindVertexArray(vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    std::cout << "Loaded vertex data: " << std::size(vertices) * sizeof(float) << " bytes\n";

    // update vao & vbo values
    m_VAO = vao;
    m_VBO = vbo;
}

void Object::free()
{
    EngineObject::free();
}