#include <memory>

#include "draw_call_builder.hpp"

#include "mesh.hpp"
#include "texture.hpp"
#include "shader_program.hpp"
#include "vbo.hpp"
#include "vao.hpp"
#include "material.hpp"

#include <algorithm>
#include <memory>


DrawCallBuilder::DrawCallBuilder()
{
}

DrawCall DrawCallBuilder::build(
        std::shared_ptr<ShaderProgram> program,
        std::shared_ptr<Mesh> mesh,
        std::shared_ptr<Texture> texture
        )
{
    int positionLoc = program->getLocation("in_Position");
    int uvLoc = program->getLocation("in_UV");
    int normalLoc = program->getLocation("in_Normal");

    std::shared_ptr<Vao> vao = std::make_shared<Vao>();

    setVertices(mesh, positionLoc, vao);
    setIndices(mesh, vao);
    if(mesh->hasUvs() && positionLoc != -1)
    {
        setUvs(mesh, uvLoc, vao);
    }
    if(mesh->hasNormals() && normalLoc != -1)
    {
        setNormals(mesh, normalLoc, vao);
    }

    std::shared_ptr<Material> material = std::make_shared<Material>(program, texture);

    return DrawCall(vao, material);
}

void DrawCallBuilder::setNormals(const std::shared_ptr<Mesh>& mesh, int normalLoc, std::shared_ptr<Vao>& vao)
{
    std::shared_ptr<Vbo> normalVbo = std::make_shared<Vbo>(VboContentType::Buffer);
    normalVbo->setData(flatten(mesh->Normals));
    vao->setAttribute(normalLoc, normalVbo, 3, GL_FLOAT);
}

void DrawCallBuilder::setUvs(const std::shared_ptr<Mesh>& mesh, int uvLoc, std::shared_ptr<Vao>& vao)
{
    std::shared_ptr<Vbo> uvVbo = std::make_shared<Vbo>(VboContentType::Buffer);
    uvVbo->setData(flatten(mesh->Uvs));
    vao->setAttribute(uvLoc, uvVbo, 2, GL_FLOAT);
}

void DrawCallBuilder::setIndices(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Vao>& vao)
{
    vao->IndicesCount = mesh->Indices.size();
    std::shared_ptr<Vbo> indicesVbo = std::make_shared<Vbo>(VboContentType::Indices);
    indicesVbo->setData(mesh->Indices);
    vao->bindVbo(indicesVbo);
}

void DrawCallBuilder::setVertices(const std::shared_ptr<Mesh>& mesh, int positionLoc, std::shared_ptr<Vao>& vao)
{
    std::shared_ptr<Vbo> posVbo = std::make_shared<Vbo>(VboContentType::Buffer);
    posVbo->setData(flatten(mesh->Vertices));
    vao->setAttribute(positionLoc, posVbo, 3, GL_FLOAT);
}

std::vector<float> DrawCallBuilder::flatten(std::vector<glm::vec3> vector)
{
    std::vector<float> flat;
    flat.reserve(vector.size() * 3);

    std::for_each(vector.begin(), vector.end(), [&flat](glm::vec3 vec)
    {
        flat.emplace_back(vec.x);
        flat.emplace_back(vec.y);
        flat.emplace_back(vec.z);
    });

    return flat;
}

std::vector<float> DrawCallBuilder::flatten(std::vector<glm::vec2> vector)
{
    std::vector<float> flat;
    flat.reserve(vector.size() * 2);

    std::for_each(vector.begin(), vector.end(), [&flat](glm::vec2 vec)
    {
        flat.emplace_back(vec.x);
        flat.emplace_back(vec.y);
    });

    return flat;
}

glm::vec3 circlePoint(int i, float step, float radius)
{
    float theta = i * step;
    float x = cosf(theta);
    float z = sinf(theta);

    return glm::vec3(x * radius, 0, z * radius);
}

