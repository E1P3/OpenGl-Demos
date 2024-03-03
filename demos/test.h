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

GameObject* spawnManipulator(glm::vec3 position){
    GameObject* newControlPoint = ResourceManager::loadGameObject();
    RenderModule* sphereRenderModule = new RenderModule(newControlPoint, sphereModel, sphereMaterial, phongShader, true);
    newControlPoint->setPosition(position);
    newControlPoint->Scale(glm::vec3(0.1f, 0.1f, 0.1f));
    return newControlPoint;
}

void setUpScene(){
    std::string vShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhong.vert";
    std::string fShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhong.frag";

    DirectionalLight* directionalLight = ResourceManager::loadDirectionalLight(0.1f, glm::vec3(0.0f, 0.0f, 1.0f));
    PointLight* pointLight = ResourceManager::loadPointLight(0.1f, glm::vec3(0.0f, 50.0f, 300.0f), 1.0f, 0.09f, 0.032f);

    phongShader = new blinnPhongShader(vShaderPath.c_str(), fShaderPath.c_str());
    ResourceManager::addShader(phongShader);

    Model* faceModel = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/neutral.obj").c_str());
    sphereModel = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/defaultSphere.fbx").c_str());
    BasicMaterial *faceMaterial = new BasicMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.5f,0.0f), glm::vec3(0.0f,1.0f,0.5f), 1.5f);
    BasicMaterial *eyeMaterial = new BasicMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,1.0f), 5.0f);
    sphereMaterial = new BasicMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f,0.0f,0.0f), 100.0f);
    
    Camera* camera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), Camera_Projection::PERSP, 45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    ResourceManager::setActiveCamera(camera);
    camera->setPosition(glm::vec3(0.0f, 15.0f, 46.0f));
    camera->setMode(Camera_Mode::FREE);

    GameObject* face = ResourceManager::loadGameObject();
    RenderModule* renderModule = new RenderModule(face, faceModel, faceMaterial, phongShader, true);

    GameObject* sphere = ResourceManager::loadGameObject();
    RenderModule* sphereRenderModule = new RenderModule(sphereModel, sphereMaterial, phongShader);
    sphere->addModule(sphereRenderModule);
    sphere->Scale(glm::vec3(0.5f, 0.5f, 0.5f));

    GameObject* eyeBallR = ResourceManager::loadGameObject();
    RenderModule* eyeBallRRenderModule = new RenderModule(sphereModel, eyeMaterial, phongShader);
    eyeBallR->addModule(eyeBallRRenderModule);
    eyeBallR->setPosition(glm::vec3(4.3f, 17.4f, 4.8f));
    eyeBallR->Scale(glm::vec3(4.5f, 4.5f, 4.5f));

    GameObject* eyeBallL = ResourceManager::loadGameObject();
    RenderModule* eyeBallLRenderModule = new RenderModule(sphereModel, eyeMaterial, phongShader);
    eyeBallL->addModule(eyeBallLRenderModule);
    eyeBallL->setPosition(glm::vec3(-4.3f, 17.4f, 4.8f));
    eyeBallL->Scale(glm::vec3(4.5f, 4.5f, 4.5f));

    GameObject* cameraTarget = ResourceManager::loadGameObject();
    cameraTarget->setPosition(glm::vec3(0.0f, 17.0f, 0.0f));
    camera->setTpsOffset(45.0f);
    camera->setTarget(cameraTarget);
    camera->setMode(Camera_Mode::TPS);

    std::vector<GameObject*> manipulators;

    ImGuiWrapper::attachGuiFunction("Frame Rate", [](){ImGui::Text("Frame Rate: %.1f", ImGui::GetIO().Framerate);});
    ImGuiWrapper::attachGuiFunction("Face Properties", [face, faceMaterial](){face->OnGui(); faceMaterial->OnGui();});
    ImGuiWrapper::attachGuiFunction("Point Light", [pointLight](){pointLight->OnGui();});
    ImGuiWrapper::attachGuiFunction("EyeBalls", [eyeBallR, eyeBallL, eyeMaterial](){eyeBallR->OnGui(); eyeBallL->OnGui(); eyeMaterial->OnGui();});
    ImGuiWrapper::attachGuiFunction("Pointer", [sphere, renderModule](){
        keyData key = ResourceManager::getMouseData(GLFW_MOUSE_BUTTON_LEFT);
        if(key.pressDuration > 0.0f && key.pressDuration < 1.1f * ResourceManager::getDeltaTime()){
            glm::vec3 newPosition = renderModule->getClosestVertexToMouseClick(ResourceManager::getMouseRayDirection());
            if(newPosition != glm::vec3(0.0f)){
                sphere->setPosition(newPosition);
            }
        }
    });
    
}

#endif // TEST_H    