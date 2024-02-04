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

std::vector<Cubemap*> skyboxes;
GlassShader* glassShader;
SkyboxShader* skyboxShader;

int skyboxIndex = 0;

void OnGui(){
    if(ImGui::Button("Change Skybox")){
        skyboxIndex = (skyboxIndex + 1) % skyboxes.size();
        glassShader->setCubeMap(skyboxes[skyboxIndex]);
        skyboxShader->setCubeMap(skyboxes[skyboxIndex]);
    }
}

void setUpScene(ImGuiWrapper* imguiWrapper){

    std::string dragonPath = std::string(ASSET_DIR) + "/models/dragon.fbx";
    std::string potPath = std::string(ASSET_DIR) + "/models/teapot.fbx";
    std::string spherePath = std::string(ASSET_DIR) + "/models/sphere.fbx";

    std::string vSkyShaderPath = std::string(SRC_DIR) + "/shaders/skybox/skyboxShader.vert";
    std::string fSkyShaderPath = std::string(SRC_DIR) + "/shaders/skybox/skyboxShader.frag";
    std::string vGlassShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/transmittance/glassShader.vert";
    std::string fGlassShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/transmittance/glassShader.frag";

    std::string cubePath = std::string(ASSET_DIR) + "/textures/skybox/clouds_";
    Cubemap* skybox = new Cubemap(cubePath, ".bmp");
    skyboxes.push_back(skybox);
    
    cubePath = std::string(ASSET_DIR) + "/textures/skybox/night_";
    Cubemap* skybox1 = new Cubemap(cubePath, ".png");
    skyboxes.push_back(skybox1);

    cubePath = std::string(ASSET_DIR) + "/textures/skybox/bluecloud_";
    Cubemap* skybox2 = new Cubemap(cubePath, ".jpg");
    skyboxes.push_back(skybox2);

    cubePath = std::string(ASSET_DIR) + "/textures/skybox/browncloud_";
    Cubemap* skybox3 = new Cubemap(cubePath, ".jpg");
    skyboxes.push_back(skybox3);

    cubePath = std::string(ASSET_DIR) + "/textures/skybox/graycloud_";
    Cubemap* skybox4 = new Cubemap(cubePath, ".jpg");
    skyboxes.push_back(skybox4);

    cubePath = std::string(ASSET_DIR) + "/textures/skybox/yellowcloud_";
    Cubemap* skybox5 = new Cubemap(cubePath, ".jpg");
    skyboxes.push_back(skybox5);

    DirectionalLight* directionalLight = ResourceManager::loadDirectionalLight(0.1f, glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)));
    PointLight* pointLight = ResourceManager::loadPointLight(0.1f, glm::vec3(3.0f, 3.0f, 3.0f), 1.0f, 0.09f, 0.032f);

    Model* dragon = ResourceManager::loadModel(dragonPath.c_str());
    Model* pot = ResourceManager::loadModel(potPath.c_str());
    Model* sphere = ResourceManager::loadModel(spherePath.c_str());

    glassShader = new GlassShader(vGlassShaderPath.c_str(), fGlassShaderPath.c_str(), skybox);
    skyboxShader = new SkyboxShader(vSkyShaderPath.c_str(), fSkyShaderPath.c_str(), skybox);

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
    dragonObject->setPosition(glm::vec3(-3.0f, 0.0f, -5.0f));
    dragonObject->Scale(glm::vec3(0.1f, 0.1f, 0.1f));

    GameObject* potObject = ResourceManager::loadGameObject();
    RenderModule* potRenderModule = new RenderModule(pot, glassMaterial, glassShader);
    GameplayModule* potGameplayModule = new GameplayModule();
    potObject->addModule(potRenderModule);
    potObject->addModule(potGameplayModule);
    potObject->setPosition(glm::vec3(3.0f, 0.0f, -5.0f));

    GameObject* sphereObject = ResourceManager::loadGameObject();
    RenderModule* sphereRenderModule = new RenderModule(sphere, glassMaterial, glassShader);
    GameplayModule* sphereGameplayModule = new GameplayModule();
    sphereObject->addModule(sphereRenderModule);
    sphereObject->addModule(sphereGameplayModule);
    sphereObject->setPosition(glm::vec3(0.0f, 0.0f, -5.0f));


    camera->lookAt(sphereObject->getPosition());


    imguiWrapper->attachGuiFunction("Material Properties", [glassMaterial](){glassMaterial->OnGui();});
    imguiWrapper->attachGuiFunction("Skybox Properties", OnGui);

}

#endif // ANIMATION1_H