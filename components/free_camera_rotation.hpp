#pragma once
#include "component.hpp"
#include "systems/input_system.hpp"

#include <glm/fwd.hpp>

class FreeCameraRotation : public Component
{
    public:
        explicit FreeCameraRotation(const InputSystem& input);

        enum class RotationMode
        {
            Self,
            Parent
        };

        float& speed();
        RotationMode& rotationMode();

        void update(float dt);

        static glm::highp_dquat fpsRotate(glm::highp_dquat rotation, glm::vec2 delta);
    private:
        float Speed;
        const InputSystem& Input;
        RotationMode Mode;
};


template<>
struct ComponentTrait<FreeCameraRotation>
{
    static constexpr int Index = 7;
};


