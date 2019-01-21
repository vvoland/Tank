#include <utility>
#include <components/transform.hpp>

#include "material.hpp"
#include "gameobject.hpp"
#include "shader_program.hpp"
#include "texture.hpp"
#include "mvp.hpp"
#include "light.hpp"

Material::Material(std::shared_ptr<ShaderProgram> program, std::shared_ptr<Texture> texture)
    : AmbientColor(0.05f, 0.05f, 0.05f),
    Reflection(0.0f),
    Refraction(0.0f),
    RefractionCoeff(1.0f)
{
    mProgram = std::move(program);
    mTexture = std::move(texture);
    Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

void Material::bind() const
{
    mProgram->bind();
    if(mTexture != nullptr)
    {
        mTexture->bind(0);
        mProgram->setUniform("in_Texture", 0);
    }
    if(mSkybox != nullptr)
    {
        mSkybox->bind(1);
        mProgram->setUniform("in_Skybox", 1);
    }
    mProgram->setUniform("in_Color", Color);
    mProgram->setUniform("in_AmbientColor", AmbientColor);
    mProgram->setUniform("in_UvSpeed", UvSpeed);
    mProgram->setUniform("in_Refraction", Refraction);
    mProgram->setUniform("in_Reflection", Reflection);
    mProgram->setUniform("in_RefractionCoefficient", RefractionCoeff);
}

void Material::setMVP(const Mvp& mvp)
{
    mProgram->setUniform("in_Model", mvp.Model);
    mProgram->setUniform("in_View", mvp.View);
    mProgram->setUniform("in_Projection", mvp.Projection);
    mProgram->setUniform("in_Mvp", mvp.Mvp);
}

glm::vec4& Material::color()
{
    return Color;
}

void Material::setLights(const std::vector<std::shared_ptr<Light>>& lights)
{
    static const std::string positions[] = {
           "in_Light[0].Position",
           "in_Light[1].Position",
           "in_Light[2].Position",
           "in_Light[3].Position",
    };
    static const std::string types[] = {
            "in_Light[0].Type",
            "in_Light[1].Type",
            "in_Light[2].Type",
            "in_Light[3].Type",
    };
    static const std::string ranges[] = {
            "in_Light[0].Range",
            "in_Light[1].Range",
            "in_Light[2].Range",
            "in_Light[3].Range",
    };
    static const std::string colors[] = {
            "in_Light[0].Color",
            "in_Light[1].Color",
            "in_Light[2].Color",
            "in_Light[3].Color",
    };
    static const std::string directions[] = {
            "in_Light[0].ReflectorDirection",
            "in_Light[1].ReflectorDirection",
            "in_Light[2].ReflectorDirection",
            "in_Light[3].ReflectorDirection",
    };
    int count = static_cast<int>(lights.size());

    mProgram->setUniform("in_LightCount", count);
    for(int i = 0; i < count; i++)
    {
        auto light = lights[i];
        auto transform = light->Owner->getComponent<Transform>();
        int type = static_cast<int>(light->type());

        mProgram->setUniform(positions[i], transform->worldPosition());
        mProgram->setUniform(types[i], type);
        mProgram->setUniform(ranges[i], light->range());
        mProgram->setUniform(colors[i], light->color());
        if(light->type() == LightType::Reflector)
            mProgram->setUniform(directions[i], light->reflectorDirection());
    }

}

glm::vec3& Material::ambient()
{
    return AmbientColor;
}

float& Material::uvSpeed()
{
    return UvSpeed;
}

std::shared_ptr<ShaderProgram> Material::program()
{
    return mProgram;
}

void Material::setSkybox(std::shared_ptr<Texture> texture)
{
    mSkybox = std::move(texture);
}

float& Material::reflection()
{
    return Reflection;
}

float& Material::refraction()
{
    return Refraction;
}

float& Material::refractionCoefficient()
{
    return RefractionCoeff;
}
