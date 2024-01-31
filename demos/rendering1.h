#ifndef RENDERING1_H
#define RENDERING1_H

// Scene to showcase different shading techniques

#include "../src/resourceManager.h"
#include "../src/entityModules/controllerModule.h"
#include "../src/entityModules/renderModule.h"
#include "../src/entityModules/gameplayModule.h"
#include "../src/shaders/forwardPass/forwardShader.h"
#include "../src/shaders/forwardPass/blinnPhongShader.h"
#include "../src/shaders/forwardPass/toonShader.h"
#include "../src/shaders/forwardPass/pbrShader.h"

void setUpScene(){

    std::string modelPath = std::string(ASSET_DIR) + "/models/teapot.fbx";
    std::string planePath = std::string(ASSET_DIR) + "/models/defaultPlane.fbx";

    std::string vShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/blinnPhong.vert";
    std::string fShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/blinnPhong.frag";
    std::string vToonShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/toonShader.vert";
    std::string fToonShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/toonShader.frag";
    std::string vPBRShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/pbrShader.vert";
    std::string fPBRShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/pbrShader.frag";

    DirectionalLight* directionalLight = ResourceManager::loadDirectionalLight(0.1f, glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)));
    PointLight* pointLight = ResourceManager::loadPointLight(0.1f, glm::vec3(3.0f, 3.0f, 3.0f), 1.0f, 0.09f, 0.032f);

    Shader* shader = new blinnPhongShader(vShaderPath.c_str(), fShaderPath.c_str());
    Shader* toonShader = new ToonShader(vToonShaderPath.c_str(), fToonShaderPath.c_str());
    Shader* pbrShader = new PBRShader(vPBRShaderPath.c_str(), fPBRShaderPath.c_str());

    ResourceManager::addShader(shader);
    ResourceManager::addShader(toonShader);
    ResourceManager::addShader(pbrShader);

    Material *material = ResourceManager::loadMaterial(glm::vec3(1.0f,1.0f,1.0f), glm::vec3(0.5f,0.5f,0.5f), glm::vec3(1.0f,1.0f,1.0f), 32.0f);
    
    Model* model = ResourceManager::loadModel(modelPath.c_str());
    Model* plane = ResourceManager::loadModel(planePath.c_str());

    GameObject* player = ResourceManager::loadGameObject();
    Camera* camera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), Camera_Projection::PERSP, 45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    ControllerModule* controllerModule = new ControllerModule(true, true, true);
    player->addModule(controllerModule);
    player->addModule(camera);
    ResourceManager::setActiveCamera(camera);

    GameObject* pot1 = ResourceManager::loadGameObject();
    RenderModule* renderModule = new RenderModule(model, material, shader);
    pot1->addModule(renderModule);
    pot1->Translate(glm::vec3(0.0f, 0.0f, -10.0f));

    GameObject* pot2 = ResourceManager::loadGameObject();
    RenderModule* renderModule1 = new RenderModule(model, material, toonShader);
    pot2->addModule(renderModule1);
    pot2->Translate(glm::vec3(3.0f, 0.0f, -10.0f));
    

    GameObject* pot3 = ResourceManager::loadGameObject();
    RenderModule* renderModule2 = new RenderModule(model, material, pbrShader);
    pot3->addModule(renderModule2);
    pot3->Translate(glm::vec3(-3.0f, 0.0f, -10.0f));

    GameObject* floor = ResourceManager::loadGameObject();
    RenderModule* renderModule3 = new RenderModule(plane, material, shader);
    floor->addModule(renderModule3);
    floor->setRotation(glm::vec3 (-90.0f, 0.0f, 0.0f));
    floor->setScale(glm::vec3(20.0f, 20.0f, 20.0f));
    floor->Translate(glm::vec3(0.0f, -1.0f, 0.0f));


    player->Translate(glm::vec3(0.0f, 0.0f, 10.0f));
    camera->lookAt(pot1->getPosition());
}

#endif // RENDERING1_H