#include "tank_app.hpp"
#include "material.hpp"
#include "transform.hpp"
#include "renderer.hpp"
#include "camera.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>

TankApp::TankApp()
{
}

void TankApp::onInit()
{
    Assets.load("../assets/");
    Shader = std::make_shared<ShaderProgram>();
    Shader->fromShaders({Assets.DefaultVs, Assets.DefaultFs});

    Cam = std::make_shared<Camera>(Input);
    auto camObject = createGameObject("Camera");
    camObject->addComponent(Cam);
    auto camTransform = camObject->getComponent<Transform>();
    camTransform->translateLocal(glm::vec3(8.0f, 3.0f, -10.0f));

    auto dirLightObject = createGameObject("DirectionalLight");
    Sun = std::make_shared<Light>(LightType::Directional);
    dirLightObject->addComponent(Sun);
    dirLightObject->getComponent<Transform>()->translateLocal(glm::vec3(1000, 1000, 0));
    dirLightObject->getComponent<Transform>()->Scale = glm::vec3(10, 10, 10);


    auto towerDrawcall = Builder.build(Shader, Assets.TowerMesh, Assets.TowerTexture);
    float size = 20.0f;
    int amount = 8;
    for(int x = -amount / 2; x < amount / 2; x++)
    for(int y = -amount / 2; y < amount / 2; y++)
    {
        Tower = createGameObject("Tower");
        std::shared_ptr<Renderer> towerRenderer = std::make_shared<Renderer>(towerDrawcall);
        Tower->addComponent(towerRenderer);
        auto towerTransform = Tower->getComponent<Transform>();
        towerTransform->Scale = glm::vec3(5.00f);
        towerTransform->Position = glm::vec3(x * size, 0.0f, y * -size);
    }
    Cam->copyRotationFromTransform();
}

std::shared_ptr<GameObject> TankApp::createGameObject(const std::string& name)
{
    std::shared_ptr<GameObject> obj = std::make_shared<GameObject>(name);
    GameObjects.push_back(obj);

    std::shared_ptr<Transform> transform = std::make_shared<Transform>();
    obj->addComponent(transform);

    return obj;
}

void TankApp::onGui()
{
    ImGui::Begin("Controls");

    auto camTransform = Cam->Owner->getComponent<Transform>();
    ImGui::LabelText("Position", "%f %f %f", camTransform->Position.x, camTransform->Position.y, camTransform->Position.z);
    ImGui::End();
}

void TankApp::onDraw()
{
    float dt = DeltaTime;
    Time += dt;
    Input.update(dt);
    Cam->update(dt);

    View = Cam->view();

    std::vector<std::shared_ptr<GameObject>> parents;
    std::vector<std::shared_ptr<Light>> lights;
    for(auto obj : GameObjects)
    {
        if(obj->hasComponent<Transform>())
        {
            auto transform = obj->getComponent<Transform>();
            if(transform->parent() == nullptr)
            {
                parents.emplace_back(obj);
            }
            auto light = obj->getComponent<Light>();
            if(light != nullptr && light->Enabled)
            {
                lights.emplace_back(light);
            }
        }
    }

    if(Wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for(auto obj : parents)
    {
        drawObjectAndChildren(obj.get(), lights);
    }
    if(Wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void TankApp::drawObjectAndChildren(GameObject* obj, const std::vector<std::shared_ptr<Light>>& lights)
{
    auto transform = obj->getComponent<Transform>();

    if(obj->hasComponent<Renderer>())
    {
        auto renderer = obj->getComponent<Renderer>();
        Mvp mvp;
        mvp.Projection = Projection;
        mvp.View = View;
        mvp.Model = transform->worldMatrix();
        mvp.Mvp = Projection * View * transform->worldMatrix();

        renderer->Call.draw(mvp, lights);
    }

    for(auto childObj : transform->children())
    {
        drawObjectAndChildren(childObj->Owner, lights);
    }
}

void TankApp::onInput(KeyInputType type, KeyCode key)
{
    switch(type)
    {
        case KeyInputType::Down:
            printf("[Input] %c is down\n", key);
            Input.setKeyDown(key, true);
            break;
        case KeyInputType::Up:
            printf("[Input] %c is up\n", key);
            Input.setKeyDown(key, false);
            break;
    }
}

void TankApp::onMouse(double x, double y)
{
    Input.setMousePosition(x, y);
}

void TankApp::onViewportChange(int width, int height)
{
    float aspectRatio = float(width) / height;
    Projection = glm::perspective(45.0f, aspectRatio, 0.1f, 10000.0f);

}

