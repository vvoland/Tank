#include "vao.hpp"
#include "vbo.hpp"

Vao::Vao()
{
    IndicesCount = 0;
    glGenVertexArrays(1, &Id);
}

Vao::Vao(Vao&& other) noexcept
{
    other.Id = 0;
    Id = other.Id;
    IndicesCount = other.IndicesCount;
    Locations = std::move(other.Locations);
    Vbos = std::move(other.Vbos);
}

Vao& Vao::operator=(Vao&& other) noexcept
{
    std::swap(Id, other.Id);
    std::swap(IndicesCount, other.IndicesCount);
    std::swap(Locations, other.Locations);
    std::swap(Vbos, other.Vbos);
    return *this;
}

Vao::~Vao()
{
    if(Id > 0)
    {
        glDeleteVertexArrays(1, &Id);
    }
}

void Vao::bind() const
{
    glBindVertexArray(Id);
}

void Vao::setAttribute(int location, std::shared_ptr<Vbo> vbo, int elementSize, GLenum dataType)
{
    bindVbo(vbo);

    Locations.push_back(location);
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, elementSize, dataType, GL_FALSE, 0, nullptr);
    glDisableVertexAttribArray(location);
}

void Vao::bindVbo(std::shared_ptr<Vbo> vbo)
{
    bind();
    vbo->bind();

    Vbos.emplace_back(std::move(vbo));
}

GLuint Vao::id() const
{
    return Id;
}

void Vao::enableAttributes() const
{
    for(int location : Locations)
        glEnableVertexAttribArray(location);
}

void Vao::disableAttributes() const
{
    for(int location : Locations)
        glDisableVertexAttribArray(location);
}
