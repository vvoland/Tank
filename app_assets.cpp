#include <unordered_map>

#include "app_assets.hpp"
#include "shader.hpp"

void AppAssets::load(std::string prefix)
{
    DefaultVs = std::make_shared<Shader>(ShaderType::Vertex);
    DefaultFs = std::make_shared<Shader>(ShaderType::Fragment);

    TankBaseTexture = std::make_shared<Texture>();
    TankTracksTexture = std::make_shared<Texture>();

    TankBaseTexture->loadFromFile(prefix + "tank.png");
    TankTracksTexture->loadFromFile(prefix + "tank_tracks.png");

    DefaultVs->loadFromFile(prefix + "default.vertexshader");
    DefaultFs->loadFromFile(prefix + "default.fragmentshader");

    auto tankMeshes = Mesh::loadAllFromFile(prefix + "tank.obj");

    TankBaseMesh = std::make_shared<Mesh>(std::move(tankMeshes["Body_UV.004"]));
    TankTowerMesh = std::make_shared<Mesh>(std::move(tankMeshes["Tower_UV.001"]));
    TankLeftTracksMesh = std::make_shared<Mesh>(std::move(tankMeshes["LeftTrack_Circle.001"]));
    TankRightTracksMesh = std::make_shared<Mesh>(std::move(tankMeshes["RightTrack_Circle.015"]));

    std::exit(0);
}
