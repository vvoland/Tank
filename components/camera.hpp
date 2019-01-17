#pragma once

#include "component.hpp"
#include "input_system.hpp"
#include "transform.hpp"

#include <memory>

class Camera : public Component
{
    public:
        Camera();

        float& speed();
        void update(float dt);
        glm::mat4 view() const;
    private:
        glm::quat CurrentRotation;
        bool Initialized;
        float Speed;
};

template<>
struct ComponentTrait<Camera>
{
    static constexpr int Index = 3;
};