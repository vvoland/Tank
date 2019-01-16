#pragma once

#include <vector>
#include <GL/glew.h>

enum class VboContentType
{
   Buffer,
   Indices
};

class Vbo
{
    public:
        Vbo(VboContentType contentType);
        Vbo(Vbo&& other);
        Vbo(const Vbo& other) = delete;
        Vbo& operator=(Vbo vbo);
        ~Vbo();

        void bind() const;

        template<typename T>
        void setData(const std::vector<T>& data)
        {
            bind();
            glBufferData(target(), data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);
        }

    private:
        const VboContentType Type;
        GLenum target() const;
        GLuint Id;
};

