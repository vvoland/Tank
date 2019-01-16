#pragma once

#include "component.hpp"
#include "transform.hpp"

class Velocity : public Component
{
    public:
        Velocity();

        void update(float deltaTime);
        glm::vec3& velocity();

    private:
        glm::vec3 mVelocity;
};


template<>
struct ComponentTrait<Velocity>
{
    static constexpr int Index = 2;
};