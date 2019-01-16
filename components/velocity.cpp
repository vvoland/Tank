#include "gameobject.hpp"
#include "velocity.hpp"

Velocity::Velocity()
{
}

void Velocity::update(float deltaTime)
{
    auto transform = Owner->getComponent<Transform>();
    if(transform)
    {
         transform->translateLocal(mVelocity * deltaTime);
    }
}

glm::vec3& Velocity::velocity()
{
    return mVelocity;
}
