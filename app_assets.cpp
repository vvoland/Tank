#include <unordered_map>

#include "app_assets.hpp"
#include "shader.hpp"

void AppAssets::load(std::string prefix)
{
    DefaultVs = std::make_shared<Shader>(ShaderType::Vertex);
    DefaultFs = std::make_shared<Shader>(ShaderType::Fragment);

    TankBaseTexture = std::make_shared<Texture>();
    TankTracksTexture = std::make_shared<Texture>();

    TankBaseTexture->loadFromFile(prefix + "tank.png", TextureFlags::MirroredRepeat);
    TankTracksTexture->loadFromFile(prefix + "tank_tracks.png");

    DefaultVs->loadFromFile(prefix + "default.vertexshader");
    DefaultFs->loadFromFile(prefix + "default.fragmentshader");

    auto tankMeshes = Mesh::loadAllFromFile(prefix + "tank2.obj");

    PlaneMesh = std::make_shared<Mesh>();
    PlaneMesh->loadFromFile(prefix + "plane.obj");
    GrassTexture = std::make_shared<Texture>();
    GrassTexture->loadFromFile(prefix + "grass.png");

    TankBaseMesh = std::make_shared<Mesh>(std::move(tankMeshes["MC-1_Base_UV.004"]));
    TankTowerMesh = std::make_shared<Mesh>(std::move(tankMeshes["MC-1_Base.001_Tank_Tower"]));
    TankLeftTracksMesh = std::make_shared<Mesh>(std::move(tankMeshes["MC-1_LeftTrack_Circle.052"]));
    TankRightTracksMesh = std::make_shared<Mesh>(std::move(tankMeshes["MC-1_RightTrack_Circle.052"]));
}
