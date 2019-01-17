#include "transform.hpp"

#include <algorithm>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

bool Transform::isDirty() const
{
    return Dirty;
}

void Transform::markDirty()
{
    Dirty = true;
    for(Transform* child : Children)
    {
        child->markDirty();
    }
}

std::shared_ptr<Transform> Transform::parent() const
{
    return Parent;
}

void Transform::setParent(std::shared_ptr<Transform> parent)
{
    if(Parent != nullptr)
    {
        Parent->removeChild(parent.get());
    }
    Parent = parent;
    parent->addChild(this);
}

void Transform::removeChild(Transform* transform)
{
    Children.erase(std::remove(Children.begin(), Children.end(), transform));
}

const std::vector<Transform*> Transform::children() const
{
    return Children;
}

void Transform::addChild(Transform* transform)
{
    Children.emplace_back(transform);
}

Transform::Transform()
    : Dirty(true)
{
    Scale = glm::vec3(1.0f);
}

Transform::~Transform()
{

}

const glm::mat4& Transform::worldMatrix()
{
    if(Dirty)
    {
        glm::mat4 model(1.0f);
        if(Parent != nullptr)
            model = Parent->worldMatrix();

        model = localTransform(model);
        CachedWorldMatrix = model;

        Dirty = false;
    }

    return CachedWorldMatrix;
}

void Transform::rotateLocal(const glm::quat& rot)
{
    Rotation *= rot;
    markDirty();
}

glm::vec3 Transform::transformLocalPoint(const glm::vec3& point)
{
    glm::mat4 mat = localTransform(glm::mat4(1.0f));
    mat = glm::translate(mat, point);

    return glm::vec3(mat[3]);
}

void Transform::translateLocal(const glm::vec3& translation)
{
    Position = transformLocalPoint(translation);
    markDirty();
}

void Transform::lookAt(const glm::vec3& worldPoint)
{
    glm::vec3 worldPos = worldPosition();
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 mat = glm::lookAt(worldPos, worldPoint, up);

    glm::quat worldRot = worldRotation(mat);
    if(parent() != nullptr)
    {
        glm::quat parentWorldRot = worldRotation(parent()->worldMatrix());
        Rotation = worldRot * glm::inverse(parentWorldRot);
    }
    else
    {
        Rotation = worldRot;
    }

    markDirty();
}

glm::mat4 Transform::localTransform(glm::mat4 mat) const
{
    mat = mat * glm::toMat4(PreRotation);
    mat = glm::translate(mat, Position);
    mat = mat * glm::toMat4(Rotation);
    mat = glm::scale(mat, Scale);

    return mat;
}

glm::vec3 Transform::worldPosition()
{
    return glm::vec3(worldMatrix()[3]);
}

glm::quat Transform::worldRotation(const glm::mat4& mat) const
{
    glm::vec3 unused;
    glm::vec4 unused2;
    glm::quat worldRotation;

    glm::decompose(mat, unused, worldRotation, unused, unused, unused2);

    return worldRotation;
}

