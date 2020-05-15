#pragma once
#include "shader_uniforms.inl"

#include <glm/fwd.hpp>

#include <functional>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>


class Shader;

class ShaderProgram
{
    public:
        ShaderProgram();
        ~ShaderProgram();

        void fromShaders(const std::vector<std::shared_ptr<Shader>>& shaders);
        int getLocation(const std::string& attribute);

        void bind() const;

        template<typename T>
        bool setUniform(const std::string& uniform, const T& value)
        {
            static_assert(UniformTrait<T>::IsUniform, "Type can not be used as uniform!");

            bind();
            GLint location = glGetUniformLocation(Id, uniform.c_str());
            if(location == -1)
                return false;

            UniformTrait<T>::Set(location, value);
            return true;
        }
    private:
        GLuint Id;
        void throwIfError();
};


