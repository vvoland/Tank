#pragma once

#include <string>
#include <GL/glew.h>
#include <vector>
#include <memory>

class Vbo;
class Vao
{
    public:
        Vao();
        Vao(Vao&& other) noexcept;
        Vao(const Vao& other) = delete;
        Vao& operator=(Vao&& vbo) noexcept;
        ~Vao();

        void bind() const;
        GLuint id() const;
        GLuint IndicesCount;

        void setAttribute(int location, std::shared_ptr<Vbo> vbo, int elementSize, GLenum dataType = GL_FLOAT);
        void bindVbo(std::shared_ptr<Vbo> vbo);

        void enableAttributes() const;
        void disableAttributes() const;
    private:
        GLuint Id;
        std::vector<int> Locations;
        std::vector<std::shared_ptr<Vbo>> Vbos;
};


