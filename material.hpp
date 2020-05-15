#pragma once
#include "mvp.hpp"

#include <glm/fwd.hpp>

#include <memory>
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
        void setSkybox(std::shared_ptr<Texture> texture);
        void setLights(const std::vector<std::shared_ptr<Light>>& lights);
        glm::vec4& color();
        glm::vec3& ambient();
        float& uvSpeed();
        float& reflection();
        float& refraction();
        float& refractionCoefficient();
        std::shared_ptr<ShaderProgram> program();

    private:
        std::shared_ptr<ShaderProgram> mProgram;
        std::shared_ptr<Texture> mTexture, mSkybox;

        glm::vec4 Color;
        glm::vec3 AmbientColor;
        float Reflection, Refraction;
        float RefractionCoeff;
        float UvSpeed;
};


