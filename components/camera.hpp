#pragma once

#include "component.hpp"
#include "input_system.hpp"
#include "transform.hpp"

#include <memory>

class Camera : public Component
{
    public:
        Camera(const InputSystem& input);

        float& speed();
        float& rotationSpeed();
        glm::mat4 view() const;
        void copyRotationFromTransform();

        void update(float dt);
    private:
        float RotationX, RotationY;
        float Speed, RotationSpeed;
        const InputSystem& Input;

        void applyRotation();
};

template<>
struct ComponentTrait<Camera>
{
    static constexpr int Index = 3;
};