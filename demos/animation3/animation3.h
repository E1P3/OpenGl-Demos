#ifndef ANIMATION_3_H
#define ANIMATION_3_H

#include "../../src/resourceManager.h"
#include "../../src/entityModules/controllerModule.h"
#include "../../src/entityModules/renderModule.h"
#include "../../src/entityModules/gameplayModule.h"
#include "../../src/entityModules/meshManipulatorModule.h"
#include "../../src/imgui/imguiWrapper.h"
#include "../../src/shaders/forwardPass/phong/blinnPhongShader.h"
#include "../../src/materials/basicMaterial.h"
#include "../../src/utils/captureDepth.h"


Model* sphereModel;
Mesh* faceMesh;
blinnPhongShader* phongShader;
BasicMaterial* sphereMaterial;

GameObject* spawnManipulator(glm::vec3 position){
    GameObject* newControlPoint = ResourceManager::loadGameObject();
    RenderModule* sphereRenderModule = new RenderModule(newControlPoint, sphereModel, sphereMaterial, phongShader, true);
    newControlPoint->addModule(sphereRenderModule);
    newControlPoint->setPosition(position);
    newControlPoint->setName("Manipulator");
    newControlPoint->Scale(glm::vec3(0.3f, 0.3f, 0.3f));
    newControlPoint->OnStart();
    return newControlPoint;
}

glm::vec3 getDisplacementFromLine(glm::vec3 lineStart, glm::vec3 lineEnd, glm::vec3 point){
    glm::vec3 line = lineEnd - lineStart;
    glm::vec3 pointToLine = point - lineStart;
    float t = glm::dot(pointToLine, line) / glm::dot(line, line);
    return lineStart + t * line - point;
}

void setUpScene(){
    std::string vShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhong.vert";
    std::string fShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhong.frag";

    DirectionalLight* directionalLight = ResourceManager::loadDirectionalLight(0.1f, glm::vec3(0.0f, 0.0f, 1.0f));
    PointLight* pointLight = ResourceManager::loadPointLight(0.1f, glm::vec3(0.0f, 50.0f, 300.0f), 1.0f, 0.09f, 0.032f);

    phongShader = new blinnPhongShader(vShaderPath.c_str(), fShaderPath.c_str());
    ResourceManager::addShader(phongShader);

    Model* faceModel = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/neutral.obj").c_str());
    faceMesh = faceModel->getMeshes()[0];

    sphereModel = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/defaultSphere.fbx").c_str());
    BasicMaterial *faceMaterial = new BasicMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.5f,0.0f), glm::vec3(0.0f,1.0f,0.5f), 1.5f);
    BasicMaterial *eyeMaterial = new BasicMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,1.0f), 5.0f);
    sphereMaterial = new BasicMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f,0.0f,0.0f), 100.0f);
    
    Camera* camera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), Camera_Projection::PERSP, 45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    ResourceManager::setActiveCamera(camera);
    camera->setPosition(glm::vec3(0.0f, 15.0f, 46.0f));
    camera->setMode(Camera_Mode::FREE);

    GameObject* face = ResourceManager::loadGameObject();
    face->setName("Face");
    RenderModule* renderModule = new RenderModule(face, faceModel, faceMaterial, phongShader, true);
    MeshManipulatorModule* meshManipulatorModule = new MeshManipulatorModule(faceMesh);
    face->addModule(meshManipulatorModule);

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

    ImGuiWrapper::attachGuiFunction("Frame Rate", [](){
        ImGui::Text("Frame Rate: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("Mouse Position: (%.1f, %.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
        ImGui::Text("Mouse Position: (%.1f, %.1f)", ResourceManager::getMouseX(), ResourceManager::getMouseY());
    });
    ImGuiWrapper::attachGuiFunction("Face Properties", [face, faceMaterial](){face->OnGui(); faceMaterial->OnGui();});
    ImGuiWrapper::attachGuiFunction("Point Light", [pointLight](){pointLight->OnGui();});
    ImGuiWrapper::attachGuiFunction("EyeBalls", [eyeBallR, eyeBallL, eyeMaterial](){eyeBallR->OnGui(); eyeBallL->OnGui(); eyeMaterial->OnGui();});
    ImGuiWrapper::attachGuiFunction("Control Points", [meshManipulatorModule](){meshManipulatorModule->OnGui();});
    ImGuiWrapper::attachGuiFunction("Pointer", [meshManipulatorModule](){
        keyData key = ResourceManager::getMouseData(GLFW_MOUSE_BUTTON_LEFT);
        float deltaTime = ResourceManager::getDeltaTime();
        glm::vec3 position;
        GameObject* picked;
        if(key.isPressed){
            if(key.pressDuration > 0.0f && key.pressDuration < 1.1f * deltaTime)
            {
                //captureDepth();
                captureScreenshot();
                position = ResourceManager::getMouseRayHit();
                picked = ResourceManager::checkMouseVertexPick(position);
                ResourceManager::setCurrentlySelected(picked);
                if(picked != nullptr){
                    if(picked->getName() == "Face"){
                        picked = nullptr;
                        GameObject* newControlPoint = spawnManipulator(position);
                        meshManipulatorModule->addControlPoint(newControlPoint);
                        ResourceManager::setCurrentlySelected(newControlPoint);
                    }
                }
            }
            else if (key.pressDuration > 1.1f * deltaTime){
                picked = ResourceManager::getCurrentlySelected();
                if(picked != nullptr){
                    if(picked->getName() == "Manipulator"){
                        glm::vec3 position = picked->getWorldPosition();

                        glm::vec3 screenPosition = glm::project(position, ResourceManager::getActiveCamera()->getViewMatrix(), ResourceManager::getActiveCamera()->getProjectionMatrix(), glm::vec4(0, 0, ResourceManager::getScreenWidth(), ResourceManager::getScreenHeight()));
                        screenPosition.z = 0.0f;
                        glm::vec3 positionFlattened = glm::unProject(screenPosition, ResourceManager::getActiveCamera()->getViewMatrix(), ResourceManager::getActiveCamera()->getProjectionMatrix(), glm::vec4(0, 0, ResourceManager::getScreenWidth(), ResourceManager::getScreenHeight()));

                        glm::vec3 offset = positionFlattened - ResourceManager::getMouseRayOrigin();

                        picked->setPosition(position - offset*10.0f);
                    }
                }
            }
        }
        if(ResourceManager::getCurrentlySelected() != nullptr){
            ResourceManager::getCurrentlySelected()->OnGui();
        }
    });

}

#endif // ANIMATION_3_H    