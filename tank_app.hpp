#pragma once

#include "app.hpp"
#include "mesh.hpp"
#include "app_assets.hpp"
#include "shader_program.hpp"
#include "draw_call_builder.hpp"
#include "gameobject.hpp"
#include "transform.hpp"
#include "input_system.hpp"
#include "light.hpp"
#include "camera.hpp"

#include <string>
#include <memory>
#include <chrono>

class TankApp : public App
{
    public:
        TankApp();

        void onInit() override;
        void onInput(KeyInputType type, KeyCode key) override;
        void onGui() override;
        void onDraw() override;
        void onMouse(double x, double y) override;

        void onViewportChange(int width, int height) override;

    private:
        AppAssets Assets;
        DrawCallBuilder Builder;
        std::shared_ptr<ShaderProgram> Shader;

        InputSystem Input;

        bool Wireframe = false;
        float Time = 0.0f;

        std::shared_ptr<GameObject> TankBase, TankTower, TankLeftTrack, TankRightTrack;
        std::shared_ptr<Light> Sun;
        std::shared_ptr<Camera> Cam;
        std::vector<std::shared_ptr<GameObject>> GameObjects;

        glm::mat4 Projection, View;

        std::shared_ptr<GameObject> createGameObject(const std::string& name);

        void drawObjectAndChildren(GameObject* obj, const std::vector<std::shared_ptr<Light>>& lights);
};

