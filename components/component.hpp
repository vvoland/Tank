#pragma once

#include <memory>

class GameObject;

struct Component
{
    GameObject* Owner = nullptr;
    bool Enabled = true;
};

template<typename T,
        typename = std::enable_if<std::is_assignable<Component, T>::value>>
using ComponentPtr = std::shared_ptr<T>;

static constexpr int COMPONENT_INVALID_INDEX = -1;

template<typename T>
struct ComponentTrait
{
    static constexpr int Index = COMPONENT_INVALID_INDEX;
};

template<typename T>
void assert_component()
{
    constexpr int index = ComponentTrait<T>::Index;
    static_assert(index > COMPONENT_INVALID_INDEX,  "Not a component!");
    static_assert(std::is_base_of<Component, T>::value, "Component must derive from Component class!");
}
