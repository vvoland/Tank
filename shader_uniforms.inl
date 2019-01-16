#pragma once

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

template<typename T>
struct UniformTrait
{
    static constexpr bool IsUniform = false;
};

template<>
struct UniformTrait<glm::mat4>
{
    static constexpr bool IsUniform = true;
    static void Set(int location, const glm::mat4& value)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    };
};

template<>
struct UniformTrait<glm::vec4>
{
    static constexpr bool IsUniform = true;
    static void Set(int location, const glm::vec4& value)
    {
        glUniform4fv(location, 1, glm::value_ptr(value));
    };
};

template<>
struct UniformTrait<glm::vec3>
{
    static constexpr bool IsUniform = true;
    static void Set(int location, const glm::vec3& value)
    {
        glUniform3fv(location, 1, glm::value_ptr(value));
    };
};

template<>
struct UniformTrait<int>
{
    static constexpr bool IsUniform = true;
    static void Set(int location, const int& value)
    {
        glUniform1i(location, value);
    };
};

template<>
struct UniformTrait<float>
{
    static constexpr bool IsUniform = true;
    static void Set(int location, const float& value)
    {
        glUniform1f(location, value);
    };
};
