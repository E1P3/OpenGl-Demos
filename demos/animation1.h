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

GameObject* plane_body;
GameObject* pilot;
DirectionalLight* directionalLight;
PointLight* pointLight;
BasicMaterial* surfaceMaterial;
glm::vec3 planeRotationEuler = glm::vec3(0.0f, 0.0f, 0.0f);
glm::quat planeRotationQuat = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
bool rotationSwitch = true; //euler enabled

void OnGui(){
    ImGui::Text("Controls:\nF - Switch camera (First Person, Third Person, Free)\nQ/E = Roll\nW/S = Pitch\nA/D = Yaw\n\n");

    ImGui::Text("Camera Properties\n");
    glm::vec3 cameraRotation = ResourceManager::getActiveCamera()->getRotationEuler();
    glm::vec3 cameraPosition = ResourceManager::getActiveCamera()->getPosition();
    glm::quat cameraRotationQuat = glm::quat(cameraRotation);
    glm::vec3 cameraForward = ResourceManager::getActiveCamera()->getFront();
    glm::vec3 cameraUp = ResourceManager::getActiveCamera()->getUp();
    glm::vec3 cameraRight = ResourceManager::getActiveCamera()->getRight();
    ImGui::Text("Camera Rotation: (%f, %f, %f)", cameraRotation.x, cameraRotation.y, cameraRotation.z);
    ImGui::Text("Camera Rotation Quat: (%f, %f, %f, %f)", cameraRotationQuat.x, cameraRotationQuat.y, cameraRotationQuat.z, cameraRotationQuat.w);
    ImGui::Text("Camera Position: (%f, %f, %f)", cameraPosition.x, cameraPosition.y, cameraPosition.z);
    ImGui::Text("Camera Forward: (%f, %f, %f)", cameraForward.x, cameraForward.y, cameraForward.z);
    ImGui::Text("Camera Up: (%f, %f, %f)", cameraUp.x, cameraUp.y, cameraUp.z);
    ImGui::Text("Camera Right: (%f, %f, %f)", cameraRight.x, cameraRight.y, cameraRight.z);

    ImGui::Text("\nPlane Properties\n");
    glm::quat planeRotationQuat = plane_body->getRotation();
    glm::vec3 planeRotation = glm::degrees(glm::eulerAngles(planeRotationQuat));
    ImGui::Text("Plane Rotation: (%f, %f, %f)", planeRotation.x, planeRotation.y, planeRotation.z);
    ImGui::Text("Plane Rotation Quat: (%f, %f, %f, %f)", planeRotationQuat.x, planeRotationQuat.y, planeRotationQuat.z, planeRotationQuat.w);

    if(ImGui::Button("Switch Rotation Type")){
        rotationSwitch = !rotationSwitch;
    }
    if(rotationSwitch){
        planeRotationEuler = plane_body->getRotationEuler();
        ImGui::DragFloat3("Plane Rotation", &planeRotationEuler[0], 1.0f, -90.0f, 90.0f);
        plane_body->setRotation(planeRotationEuler);
    } else {
        planeRotationQuat = plane_body->getRotation();
        ImGui::DragFloat4("Plane Rotation Quaternion", &planeRotationQuat[0], 0.01f, -1.0f, 1.0f);
        plane_body->setRotation(planeRotationQuat);
    }


    std::string cameraMode = "";
    switch(ResourceManager::getActiveCamera()->getMode()){
        case Camera_Mode::FREE:
            cameraMode = "Free";
            break;
        case Camera_Mode::FPS:
            cameraMode = "First Person";
            break;
        case Camera_Mode::TPS:
            cameraMode = "Third Person";
            break;
    }

    ImGui::Text("\nCamera Mode: %s", cameraMode.c_str());

}

