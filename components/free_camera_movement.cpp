#include <utility>

#include "free_camera_movement.hpp"
#include "camera.hpp"
#include "gameobject.hpp"

FreeCameraMovement::FreeCameraMovement(const InputSystem& input)
        : Input(input)
{
    Speed = 25.0f;
}

float& FreeCameraMovement::speed()
{
    return Speed;
}

void FreeCameraMovement::update(float dt)
{
    if(!Enabled)
        return;

    static const float eps = 0.0001f;
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
}

