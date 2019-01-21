#include "draw_call.hpp"
#include "mvp.hpp"
#include "vao.hpp"
#include "material.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "shader_program.hpp"

DrawCall::DrawCall()
{
    DrawMode = GL_TRIANGLES;
    DataType = GL_UNSIGNED_INT;
}

DrawCall::DrawCall(const DrawCall& other)
{
    DrawMode = other.DrawMode;
    DataType = other.DataType;
    mVao = other.mVao;
    mMaterial = other.mMaterial;
}

DrawCall::DrawCall(DrawCall&& other) noexcept
{
    DrawMode = other.DrawMode;
    DataType = other.DataType;
    mVao = std::move(other.mVao);
    mMaterial = std::move(other.mMaterial);
}

DrawCall& DrawCall::operator=(DrawCall&& other) noexcept
{
    std::swap(DrawMode, other.DrawMode);
    std::swap(DataType, other.DataType);
    std::swap(mVao, other.mVao);
    std::swap(mMaterial, other.mMaterial);
    return *this;
}

DrawCall::DrawCall(std::shared_ptr<Vao> vao, std::shared_ptr<Material> material)
{
    DrawMode = GL_TRIANGLES;
    DataType = GL_UNSIGNED_INT;
    mVao = vao;
    mMaterial = material;
}

DrawCall::~DrawCall()
{
}

void DrawCall::draw(const Mvp& mvp, const std::vector<std::shared_ptr<Light>>& lights) const
{
    mVao->bind();
    mMaterial->bind();
    mMaterial->setMVP(mvp);
    mMaterial->setLights(lights);

    mVao->enableAttributes();

    glDrawElements(DrawMode, (GLsizei)mVao->IndicesCount, DataType, nullptr);
    mVao->disableAttributes();
}

std::shared_ptr<Material> DrawCall::material()
{
    return mMaterial;
}
