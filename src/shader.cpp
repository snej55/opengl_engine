#include "shader.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(EngineObject* parent)
 : EngineObject{parent, "Shader"}
{
}

Shader::~Shader()
{
    if (!m_freed)
    {
        free();
    }
}

void Shader::free()
{
    EngineObject::free();
}

void Shader::loadFromFile(const char* fragPath, const char* vertPath)
{
    std::string vertCode;
    std::string fragCode;
    std::ifstream vertFile;
    std::ifstream fragFile;

    vertFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    // load shader source from file
    try
    {
        vertFile.open(vertPath);
        fragFile.open(fragPath);
        std::stringstream vertStream, fragStream;

        vertStream << vertFile.rdbuf();
        fragStream << fragFile.rdbuf();

        vertFile.close();
        fragFile.close();

        vertCode = vertStream.str();
        fragCode = fragStream.str();
    } catch ([[maybe_unused]] std::ifstream::failure& e)
    {
        std::cout << "SHADER::LOAD_FROM_FILE::ERROR: Could not read source files: {vert: `" << vertPath << "`, frag: `" << fragPath << "`}" << std::endl;
        return;
    }

    const char* vShaderCode {vertCode.c_str()};
    const char* fShaderCode {fragCode.c_str()};

    // compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512]; // for errors

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cout << "SHADER::LOAD_FROM_FILE::ERROR: Vertex shader compilation failed." << std::endl << infoLog;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cout << "SHADER::LOAD_FROM_FILE::ERROR: Fragment shader compilation failed." << std::endl << infoLog;
    }

    // actually create the program
    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertex);
    glAttachShader(m_ID, fragment);
    glLinkProgram(m_ID);
    // get linking errors
    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_ID, 512, nullptr, infoLog);
        std::cout << "SHADER::LOAD_FROM_FILE::ERROR: Shader linking failed." << std::endl << infoLog;
    }

    // no longer needed
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() const
{
    glUseProgram(m_ID);
}

unsigned int Shader::getID() const
{
    return m_ID;
}
