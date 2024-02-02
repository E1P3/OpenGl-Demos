#ifndef ANIMATION1_H
#define ANIMATION1_H

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
#include "../src/shaders/skybox/cubeMap.h"
#include "../src/shaders/skybox/skyboxShader.h"

void OnGui(){
    ImGui::Begin("Animation1");
    ImGui::Text("This is a test");
    ImGui::End();
}

void setUpScene(ImGuiWrapper* imguiWrapper){

    GameObject* player = ResourceManager::loadGameObject();
    Camera* camera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), Camera_Projection::PERSP, 45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    ControllerModule* controllerModule = new ControllerModule(true, true, true);
    player->addModule(controllerModule);
    player->addModule(camera);
    ResourceManager::setActiveCamera(camera);

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

    imguiWrapper->attachGuiFunction("Animation1", [](){OnGui();});
    std::string vShaderPath = std::string(SRC_DIR) + "/shaders/skybox/skyboxShader.vert";
    std::string fShaderPath = std::string(SRC_DIR) + "/shaders/skybox/skyboxShader.frag";

    SkyboxShader* skyboxShader = new SkyboxShader(vShaderPath.c_str(), fShaderPath.c_str(), skyboxes);
    ResourceManager::addShader(skyboxShader);

    GameObject* skyboxObject = ResourceManager::loadGameObject();
    RenderModule* skyboxRenderModule = new RenderModule(nullptr, nullptr, skyboxShader);
    skyboxObject->addModule(skyboxRenderModule);

}

#endif // ANIMATION1_H