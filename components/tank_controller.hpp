#pragma once

#include "component.hpp"
#include "camera.hpp"
#include "input_system.hpp"
#include "transform.hpp"
#include "renderer.hpp"

class TankController : public Component
{
    public:
        explicit TankController(InputSystem& inputSystem, ComponentPtr<Transform> tower,
                ComponentPtr<Renderer> leftTracks, ComponentPtr<Renderer> rightTracks);

        void update(float dt);

        float& speed();
        float& towerSpeed();
        glm::quat TargetTowerRotation;

    private:
        InputSystem& Input;
        ComponentPtr<Transform> Tower;
        ComponentPtr<Renderer> LeftTracks, RightTracks;
        bool TargetInitialized;
        float Speed, TowerSpeed, TurnSpeed;
        float LeftTracksOffset, RightTracksOffset;
        float TracksSpeed;

        void rotateTower(float dt, float eps);
        void moveTracks();
        void move(ComponentPtr<Transform> transform, float dt, float eps);
};

template<>
struct ComponentTrait<TankController>
{
    static constexpr int Index = 6;
};
