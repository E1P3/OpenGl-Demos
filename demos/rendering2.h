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

void setUpScene(){

    std::string dragonPath = "../../assets/models/dragon.fbx";
    std::string potPath = "../../assets/models/teapot.fbx";
    std::string spherePath = "../../assets/models/defaultSphere.fbx";

    std::string vSkyShaderPath = "../../src/shaders/skybox/skyboxShader.vert";
    std::string fSkyShaderPath = "../../src/shaders/skybox/skyboxShader.frag";
    std::string vGlassShaderPath = "../../src/shaders/forwardPass/transmittance/glassShader.vert";
    std::string fGlassShaderPath = "../../src/shaders/forwardPass/transmittance/glassShader.frag";

    std::string cubePath = "../../assets/textures/skybox/clouds/clouds_";
    Cubemap* skybox = new Cubemap(cubePath, ".bmp");
    skyboxes.push_back(skybox);
    
    cubePath = "../../assets/textures/skybox/night/night_";
    Cubemap* skybox1 = new Cubemap(cubePath, ".png");
    skyboxes.push_back(skybox1);

    cubePath = "../../assets/textures/skybox/colorclouds/bluecloud_";
    Cubemap* skybox2 = new Cubemap(cubePath, ".jpg");
    skyboxes.push_back(skybox2);

    cubePath = "../../assets/textures/skybox/urban/CNTower/";
    Cubemap* skybox3 = new Cubemap(cubePath);
    skyboxes.push_back(skybox3);

    cubePath = "../../assets/textures/skybox/urban/ForbiddenCity/";
    Cubemap* skybox4 = new Cubemap(cubePath);
    skyboxes.push_back(skybox4);

    cubePath = "../../assets/textures/skybox/urban/GamlaStan/";
    Cubemap* skybox5 = new Cubemap(cubePath);
    skyboxes.push_back(skybox5);

    cubePath = "../../assets/textures/skybox/urban/GamlaStan2/";
    Cubemap* skybox6 = new Cubemap(cubePath);
    skyboxes.push_back(skybox6);

    cubePath = "../../assets/textures/skybox/urban/Medborgarplatsen/";
    Cubemap* skybox7 = new Cubemap(cubePath);
    skyboxes.push_back(skybox7);

    cubePath = "../../assets/textures/skybox/urban/SaintLazarusChurch/";
    Cubemap* skybox8 = new Cubemap(cubePath);
    skyboxes.push_back(skybox8);

    cubePath = "../../assets/textures/skybox/urban/SaintLazarusChurch2/";
    Cubemap* skybox9 = new Cubemap(cubePath);
    skyboxes.push_back(skybox9);

    cubePath = "../../assets/textures/skybox/urban/SaintLazarusChurch3/";
    Cubemap* skybox10 = new Cubemap(cubePath);
    skyboxes.push_back(skybox10);

    cubePath = "../../assets/textures/skybox/urban/Sodermalmsallen/";
    Cubemap* skybox11 = new Cubemap(cubePath);
    skyboxes.push_back(skybox11);

    cubePath = "../../assets/textures/skybox/urban/Sodermalmsallen2/";
    Cubemap* skybox12 = new Cubemap(cubePath);
    skyboxes.push_back(skybox12);

    cubePath = "../../assets/textures/skybox/urban/UnionSquare/";
    Cubemap* skybox13 = new Cubemap(cubePath);
    skyboxes.push_back(skybox13);

    DirectionalLight* directionalLight = ResourceManager::loadDirectionalLight(1.0f, glm::vec3(0.0f, -1.0f, 0.0f));
    PointLight* pointLight = ResourceManager::loadPointLight(0.1f, glm::vec3(3.0f, 3.0f, 3.0f), 1.0f, 0.09f, 0.032f);

    Model* dragon = ResourceManager::loadModel(dragonPath.c_str());
    Model* pot = ResourceManager::loadModel(potPath.c_str());
    Model* sphere = ResourceManager::loadModel(spherePath.c_str());

    glassShader = new GlassShader(vGlassShaderPath.c_str(), fGlassShaderPath.c_str(), skybox);
    skyboxShader = new SkyboxShader(vSkyShaderPath.c_str(), fSkyShaderPath.c_str(), skybox);

    GlassMaterial* glassMaterial = new GlassMaterial(0.66f, 5.0f, 0.02f);

    ResourceManager::addShader(skyboxShader);
    ResourceManager::addShader(glassShader);

    Camera* camera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), Camera_Projection::PERSP, 45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    ResourceManager::setActiveCamera(camera);
    camera->setMode(Camera_Mode::FREE);

    GameObject* skyboxObject = ResourceManager::loadGameObject();
    RenderModule* skyboxRenderModule = new RenderModule(nullptr, nullptr, skyboxShader);
    skyboxObject->addModule(skyboxRenderModule);

    GameObject* dragonObject = ResourceManager::loadGameObject();
    RenderModule* dragonRenderModule = new RenderModule(dragon, glassMaterial, glassShader);
    GameplayModule* dragonGameplayModule = new GameplayModule();
    dragonObject->addModule(dragonRenderModule);
    dragonObject->addModule(dragonGameplayModule);
    dragonObject->setPosition(glm::vec3(-3.0f, 0.0f, -10.0f));
    dragonObject->Scale(glm::vec3(0.15f, 0.15f, 0.15f));

    GameObject* potObject = ResourceManager::loadGameObject();
    RenderModule* potRenderModule = new RenderModule(pot, glassMaterial, glassShader);
    GameplayModule* potGameplayModule = new GameplayModule();
    potObject->addModule(potRenderModule);
    potObject->addModule(potGameplayModule);
    potObject->setPosition(glm::vec3(3.0f, 0.0f, -10.0f));

    GameObject* sphereObject = ResourceManager::loadGameObject();
    RenderModule* sphereRenderModule = new RenderModule(sphere, glassMaterial, glassShader);
    GameplayModule* sphereGameplayModule = new GameplayModule();
    sphereObject->addModule(sphereRenderModule);
    sphereObject->addModule(sphereGameplayModule);
    sphereObject->setPosition(glm::vec3(0.0f, 0.5f, -10.0f));
    sphereObject->Scale(glm::vec3(0.7f, 0.7f, 0.7f));

    camera->setTarget(sphereObject);
    camera->setMode(Camera_Mode::FREE);
    camera->lookAt(sphereObject->getPosition(), glm::vec3(0.0f, 1.0f, 0.0f));

    ImGuiWrapper::attachGuiFunction("Material Properties", [glassMaterial](){glassMaterial->OnGui();});
    ImGuiWrapper::attachGuiFunction("Skybox Properties", OnGui);

}

#endif // ANIMATION1_H