#pragma once
#include "mvp.hpp"

#include <memory>
#include <glm/detail/type_mat.hpp>
#include <glm/vec3.hpp>
#include <vector>

class Light;
class ShaderProgram;
class Texture;
class Material
{
    public:
        Material(std::shared_ptr<ShaderProgram> program, std::shared_ptr<Texture> texture);
        void bind() const;
        void setMVP(const Mvp& mvp);
        void setLights(const std::vector<std::shared_ptr<Light>>& lights);
        glm::vec4& color();
        glm::vec3& ambient();
        float& uvSpeed();

    private:
        std::shared_ptr<ShaderProgram> mProgram;
        std::shared_ptr<Texture> mTexture;

        glm::vec4 Color;
        glm::vec3 AmbientColor;
        float UvSpeed;
};


