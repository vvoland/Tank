#include "tank_app.hpp"
#include "material.hpp"
#include "transform.hpp"
#include "renderer.hpp"
#include "camera.hpp"
#include "free_camera_rotation.hpp"
#include "tank_controller.hpp"

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
    SkyboxShader = std::make_shared<ShaderProgram>();
    SkyboxShader->fromShaders({Assets.SkyboxVs, Assets.SkyboxFs});

    Cam = std::make_shared<Camera>();
    auto camObject = createGameObject("Camera");
    camObject->addComponent(Cam);
    auto camTransform = camObject->getComponent<Transform>();

    auto skybox = build3DObject("Skybox", Assets.SkyboxMesh, Assets.SkyboxTexture, nullptr, SkyboxShader);
    auto skyboxTransform = skybox->getComponent<Transform>();
    skyboxTransform->Scale = glm::vec3(10000.0f);
    skyboxTransform->markDirty();

    auto dirLightObject = createGameObject("Light");
    Sun = std::make_shared<Light>(LightType::Directional);
    Sun->range() = 50.0f;
    dirLightObject->addComponent(Sun);
    dirLightObject->getComponent<Transform>()->translateLocal(glm::vec3(-10000, 10000, -5000));

    auto grassObject = createGameObject("Grass");
    auto grassRenderer = std::make_shared<Renderer>(Builder.build(Shader, Assets.PlaneMesh, Assets.GrassTexture));
    grassObject->addComponent(grassRenderer);
//    grassRenderer->Call.material()->refraction() = 1.0f;
//    grassRenderer->Call.material()->refractionCoefficient() = 1.309f;

    RefractionBox = build3DObject("RefractionBox", Assets.CubeMesh, nullptr, nullptr);
    RefractionBox->getComponent<Renderer>()->Call.material()->refraction() = 1.0f;
    RefractionBox->getComponent<Renderer>()->Call.material()->refractionCoefficient() = BoxRefractionCoeff;
    RefractionBox->getComponent<Transform>()->Scale = glm::vec3(5.0f);
    RefractionBox->getComponent<Transform>()->translateLocal(glm::vec3(-15.0f, 2.0f, 10.0f));

    ReflectionBox = build3DObject("ReflectionBox", Assets.CubeMesh, nullptr, nullptr);
    ReflectionBox->getComponent<Renderer>()->Call.material()->reflection() = 1.0f;
    ReflectionBox->getComponent<Transform>()->Scale = glm::vec3(5.0f);
    ReflectionBox->getComponent<Transform>()->translateLocal(glm::vec3(15.0f, 5.0f, 10.0f));

    TankBase = createGameObject("Tank");
    auto baseRenderer = std::make_shared<Renderer>(Builder.build(Shader, Assets.TankBaseMesh, Assets.TankBaseTexture));
    TankBase->addComponent(baseRenderer);
    auto tankTransform = TankBase->getComponent<Transform>();
    tankTransform->Position = glm::vec3(0.0f, 0.0f, 0.0f);
    tankTransform->Scale = glm::vec3(3.0f);


    TankTower = build3DObject("Tower", Assets.TankTowerMesh, Assets.TankBaseTexture, TankBase);
    TankLeftTrack = build3DObject("LeftTrack", Assets.TankLeftTracksMesh, Assets.TankTracksTexture, TankBase);
    TankRightTrack = build3DObject("RightTrack", Assets.TankRightTracksMesh, Assets.TankTracksTexture, TankBase);

    TankTower->getComponent<Renderer>()->Call.material()->reflection() = 0.1f;
    TankBase->getComponent<Renderer>()->Call.material()->reflection() = 0.1f;

    TankControl = std::make_shared<TankController>(Input, TankTower->getComponent<Transform>(),
            TankLeftTrack->getComponent<Renderer>(), TankRightTrack->getComponent<Renderer>());
    CameraMovement = std::make_shared<FreeCameraMovement>(Input);
    CameraRotation = std::make_shared<FreeCameraRotation>(Input);

    TankBase->addComponent(TankControl);
    camObject->addComponent(CameraMovement);
    camObject->addComponent(CameraRotation);

    setTankControl(true);

    camTransform->setParent(tankTransform);
}

void TankApp::setTankControl(bool tankControl) const
{
    auto camTransform = Cam->Owner->getComponent<Transform>();
    if(!tankControl)
    {
        camTransform->PreRotation = glm::quat();
    }
    else
    {
        camTransform->Position = glm::vec3(0.0f, 4.5f, -4.5f);
        camTransform->PreRotation = TankControl->TargetTowerRotation;
        camTransform->Rotation = glm::quat(glm::radians(glm::vec3(150.0f, 0.0f, 180.0f)));
    }
    camTransform->markDirty();

    TankControl->Enabled = tankControl;
    CameraRotation->rotationMode() =
            tankControl ? FreeCameraRotation::RotationMode::Parent : FreeCameraRotation::RotationMode::Self;
    CameraRotation->Enabled = true;
    CameraMovement->Enabled = !tankControl;
}

std::shared_ptr<GameObject> TankApp::build3DObject(const std::string& name,
        std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture, std::shared_ptr<GameObject> parent,
        std::shared_ptr<ShaderProgram> shader)
{
    auto obj = createGameObject(name);
    if(shader == nullptr)
        shader = Shader;
    auto drawCall = Builder.build(shader, mesh, texture);
    drawCall.material()->setSkybox(Assets.SkyboxTexture);
    auto baseRenderer = std::make_shared<Renderer>(drawCall);
    obj->addComponent(baseRenderer);
    if(parent != nullptr)
    {
        auto transform = obj->getComponent<Transform>();
        transform->setParent(parent->getComponent<Transform>());
    }

    return obj;
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

    if(ImGui::SliderFloat("Box refraction coefficient", &BoxRefractionCoeff, 1.0f, 2.0f))
    {
        RefractionBox->getComponent<Renderer>()->Call.material()->refractionCoefficient() = BoxRefractionCoeff;
    }
    ImGui::Text("Press SPACE to toggle tank control");
    ImGui::End();
}

void TankApp::onDraw()
{
    float dt = DeltaTime;
    Time += dt;
    Input.update(dt);

    if(Input.isKeyPressed(' '))
    {
        setTankControl(!TankControl->Enabled);
    }

    CameraRotation->update(dt);
    CameraMovement->update(dt);
    TankControl->update(dt);
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

    Input.lateUpdate();
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

