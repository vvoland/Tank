#pragma once
#include <glm/detail/type_mat4x4.hpp>

struct Mvp
{
    glm::mat4 Model, View, Projection;
    glm::mat4 Mvp;
};