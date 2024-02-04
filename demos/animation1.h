#ifndef ANIMATION1_H
#define ANIMATION1_H

#include "../src/resourceManager.h"
#include "../src/entityModules/controllerModule.h"
#include "../src/entityModules/renderModule.h"
#include "../src/entityModules/gameplayModule.h"
#include "../src/materials/basicMaterial.h"
#include "../src/materials/pbrMaterial.h"
#include "../src/materials/toonMaterial.h"
#include "../src/materials/glassMaterial.h"
#include "../src/shaders/forwardPass/phong/blinnPhongShader.h"
#include "../src/shaders/forwardPass/toon/toonShader.h"
#include "../src/shaders/forwardPass/cook-torrace/pbrShader.h"
#include "../src/shaders/forwardPass/transmittance/glassShader.h"
#include "../src/imgui/imguiWrapper.h"
#include "../src/cubemap.h"
#include "../src/shaders/skybox/skyboxShader.h"

ControllerModule* controllerModule;

void OnGui(){
    glm::quat orientation = ResourceManager::getActiveCamera()->getParent()->getRotation();
    glm::vec3 orientationEuler = glm::degrees(glm::eulerAngles(orientation));
    ImGui::Text("Camera Orientation (Euler): (%f, %f, %f)", orientationEuler.x, orientationEuler.y, orientationEuler.z);
    ImGui::Text("Camera Orientation (Quaternion): (%f, %f, %f, %f)", orientation.x, orientation.y, orientation.z, orientation.w);
}

void setUpScene(ImGuiWrapper* imguiWrapper){

    std::string modelPath = std::string(ASSET_DIR) + "/models/bateman.dae";


    std::string vSkyShaderPath = std::string(SRC_DIR) + "/shaders/skybox/skyboxShader.vert";
    std::string fSkyShaderPath = std::string(SRC_DIR) + "/shaders/skybox/skyboxShader.frag";
    std::string vPhongShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhong.vert";
    std::string fPhongShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhong.frag";

    std::string cubePath = std::string(ASSET_DIR) + "/textures/skybox/clouds_";
    Cubemap* skybox = new Cubemap(cubePath, ".bmp");
    
    DirectionalLight* directionalLight = ResourceManager::loadDirectionalLight(0.1f, glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)));
    PointLight* pointLight = ResourceManager::loadPointLight(0.1f, glm::vec3(3.0f, 3.0f, 3.0f), 1.0f, 0.09f, 0.032f);

    Model* dragon = ResourceManager::loadModel(modelPath.c_str());

    blinnPhongShader* phongShader = new blinnPhongShader(vPhongShaderPath.c_str(), fPhongShaderPath.c_str());\
    SkyboxShader* skyboxShader = new SkyboxShader(vSkyShaderPath.c_str(), fSkyShaderPath.c_str(), skybox);

    BasicMaterial* basicMaterial = new BasicMaterial(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f) , 32.0f);

    ResourceManager::addShader(skyboxShader);
    ResourceManager::addShader(phongShader);

    GameObject* player = ResourceManager::loadGameObject();
    Camera* camera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), Camera_Projection::PERSP, 45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    controllerModule = new ControllerModule(true, true, true);
    player->addModule(controllerModule);
    player->addModule(camera);
    ResourceManager::setActiveCamera(camera);

    GameObject* skyboxObject = ResourceManager::loadGameObject();
    RenderModule* skyboxRenderModule = new RenderModule(nullptr, nullptr, skyboxShader);
    skyboxObject->addModule(skyboxRenderModule);

    GameObject* gameObject = ResourceManager::loadGameObject();
    RenderModule* dragonRenderModule = new RenderModule(dragon, basicMaterial, phongShader);
    GameplayModule* gameplayModule = new GameplayModule();
    gameObject->addModule(gameplayModule);
    gameObject->addModule(dragonRenderModule);
    gameObject->setPosition(glm::vec3(0.0f, 0.0f, -5.0f));


    camera->lookAt(glm::vec3(0.0f, 0.0f, -5.0f));


    imguiWrapper->attachGuiFunction("Camera Properties", OnGui);

}

#endif // ANIMATION1_H