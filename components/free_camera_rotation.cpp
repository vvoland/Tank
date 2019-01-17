#include <utility>

#include "free_camera_rotation.hpp"
#include "camera.hpp"
#include "gameobject.hpp"

FreeCameraRotation::FreeCameraRotation(const InputSystem& input)
    :
        Input(input),
        Mode(RotationMode::Self)
{
    Speed = 50.0f;
}

float& FreeCameraRotation::speed()
{
    return Speed;
}

void FreeCameraRotation::update(float dt)
{
    if(!Enabled)
        return;

    static const float eps = 0.0001f;
    glm::vec2 delta = Input.mouseDelta() * Speed;
    if(glm::length(delta) > eps)
    {
        auto transform = Owner->getComponent<Transform>();
        glm::quat& targetRotation = Mode == RotationMode::Self ? transform->Rotation : transform->PreRotation;
        float verticalInvert = Mode == RotationMode::Parent ? -1.0f : 1.0f;

        targetRotation = fpsRotate(targetRotation, glm::vec2(verticalInvert * delta.y, delta.x) * dt);
        transform->markDirty();
    }
}

glm::highp_dquat FreeCameraRotation::fpsRotate(glm::highp_dquat rotation, glm::vec2 delta)
{
    glm::highp_dquat horizontalRot(glm::vec3(0.0f, -delta.y, 0.0f));
    glm::highp_dquat verticalRot(glm::vec3(-delta.x, 0.0f, 0.0f));

    return horizontalRot * rotation * verticalRot;
}

FreeCameraRotation::RotationMode& FreeCameraRotation::rotationMode()
{
    return Mode;
}
