#pragma once
#include "mvp.hpp"
#include "light.hpp"

#include <memory>
#include <GL/glew.h>
#include <vector>
#include "texture.hpp"

class Vao;
class Material;
class DrawCall
{
    public:
        DrawCall();
        DrawCall(std::shared_ptr<Vao> vao, std::shared_ptr<Material> material);
        DrawCall(DrawCall&& other) noexcept;
        DrawCall(const DrawCall& other);
        DrawCall& operator=(DrawCall&& vbo) noexcept;
        ~DrawCall();

        std::shared_ptr<Material> material();
        GLenum DrawMode;
        GLenum DataType;

        void draw(const Mvp& mvp, const std::vector<std::shared_ptr<Light>>& lights) const;
    private:
        std::shared_ptr<Vao> mVao;
        std::shared_ptr<Material> mMaterial;
};


