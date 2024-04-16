#ifndef TEST_H
#define TEST_H

#include "../src/resourceManager.h"
#include "../src/entityModules/controllerModule.h"
#include "../src/entityModules/renderModule.h"
#include "../src/entityModules/gameplayModule.h"
#include "../src/imgui/imguiWrapper.h"
#include "../src/shaders/forwardPass/phong/blinnPhongShader.h"
#include "../src/materials/basicMaterial.h"

Model* sphereModel;
blinnPhongShader* phongShader;
BasicMaterial* sphereMaterial;

void setUpScene(){
    std::string vShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhong.vert";
    std::string fShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhong.frag";
    
    DirectionalLight* directionalLight = ResourceManager::loadDirectionalLight(0.1f, glm::vec3(0.0f, 0.0f, 1.0f));
    PointLight* pointLight = ResourceManager::loadPointLight(0.1f, glm::vec3(0.0f, 50.0f, 300.0f), 1.0f, 0.09f, 0.032f);

    phongShader = new blinnPhongShader(vShaderPath.c_str(), fShaderPath.c_str());
    ResourceManager::addShader(phongShader);

    Model* sphereModel = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/highpoly.fbx").c_str());
    sphereMaterial = new BasicMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f,0.0f,0.0f), 100.0f);
    
    Camera* camera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), Camera_Projection::PERSP, 45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    ResourceManager::setActiveCamera(camera);
    camera->setPosition(glm::vec3(0.0f, 15.0f, 46.0f));
    camera->setMode(Camera_Mode::FREE);

    GameObject* sphere = ResourceManager::loadGameObject();
    RenderModule* renderModule = new RenderModule(sphereModel, sphereMaterial, phongShader);
    sphere->addModule(renderModule);

    camera->setTpsOffset(15.0f);
    camera->setTarget(sphere);
    camera->setMode(Camera_Mode::TPS);
    
}

#endif // TEST_H    