void setUpScene(){

    std::string planeBodyPath = std::string(ASSET_DIR) + "/models/plane/plane_body.obj";
    std::string planePropellerPath = std::string(ASSET_DIR) + "/models/plane/plane_propeller.obj";
    std::string surfacePath = std::string(ASSET_DIR) + "/models/defaultPlane.fbx";


    std::string vSkyShaderPath = std::string(SRC_DIR) + "/shaders/skybox/skyboxShader.vert";
    std::string fSkyShaderPath = std::string(SRC_DIR) + "/shaders/skybox/skyboxShader.frag";
    std::string vPhongShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhongTex.vert";
    std::string fPhongShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhongTex.frag";

    std::string cubePath = std::string(ASSET_DIR) + "/textures/skybox/bluecloud_";
    Cubemap* skybox = new Cubemap(cubePath, ".jpg");
    
    directionalLight = ResourceManager::loadDirectionalLight(0.1f, glm::degrees(glm::vec3(90.0f, 0.0f, 0.0f)));
    pointLight = ResourceManager::loadPointLight(0.1f, glm::vec3(3.0f, 3.0f, 3.0f), 1.0f, 0.09f, 0.032f);

    Model* planeBodyModel = ResourceManager::loadModel(planeBodyPath.c_str());
    Model* planePropellerModel = ResourceManager::loadModel(planePropellerPath.c_str());
    Model* surfaceModel = ResourceManager::loadModel(surfacePath.c_str());

    blinnPhongShader* phongShader = new blinnPhongShader(vPhongShaderPath.c_str(), fPhongShaderPath.c_str());
    SkyboxShader* skyboxShader = new SkyboxShader(vSkyShaderPath.c_str(), fSkyShaderPath.c_str(), skybox);

    phongShader->setDebug(false);

    BasicMaterial* basicMaterial = new BasicMaterial(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f) , 32.0f);
    surfaceMaterial = new BasicMaterial(glm::vec3(0.0f, 0.3f, 0.4f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.55f, 0.55f, 0.55f) , 0.76f);

    ResourceManager::addShader(skyboxShader);
    ResourceManager::addShader(phongShader);

    Camera* camera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), Camera_Projection::PERSP, 45.0f, 16.0f / 9.0f, 0.1f, 10000.0f);
    ResourceManager::setActiveCamera(camera);

    GameObject* skyboxObject = ResourceManager::loadGameObject();
    RenderModule* skyboxRenderModule = new RenderModule(nullptr, nullptr, skyboxShader);
    skyboxObject->addModule(skyboxRenderModule);

    GameObject* surface = ResourceManager::loadGameObject();
    RenderModule* surfaceRenderModule = new RenderModule(surfaceModel, surfaceMaterial, phongShader);
    surface->addModule(surfaceRenderModule);
    surface->setPosition(glm::vec3(0.0f, -100.0f, 0.0f));
    surface->setScale(glm::vec3(10000.0f, 10000.0f, 10000.0f));
    surface->setRotation(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

    plane_body = ResourceManager::loadGameObject();
    RenderModule* gameModelRenderModule = new RenderModule(planeBodyModel, basicMaterial, phongShader);
    GameplayModule* gameplayModule1 = new GameplayModule();
    gameplayModule1->setControlled(true);
    plane_body->addModule(gameplayModule1);
    plane_body->addModule(gameModelRenderModule);
    plane_body->setPosition(glm::vec3(0.0f, 0.0f, -5.0f));
    plane_body->setScale(glm::vec3(0.1f, 0.1f, 0.1f));

    GameObject* plane_propeller = ResourceManager::loadGameObject();
    RenderModule* propellerRenderModule = new RenderModule(planePropellerModel, basicMaterial, phongShader);
    GameplayModule* gameplayModule = new GameplayModule(glm::vec3(1.0f, 0.0f, 0.0f), 100.0f);
    plane_propeller->addModule(propellerRenderModule);
    plane_propeller->addModule(gameplayModule);

    pilot = ResourceManager::loadGameObject();
    pilot->setPosition(glm::vec3(-5.9, 2.6, 2.7));

    plane_body->addChild(plane_propeller);
    plane_body->addChild(pilot);

    camera->setTarget(pilot);
    camera->setMode(Camera_Mode::TPS);

    ImGuiWrapper::attachGuiFunction("Camera Properties", OnGui);

}

#endif // ANIMATION1_H