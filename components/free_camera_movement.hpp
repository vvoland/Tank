#pragma once

#include <systems/input_system.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/fwd.hpp>
#include "component.hpp"

class FreeCameraMovement : public Component
{
    public:
        explicit FreeCameraMovement(const InputSystem& input);

        float& speed();

        void update(float dt);
    private:
        float Speed;
        const InputSystem& Input;
};


template<>
struct ComponentTrait<FreeCameraMovement>
{
    static constexpr int Index = 5;
};


