#include "vbo.hpp"

#include <GL/glew.h>
#include <stdexcept>


Vbo::Vbo(VboContentType contentType)
    : Type(contentType)
{
    glGenBuffers(1, &Id);
}

Vbo::Vbo(Vbo&& other)
    : Type(other.Type)
{
    Id = other.Id;
    other.Id = 0;
}

Vbo& Vbo::operator=(Vbo other)
{
    std::swap(Id, other.Id);
    return *this;
}

Vbo::~Vbo()
{
    if(Id > 0)
    {
        glDeleteBuffers(1, &Id);
    }
}

void Vbo::bind() const
{
    glBindBuffer(target(), Id);
}

GLenum Vbo::target() const
{
    switch(Type)
    {
        case VboContentType::Buffer:
            return GL_ARRAY_BUFFER;
        case VboContentType::Indices:
            return GL_ELEMENT_ARRAY_BUFFER;
    }

    throw std::runtime_error("Unsupported Vbo!");
}
