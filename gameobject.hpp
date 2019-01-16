#pragma once

#include "component.hpp"

#include <memory>

class GameObject
{
    public:
        explicit GameObject(const std::string& name);

        template<typename T>
        void addComponent(std::shared_ptr<T> component)
        {
            assert_component<T>();

            constexpr int index = ComponentTrait<T>::Index;
            Components[index] = std::static_pointer_cast<Component>(component);
            Components[index]->Owner = this;
        }

        template<typename T>
        void removeComponent()
        {
            assert_component<T>();

            constexpr int index = ComponentTrait<T>::Index;
            Components[index].reset();
        }

        template<typename T>
        bool hasComponent() const
        {
            assert_component<T>();

            constexpr int index = ComponentTrait<T>::Index;
            return Components[index] != nullptr;
        }

        template<typename T>
        std::shared_ptr<T> getComponent() const
        {
            assert_component<T>();

            constexpr int index = ComponentTrait<T>::Index;
            return std::static_pointer_cast<T>(Components[index]);
        }

        std::string Name;
    private:
        std::shared_ptr<Component> Components[16];

};

