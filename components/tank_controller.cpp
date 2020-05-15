#include <utility>

#include "gameobject.hpp"
#include "free_camera_rotation.hpp"
#include "tank_controller.hpp"
#include "material.hpp"

TankController::TankController(InputSystem& inputSystem, std::shared_ptr<Transform> tower,
                               ComponentPtr<Renderer> leftTracks, ComponentPtr<Renderer> rightTracks)
    :
        TargetTowerRotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
        Input(inputSystem),
        Tower(std::move(tower)),
        LeftTracks(std::move(leftTracks)),
        RightTracks(std::move(rightTracks)),
        TargetInitialized(false),
        Speed(2.0f),
        TowerSpeed(50.0f),
        TurnSpeed(0.25f),
        LeftTracksOffset(0.0f),
        RightTracksOffset(0.0f),
        TracksSpeed(0.5f)
{

}

void TankController::update(float dt)
{
    if(!Enabled)
        return;

    auto transform = Owner->getComponent<Transform>();
    const float eps = 0.0001f;

    rotateTower(dt, eps);
    move(transform, dt, eps);
    moveTracks();
}

void TankController::move(ComponentPtr<Transform> transform, float dt, float eps)
{
    float forward = 0.0f;
    if(Input.isKeyDown('W'))
    {
        forward = 1.0f;
    }
    else if(Input.isKeyDown('S'))
    {
        forward = -1.0f;
    }

    if(forward > eps || forward < eps)
    {
        transform->translateLocal(glm::vec3(0.0f, 0.0f, forward) * Speed * dt);
        LeftTracksOffset += forward * TracksSpeed * dt;
        RightTracksOffset += forward * TracksSpeed * dt;
    }


    float horizontal = 0.0f;
    float turningSpeed = 0.5f;
    if(Input.isKeyDown('A'))
    {
        horizontal = -1.0f;
        LeftTracksOffset += dt * TracksSpeed * turningSpeed;
        RightTracksOffset -= dt * TracksSpeed * turningSpeed;
    }
    else if(Input.isKeyDown('D'))
    {
        horizontal = 1.0f;
        LeftTracksOffset -= dt * TracksSpeed * turningSpeed;
        RightTracksOffset += dt * TracksSpeed * turningSpeed;
    }
    if(horizontal > eps || horizontal < eps)
    {
        transform->rotateLocal(glm::quat(glm::vec3(0.0f, -horizontal * dt * TurnSpeed, 0.0f)));
    }
}

void TankController::moveTracks()
{
    LeftTracks->Call.material()->uvSpeed() = LeftTracksOffset;
    RightTracks->Call.material()->uvSpeed() = RightTracksOffset;
}

void TankController::rotateTower(float dt, const float eps)
{
    if(!TargetInitialized)
    {
        TargetInitialized = true;
        TargetTowerRotation = Tower->Rotation;
    }
    glm::vec2 delta = Input.mouseDelta() * TowerSpeed;

    if(delta.x > eps || delta.x < eps || delta.y > eps || delta.y < eps)
    {
        delta *= dt;

        TargetTowerRotation = FreeCameraRotation::fpsRotate(TargetTowerRotation, glm::vec2(0.0f, delta.x));
        Tower->Rotation = glm::slerp(Tower->Rotation, TargetTowerRotation, glm::clamp(dt, 0.0f, 1.0f));
        Tower->markDirty();
    }
}

float& TankController::speed()
{
    return Speed;
}

float& TankController::towerSpeed()
{
    return TowerSpeed;
}

