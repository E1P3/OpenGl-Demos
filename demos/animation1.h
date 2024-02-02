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

void OnGui(){
    ImGui::Begin("Animation1");
    ImGui::Text("This is a test");
    ImGui::End();
}

void setUpScene(ImGuiWrapper* imguiWrapper){

    std::string dragonPath = std::string(ASSET_DIR) + "/models/dragon.fbx";

    std::string vSkyShaderPath = std::string(SRC_DIR) + "/shaders/skybox/skyboxShader.vert";
    std::string fSkyShaderPath = std::string(SRC_DIR) + "/shaders/skybox/skyboxShader.frag";
    std::string vGlassShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/transmittance/glassShader.vert";
    std::string fGlassShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/transmittance/glassShader.frag";

    std::string cubePath = std::string(ASSET_DIR) + "/textures/skybox/clouds_";
    std::string faces[] = {
        cubePath + "right.bmp",
        cubePath + "left.bmp",
        cubePath + "top.bmp",
        cubePath + "bottom.bmp",
        cubePath + "front.bmp",
        cubePath + "back.bmp"
    };

    Cubemap* skybox = new Cubemap(faces);
    std::vector<Cubemap*> skyboxes;
    skyboxes.push_back(skybox);

    DirectionalLight* directionalLight = ResourceManager::loadDirectionalLight(0.1f, glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)));
    PointLight* pointLight = ResourceManager::loadPointLight(0.1f, glm::vec3(3.0f, 3.0f, 3.0f), 1.0f, 0.09f, 0.032f);

    Model* dragon = ResourceManager::loadModel(dragonPath.c_str());

    GlassShader* glassShader = new GlassShader(vGlassShaderPath.c_str(), fGlassShaderPath.c_str(), skybox);
    SkyboxShader* skyboxShader = new SkyboxShader(vSkyShaderPath.c_str(), fSkyShaderPath.c_str(), skyboxes);

    GlassMaterial* glassMaterial = new GlassMaterial(1.5f, 5.0f, 0.01f);

    ResourceManager::addShader(skyboxShader);
    ResourceManager::addShader(glassShader);

    GameObject* player = ResourceManager::loadGameObject();
    Camera* camera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), Camera_Projection::PERSP, 45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    ControllerModule* controllerModule = new ControllerModule(true, true, true);
    player->addModule(controllerModule);
    player->addModule(camera);
    ResourceManager::setActiveCamera(camera);

    GameObject* skyboxObject = ResourceManager::loadGameObject();
    RenderModule* skyboxRenderModule = new RenderModule(nullptr, nullptr, skyboxShader);
    skyboxObject->addModule(skyboxRenderModule);

    GameObject* dragonObject = ResourceManager::loadGameObject();
    RenderModule* dragonRenderModule = new RenderModule(dragon, glassMaterial, glassShader);
    GameplayModule* dragonGameplayModule = new GameplayModule();
    dragonObject->addModule(dragonRenderModule);
    dragonObject->addModule(dragonGameplayModule);
    dragonObject->setPosition(glm::vec3(0.0f, 0.0f, -5.0f));
    dragonObject->Scale(glm::vec3(0.1f, 0.1f, 0.1f));

    camera->lookAt(dragonObject->getPosition());


    imguiWrapper->attachGuiFunction("Material Properties", [glassMaterial](){glassMaterial->OnGui();});

}

#endif // ANIMATION1_H