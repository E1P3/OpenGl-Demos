#ifndef TEST_H
#define TEST_H

#include "../src/resourceManager.h"
#include "../src/entityModules/controllerModule.h"
#include "../src/entityModules/renderModule.h"
#include "../src/entityModules/gameplayModule.h"
#include "../src/imgui/imguiWrapper.h"
#include "../src/shaders/forwardPass/phong/blinnPhongShader.h"
#include "../src/materials/basicMaterial.h"

void setUpScene(){
    std::string vShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhong.vert";
    std::string fShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhong.frag";

    DirectionalLight* directionalLight = ResourceManager::loadDirectionalLight(0.1f, glm::vec3(0.0f, 0.0f, 1.0f));
    PointLight* pointLight = ResourceManager::loadPointLight(0.1f, glm::vec3(-3.0f, 3.0f, -3.0f), 1.0f, 0.09f, 0.032f);

    blinnPhongShader* phongShader = new blinnPhongShader(vShaderPath.c_str(), fShaderPath.c_str());
    ResourceManager::addShader(phongShader);

    Model* surfaceModel = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/terrain.fbx").c_str());
    Model* sphereModel = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/defaultSphere.fbx").c_str());
    BasicMaterial *material = new BasicMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.5f,0.5f,0.5f), glm::vec3(1.0f,1.0f,1.0f), 32.0f);

    Camera* camera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), Camera_Projection::PERSP, 45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    ResourceManager::setActiveCamera(camera);
    camera->setMode(Camera_Mode::FREE);

    GameObject* dragon = ResourceManager::loadGameObject();
    RenderModule* renderModule = new RenderModule(sphereModel, material, phongShader);
    dragon->addModule(renderModule);

    GameObject* sphere = ResourceManager::loadGameObject();
    BasicMaterial* sphereMaterial = new BasicMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f,0.0f,0.0f), 100.0f);
    RenderModule* sphereRenderModule = new RenderModule(sphereModel, sphereMaterial, phongShader);
    sphere->addModule(sphereRenderModule);
    sphere->Scale(glm::vec3(0.1f, 0.1f, 0.1f));

    ImGuiWrapper::attachGuiFunction("Frame Rate", [](){ImGui::Text("Frame Rate: %.1f", ImGui::GetIO().Framerate);});
    ImGuiWrapper::attachGuiFunction("Pointer", [sphere, renderModule](){
        keyData key = ResourceManager::getMouseData(GLFW_MOUSE_BUTTON_LEFT);
        if(key.pressDuration > 0.0f && key.pressDuration < 1.1f * ResourceManager::getDeltaTime()){
            glm::vec3 newPosition = renderModule->getClosestVertexToMouseClick();
            if(newPosition != glm::vec3(0.0f)){
                sphere->setPosition(newPosition);
            }
        }
    });
    
}

#endif // TEST_H    