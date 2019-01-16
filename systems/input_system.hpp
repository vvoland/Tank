#pragma once

#include "input.hpp"

#include <map>
#include <glm/vec2.hpp>

class InputSystem
{
    public:
        InputSystem();

        const glm::vec2& mousePosition() const;
        const glm::vec2& mouseDelta() const;

        void update(float dt);
        bool isKeyDown(KeyCode key) const;

        void setKeyDown(KeyCode key, bool down);

        void setMousePosition(double d, double d1);

    private:
        std::map<KeyCode, bool> KeyDown;

        glm::vec2 MousePosition;
        glm::vec2 LastMousePosition;
        glm::vec2 MouseDelta;
};


