#include "camera.hpp"
#include "gameobject.hpp"

Camera::Camera(const InputSystem& input)
    : Input(input)
{
    Speed = 150.0f;
    RotationSpeed = 1.0f;
    RotationX = RotationY = 0.0f;
}

float& Camera::speed()
{
    return Speed;
}

void Camera::update(float dt)
{
    static const float eps = 0.0001f;
    glm::vec2 mouseDelta = Input.mouseDelta();
    if(glm::length(mouseDelta) > eps)
    {
        auto transform = Owner->getComponent<Transform>();
        RotationX -= mouseDelta.y * RotationSpeed;
        RotationY -= mouseDelta.x * RotationSpeed;
        applyRotation();
    }

    float x = 0.0f;
    float y = 0.0f;

    if(Input.isKeyDown('A'))
        x -= 1.0f;
    if(Input.isKeyDown('D'))
        x += 1.0f;
    if(Input.isKeyDown('W'))
        y -= 1.0f;
    if(Input.isKeyDown('S'))
        y += 1.0f;

    if(x > eps || x < -eps || y > eps || y < -eps)
    {
        auto transform = Owner->getComponent<Transform>();
        glm::vec3 dir = glm::normalize(glm::vec3(x, 0, y));
        transform->translateLocal(dir * Speed * dt);
    }

    if(Input.isKeyDown(' '))
    {
        auto transform = Owner->getComponent<Transform>();
        transform->Position.y += Speed * dt;
        transform->markDirty();
    }
}

void Camera::applyRotation()
{
    auto transform = Owner->getComponent<Transform>();
    transform->Rotation = glm::quat(glm::vec3(RotationX, RotationY, 0.0f));
    transform->markDirty();
}

glm::mat4 Camera::view() const
{
    auto transform = Owner->getComponent<Transform>();
    return glm::inverse(transform->worldMatrix());
}

void Camera::copyRotationFromTransform()
{
    auto transform = Owner->getComponent<Transform>();
    auto euler = glm::eulerAngles(transform->Rotation);
    RotationX = euler.x;
    RotationY = euler.y;
    applyRotation();
}

float& Camera::rotationSpeed()
{
    return RotationSpeed;
}

