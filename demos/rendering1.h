#ifndef RENDERING1_H
#define RENDERING1_H

// Scene to showcase different shading techniques

#include "../src/resourceManager.h"
#include "../src/entityModules/controllerModule.h"
#include "../src/entityModules/renderModule.h"
#include "../src/entityModules/gameplayModule.h"
#include "../src/materials/basicMaterial.h"
#include "../src/materials/pbrMaterial.h"
#include "../src/materials/toonMaterial.h"
#include "../src/shaders/forwardPass/phong/blinnPhongShader.h"
#include "../src/shaders/forwardPass/toon/toonShader.h"
#include "../src/shaders/forwardPass/cook-torrace/pbrShader.h"
#include "../src/imgui/imguiWrapper.h"

void setUpScene(){

    std::string potPath = std::string(ASSET_DIR) + "/models/teapot.fbx";
    std::string spherePath = std::string(ASSET_DIR) + "/models/defaultSphere.fbx";
    std::string dragonPath = std::string(ASSET_DIR) + "/models/dragon.fbx";
    std::string planePath = std::string(ASSET_DIR) + "/models/defaultPlane.fbx";

    std::string vShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhong.vert";
    std::string fShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhong.frag";
    std::string vToonShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/toon/toonShader.vert";
    std::string fToonShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/toon/toonShader.frag";
    std::string vPBRShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/cook-torrace/pbrShader.vert";
    std::string fPBRShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/cook-torrace/pbrShader.frag";

    DirectionalLight* directionalLight = ResourceManager::loadDirectionalLight(0.1f, glm::vec3(0.0f, 0.0f, 1.0f));
    PointLight* pointLight = ResourceManager::loadPointLight(0.1f, glm::vec3(3.0f, 3.0f, 3.0f), 1.0f, 0.09f, 0.032f);

    Shader* shader = new blinnPhongShader(vShaderPath.c_str(), fShaderPath.c_str());
    Shader* toonShader = new ToonShader(vToonShaderPath.c_str(), fToonShaderPath.c_str());
    Shader* pbrShader = new PBRShader(vPBRShaderPath.c_str(), fPBRShaderPath.c_str());

    ResourceManager::addShader(shader);
    ResourceManager::addShader(toonShader);
    ResourceManager::addShader(pbrShader);

    BasicMaterial *phongMaterial = new BasicMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.5f,0.5f,0.5f), glm::vec3(1.0f,1.0f,1.0f), 32.0f);
    ToonMaterial *toonMaterial = new ToonMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.5f,0.5f,0.5f), glm::vec3(1.0f,1.0f,1.0f), 0.5f, 4, 0.019f);
    BasicMaterial *floorMaterial = new BasicMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.5f,0.5f,0.5f), glm::vec3(1.0f,1.0f,1.0f), 32.0f);
    PBRMaterial *pbrMaterial = new PBRMaterial(glm::vec3(1.0f,1.0f,1.0f), 0.1f, 0.3f, 0.1f);

    Model* sphere = ResourceManager::loadModel(spherePath.c_str());
    Model* pot = ResourceManager::loadModel(potPath.c_str());
    Model* dragon = ResourceManager::loadModel(dragonPath.c_str());
    Model* plane = ResourceManager::loadModel(planePath.c_str());

    Camera* camera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), Camera_Projection::PERSP, 45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    ResourceManager::setActiveCamera(camera);
    camera->setMode(Camera_Mode::FREE);

    GameObject* pot1 = ResourceManager::loadGameObject();
    RenderModule* renderModule = new RenderModule(pot, phongMaterial, shader);
    GameplayModule* gameplayModule = new GameplayModule();
    pot1->addModule(renderModule);
    pot1->addModule(gameplayModule);
    pot1->Translate(glm::vec3(0.0f, 0.0f, -10.0f));

    GameObject* pot2 = ResourceManager::loadGameObject();
    RenderModule* renderModule1 = new RenderModule(pot, toonMaterial, toonShader);
    GameplayModule* gameplayModule1 = new GameplayModule();
    pot2->addModule(renderModule1);
    pot2->addModule(gameplayModule1);
    pot2->Translate(glm::vec3(3.0f, 0.0f, -10.0f));
    

    GameObject* pot3 = ResourceManager::loadGameObject();
    RenderModule* renderModule2 = new RenderModule(pot, pbrMaterial, pbrShader);
    GameplayModule* gameplayModule2 = new GameplayModule();
    pot3->addModule(renderModule2);
    pot3->addModule(gameplayModule2);
    pot3->Translate(glm::vec3(-3.0f, 0.0f, -10.0f));

    GameObject* sphere1 = ResourceManager::loadGameObject();
    RenderModule* renderModule3 = new RenderModule(sphere, phongMaterial, shader);
    GameplayModule* gameplayModule3 = new GameplayModule();
    sphere1->addModule(renderModule3);
    sphere1->addModule(gameplayModule3);
    sphere1->Translate(glm::vec3(0.0f, 3.0f, -10.0f));

    GameObject* sphere2 = ResourceManager::loadGameObject();
    RenderModule* renderModule4 = new RenderModule(sphere, toonMaterial, toonShader);
    GameplayModule* gameplayModule4 = new GameplayModule();
    sphere2->addModule(renderModule4);
    sphere2->addModule(gameplayModule4);
    sphere2->Translate(glm::vec3(3.0f, 3.0f, -10.0f));
    

    GameObject* sphere3 = ResourceManager::loadGameObject();
    RenderModule* renderModule5 = new RenderModule(sphere, pbrMaterial, pbrShader);
    GameplayModule* gameplayModule5 = new GameplayModule();
    sphere3->addModule(renderModule5);
    sphere3->addModule(gameplayModule5);
    sphere3->Translate(glm::vec3(-3.0f, 3.0f, -10.0f));

    GameObject* dragon1 = ResourceManager::loadGameObject();
    RenderModule* renderModule7 = new RenderModule(dragon, phongMaterial, shader);
    GameplayModule* gameplayModule7 = new GameplayModule();
    dragon1->addModule(renderModule7);
    dragon1->addModule(gameplayModule7);
    dragon1->Translate(glm::vec3(0.0f, -3.0f, -10.0f));
    dragon1->setScale(glm::vec3(0.2f, 0.2f, 0.2f));

    GameObject* dragon2 = ResourceManager::loadGameObject();
    RenderModule* renderModule8 = new RenderModule(dragon, toonMaterial, toonShader);
    GameplayModule* gameplayModule8 = new GameplayModule();
    dragon2->addModule(renderModule8);
    dragon2->addModule(gameplayModule8);
    dragon2->Translate(glm::vec3(3.0f, -3.0f, -10.0f));
    dragon2->setScale(glm::vec3(0.2f, 0.2f, 0.2f));

    GameObject* dragon3 = ResourceManager::loadGameObject();
    RenderModule* renderModule9 = new RenderModule(dragon, pbrMaterial, pbrShader);
    GameplayModule* gameplayModule9 = new GameplayModule();
    dragon3->addModule(renderModule9);
    dragon3->addModule(gameplayModule9);
    dragon3->Translate(glm::vec3(-3.0f, -3.0f, -10.0f));
    dragon3->setScale(glm::vec3(0.2f, 0.2f, 0.2f));


    GameObject* floor = ResourceManager::loadGameObject();
    RenderModule* renderModule6 = new RenderModule(plane, floorMaterial, shader);
    floor->addModule(renderModule6);
    floor->setRotation(glm::vec3 (-90.0f, 0.0f, 0.0f));
    floor->setScale(glm::vec3(20.0f, 20.0f, 20.0f));
    floor->Translate(glm::vec3(0.0f, -4.0f, 0.0f));

    camera->setTarget(pot1);
    camera->setMode(Camera_Mode::TPS);

    ImGuiWrapper::attachGuiFunction("Phong Shader", [phongMaterial](){phongMaterial->OnGui();});
    ImGuiWrapper::attachGuiFunction("PBR Shader", [pbrMaterial](){pbrMaterial->OnGui();});
    ImGuiWrapper::attachGuiFunction("Toon Shader", [toonMaterial](){toonMaterial->OnGui();});
}

#endif // RENDERING1_H