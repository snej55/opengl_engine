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
}