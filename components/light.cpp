#include <glm/gtc/matrix_transform.hpp>
#include "gameobject.hpp"
#include "transform.hpp"
#include "light.hpp"

Light::Light(LightType type)
    :
    Component(),
    mType(type),
    mColor(1.0f, 1.0f, 1.0f)
{
}

LightType& Light::type()
{
    return mType;
}

glm::vec3& Light::color()
{
    return mColor;
}

float& Light::range()
{
    return Range;
}

glm::vec3 Light::reflectorDirection()
{
    auto transform = Owner->getComponent<Transform>();
    glm::mat4 world = transform->worldMatrix();
    world = glm::translate(world, glm::vec3(0, 0, -1));
    return glm::vec3(world[3]) - transform->worldPosition();
}
