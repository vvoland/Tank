#include "shader_program.hpp"
#include "shader.hpp"

#include <GL/glew.h>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <memory>

ShaderProgram::ShaderProgram()
{
    Id = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(Id);
}

void ShaderProgram::fromShaders(const std::vector<std::shared_ptr<Shader>>& shaders)
{
    int id = Id;
    const auto attach = [id](std::shared_ptr<Shader> shader)
    {
        glAttachShader(id, shader->id());
    };
    const auto detach = [id](std::shared_ptr<Shader> shader)
    {
        glDetachShader(id, shader->id());
    };

    std::for_each(shaders.begin(), shaders.end(), attach);

    glLinkProgram(Id);
    throwIfError();

    std::for_each(shaders.begin(), shaders.end(), detach);
}

void ShaderProgram::throwIfError()
{
    GLint result = GL_FALSE;
    int infoLength;

    glGetProgramiv(Id, GL_LINK_STATUS, &result);
    glGetProgramiv(Id, GL_INFO_LOG_LENGTH, &infoLength);
    if(result != GL_TRUE)
    {
        std::string error;
        error.resize(infoLength + 1, '\0');
        glGetProgramInfoLog(Id, infoLength, NULL, (GLchar*)error.data());
        throw std::runtime_error(error);
    }
}

void ShaderProgram::bind() const
{
    glUseProgram(Id);

}

int ShaderProgram::getLocation(const std::string& attribute)
{
    return glGetAttribLocation(Id, attribute.c_str());
}
