#pragma once

#include "../../src/resourceManager.h"
#include "../../src/entityModules/controllerModule.h"
#include "../../src/entityModules/renderModule.h"
#include "../../src/entityModules/gameplayModule.h"
#include "../../src/imgui/imguiWrapper.h"
#include "../../src/shaders/forwardPass/phong/blinnPhongShader.h"
#include "../../src/materials/basicMaterial.h"
#include "roamShader.h"
#include "terrain_patch.hpp"

void setUpScene(){
    std::string vShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhong.vert";
    std::string fShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhong.frag";

    float LODScaling; float errorMargin;

    // Ireland Height Map
    // std::string heightmapPath = std::string(ASSET_DIR) + "/textures/ireland-heightmap.jpg"; 
    // TerrainPatch* terrainPatch = new TerrainPatch(heightmapPath.c_str(), 0, 0, true);
    // LODScaling = 1024.0f; errorMargin = 0.01f;



    // Simplex noise generated Height Map
    std::string heightmapPath = std::string(ASSET_DIR) + "/misc/simplex_noise_1024.map";
    TerrainPatch* terrainPatch = new TerrainPatch(heightmapPath.c_str(), 0, 0, false);
    LODScaling = 216.0f; errorMargin = 0.0045f;

    
    DirectionalLight* directionalLight = ResourceManager::loadDirectionalLight(0.1f, glm::vec3(0.0f, 0.0f, 1.0f));
    PointLight* pointLight = ResourceManager::loadPointLight(0.1f, glm::vec3(0.0f, 50.0f, 300.0f), 1.0f, 0.09f, 0.032f);
    
    blinnPhongShader *phongShader = new blinnPhongShader(vShaderPath.c_str(), fShaderPath.c_str());
    ResourceManager::addShader(phongShader);

    RoamShader* roamShader = new RoamShader(vShaderPath.c_str(), fShaderPath.c_str(), terrainPatch, LODScaling, errorMargin);
    ResourceManager::addShader(roamShader);

    Model* sphereModel = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/defaultSphere.fbx").c_str());
    BasicMaterial* sphereMaterial = new BasicMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f,0.0f,0.0f), 100.0f);
    
    Camera* camera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), Camera_Projection::PERSP, 45.0f, 16.0f / 9.0f, 0.1f, 10000.0f);
    ResourceManager::setActiveCamera(camera);
    camera->setPosition(glm::vec3(0.0f, 15.0f, 46.0f));
    camera->setMode(Camera_Mode::FREE);

    GameObject* cameraOrbit = ResourceManager::loadGameObject();
    cameraOrbit->setPosition(glm::vec3(500.0f, -200.0f, -500.0f));

    GameObject* target = ResourceManager::loadGameObject();
    target->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));


    GameObject* terrain = ResourceManager::loadGameObject();
    RenderModule* renderModule = new RenderModule(nullptr, sphereMaterial, roamShader);
    terrain->addModule(renderModule);
    terrain->setPosition(glm::vec3(0.0f, -200.0f, 0.0f));
    terrain->setRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
    terrain->setScale(glm::vec3(1000.0f, 1000.0f, 50.0f));


    //roamShader->setTarget(target);

    camera->setTpsOffset(900.0f);
    camera->setPosition(glm::vec3(0.0f, 0.0f, 10.0f));
    camera->setTarget(cameraOrbit);
    camera->setMode(Camera_Mode::FREE);
    camera->setSpeed(0.5f);

    //ImGuiWrapper::attachGuiFunction("Target", [target](){target->OnGui();});
    //ImGuiWrapper::attachGuiFunction("Terrain", [terrain](){terrain->OnGui();});
    ImGuiWrapper::attachGuiFunction("Shader", [roamShader](){roamShader->OnGui();});
    ImGuiWrapper::attachGuiFunction("Camera", [camera](){camera->OnGui();});
    //ImGuiWrapper::attachGuiFunction("Camera Orbit", [cameraOrbit](){cameraOrbit->OnGui();});
    
}
