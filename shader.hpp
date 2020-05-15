#pragma once
#include <glm/fwd.hpp>
#include <GL/glew.h>

#include <string>

class ShaderProgram;

enum class ShaderType
{
    Vertex,
    Fragment,
    Geometry
};

class Shader
{
    public:
        explicit Shader(ShaderType type);
        ~Shader();

        void loadFromFile(const std::string& name);
        void loadFromString(const char* source);

        GLuint id() const;
        ShaderType type() const;

    private:
        GLuint Id;
        ShaderType Type;
        GLenum typeToGLEnum() const;

};

