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

Model* sphereModel;
RoamShader* roamShader;
BasicMaterial* sphereMaterial;

void setUpScene(){
    std::string vShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhong.vert";
    std::string fShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhong.frag";
    std::string heightmapPath = std::string(ASSET_DIR) + "/textures/ireland-heightmap.jpg";
    TerrainPatch* terrainPatch = new TerrainPatch(heightmapPath.c_str());

    DirectionalLight* directionalLight = ResourceManager::loadDirectionalLight(0.1f, glm::vec3(0.0f, 0.0f, 1.0f));
    PointLight* pointLight = ResourceManager::loadPointLight(0.1f, glm::vec3(0.0f, 50.0f, 300.0f), 1.0f, 0.09f, 0.032f);

    roamShader = new RoamShader(vShaderPath.c_str(), fShaderPath.c_str(), terrainPatch);
    ResourceManager::addShader(roamShader);

    Model* sphereModel = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/highpoly.fbx").c_str());
    sphereMaterial = new BasicMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f,0.0f,0.0f), 100.0f);
    
    Camera* camera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), Camera_Projection::PERSP, 45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    ResourceManager::setActiveCamera(camera);
    camera->setPosition(glm::vec3(0.0f, 15.0f, 46.0f));
    camera->setMode(Camera_Mode::FREE);

    GameObject* sphere = ResourceManager::loadGameObject();
    RenderModule* renderModule = new RenderModule(nullptr, sphereMaterial, roamShader);
    sphere->addModule(renderModule);

    camera->setTpsOffset(15.0f);
    camera->setTarget(sphere);
    camera->setMode(Camera_Mode::TPS);
    
}
