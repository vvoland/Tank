#pragma once

#include <glm/vec3.hpp>
#include "component.hpp"

enum class LightType
{
    Directional = 0,
    Point = 1,
    Reflector = 2
};

class Light : public Component
{
    public:
        explicit Light(LightType type = LightType::Directional);

        float& range();
        glm::vec3& color();
        glm::vec3 reflectorDirection();
        LightType& type();
    private:
        LightType mType;
        glm::vec3 mColor;
        float Range;

};

template<>
struct ComponentTrait<Light>
{
    static constexpr int Index = 4;
};

