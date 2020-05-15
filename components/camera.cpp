#include "camera.hpp"
#include "gameobject.hpp"

#include <glm/gtx/rotate_vector.hpp>

Camera::Camera()
    :
        CurrentRotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
        Initialized(false),
        Speed(20.0f)
{
}

glm::mat4 Camera::view() const
{
    auto transform = Owner->getComponent<Transform>();
    auto realRotation = transform->Rotation;

    transform->Rotation = CurrentRotation;
    transform->markDirty();
    glm::mat4 view = glm::inverse(transform->worldMatrix());

    transform->Rotation = realRotation;
    transform->markDirty();
    return view;
}

void Camera::update(float dt)
{
    auto transform = Owner->getComponent<Transform>();
    if(!Initialized)
    {
        CurrentRotation = transform->Rotation;
        Initialized = true;
    }

    CurrentRotation = glm::slerp(CurrentRotation, transform->Rotation, dt * Speed);
}

float& Camera::speed()
{
    return Speed;
}
