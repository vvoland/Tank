#pragma once

#include "mesh.hpp"
#include "texture.hpp"
#include "shader.hpp"

#include <string>
#include <memory>

class AppAssets
{
    public:
        void load(std::string prefix);

        std::shared_ptr<Texture> TankBaseTexture, TankTracksTexture, GrassTexture;
        std::shared_ptr<Texture> SkyboxTexture;
        std::shared_ptr<Mesh> PlaneMesh, SkyboxMesh, CubeMesh;
        std::shared_ptr<Mesh> TankBaseMesh, TankTowerMesh, TankLeftTracksMesh, TankRightTracksMesh;
        std::shared_ptr<Shader> DefaultVs, DefaultFs;
        std::shared_ptr<Shader> SkyboxVs, SkyboxFs;
};


