#pragma once
#include "component.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <memory>
#include <vector>

class Transform : public Component
{
    public:
        Transform();
        ~Transform();

        glm::quat PreRotation;
        glm::vec3 Position;
        glm::quat Rotation;
        glm::vec3 Scale;

        glm::vec3 worldPosition();

        void markDirty();
        bool isDirty() const;
        const glm::mat4& worldMatrix();

        glm::vec3 transformLocalPoint(const glm::vec3& point);
        void translateLocal(const glm::vec3& translation);
        void rotateLocal(const glm::quat& rot);
        void lookAt(const glm::vec3& worldPoint);

        std::shared_ptr<Transform> parent() const;
        void setParent(std::shared_ptr<Transform> parent);
        const std::vector<Transform*> children() const;

    private:
        bool Dirty;
        glm::mat4 CachedWorldMatrix;
        std::shared_ptr<Transform> Parent;
        std::vector<Transform*> Children;

        void removeChild(Transform* transform);
        void addChild(Transform* transform);
        glm::quat worldRotation(const glm::mat4& mat) const;
        glm::mat4 localTransform(glm::mat4 mat) const;
};


template<>
struct ComponentTrait<Transform>
{
    static constexpr int Index = 0;
};
