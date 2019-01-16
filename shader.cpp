#include "shader.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <stdexcept>
#include <fstream>


Shader::Shader(ShaderType type)
    : Type(type)
{
    Id = glCreateShader(typeToGLEnum());
}

Shader::~Shader()
{
    glDeleteShader(Id);
}

void Shader::loadFromFile(const std::string& name)
{
    std::ifstream file;
    file.open(name.c_str());

    if(!file.is_open())
        throw std::runtime_error("Shader file " + name + " does not exist");

    file.seekg(0, std::fstream::end);
    long fileSize = file.tellg();
    file.seekg(0, std::fstream::beg);

    char* fileContent = new char[fileSize + 1];
    file.read(fileContent, fileSize);
    fileContent[fileSize] = '\0';

    file.close();
    loadFromString(fileContent);
}

void Shader::loadFromString(const char* content)
{
    glShaderSource(Id, 1, &content, NULL);
    glCompileShader(Id);

    GLint result = GL_FALSE;
    int infoLength;

    glGetShaderiv(Id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(Id, GL_INFO_LOG_LENGTH, &infoLength);
    if(infoLength > 0)
    {
        std::string error; error.resize(infoLength + 1, '\0');
        glGetShaderInfoLog(Id, infoLength, NULL, (GLchar*)error.data());
        throw std::runtime_error(error);
    }
}

ShaderType Shader::type() const
{
    return Type;
}

GLenum Shader::typeToGLEnum() const
{
    switch(Type)
    {
        case ShaderType::Vertex:
            return GL_VERTEX_SHADER;
        case ShaderType::Fragment:
            return GL_FRAGMENT_SHADER;
        case ShaderType::Geometry:
            return GL_GEOMETRY_SHADER;
    }

    throw std::runtime_error("Unhandled ShaderType");
}

GLuint Shader::id() const
{
    return Id;
}
