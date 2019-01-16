#pragma once

#include "draw_call.hpp"
#include "vbo.hpp"

#include <memory>
#include <vector>
#include <glm/vec3.hpp>

class Mesh;
class ShaderProgram;
class Vbo;
class Texture;

class DrawCallBuilder
{
    public:
        DrawCallBuilder();

        DrawCall build(std::shared_ptr<ShaderProgram> program, std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture);

    private:
        std::vector<float> flatten(std::vector<glm::vec3> vector);
        std::vector<float> flatten(std::vector<glm::vec2> vector);

        void setVertices(const std::shared_ptr<Mesh>& mesh, int positionLoc, std::shared_ptr<Vao>& vao);
        void setIndices(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Vao>& vao);
        void setUvs(const std::shared_ptr<Mesh>& mesh, int uvLoc, std::shared_ptr<Vao>& vao);
        void setNormals(const std::shared_ptr<Mesh>& mesh, int normalLoc, std::shared_ptr<Vao>& vao);
};


