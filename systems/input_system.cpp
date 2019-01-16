#include "input_system.hpp"

bool InputSystem::isKeyDown(KeyCode key) const
{
    return KeyDown.at(key);
}

void InputSystem::setKeyDown(KeyCode key, bool down)
{
    KeyDown[key] = down;
}

InputSystem::InputSystem()
{
    for(int i = 0; i < 1024; i++)
        KeyDown[i] = false;
}

void InputSystem::setMousePosition(double x, double y)
{
    MousePosition = glm::vec2(x, y);
}

void InputSystem::update(float dt)
{
    MouseDelta = MousePosition - LastMousePosition;
    LastMousePosition = MousePosition;
}

const glm::vec2& InputSystem::mousePosition() const
{
    return MousePosition;
}

const glm::vec2& InputSystem::mouseDelta() const
{
    return MouseDelta;
}
