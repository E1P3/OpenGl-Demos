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
#include "../../src/entityModules/faceManipulation.h"

bool loadFaces = true;

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
    Model* jaw_open_model = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_jaw_open.obj").c_str());
    Model* kiss_model = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_kiss.obj").c_str());
    Model* l_brow_lower_model = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_l_brow_lower.obj").c_str());
    Model* l_brow_narrow_model = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_l_brow_narrow.obj").c_str());
    Model* l_brow_raise_model = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_l_brow_raise.obj").c_str());
    Model* l_eye_closed_model = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_l_eye_closed.obj").c_str());
    Model* l_eye_lower_open = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_l_eye_lower_open.obj").c_str());
    Model* l_eye_upper_open = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_l_eye_upper_open.obj").c_str());
    Model* l_nose_wrinkle_model = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_l_nose_wrinkle.obj").c_str());
    Model* l_puff_model = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_l_puff.obj").c_str());
    Model* l_sad_model = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_l_sad.obj").c_str());
    Model* l_smile_model = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_l_smile.obj").c_str());
    Model* l_suck_model = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_l_suck.obj").c_str());
    Model* r_brow_lower_model = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_r_brow_lower.obj").c_str());
    Model* r_brow_narrow_model = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_r_brow_narrow.obj").c_str());
    Model* r_brow_raise_model = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_r_brow_raise.obj").c_str());
    Model* r_eye_closed_model = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_r_eye_closed.obj").c_str());
    Model* r_eye_lower_open = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_r_eye_lower_open.obj").c_str());
    Model* r_eye_upper_open = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_r_eye_upper_open.obj").c_str());
    Model* r_nose_wrinkle_model = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_r_nose_wrinkle.obj").c_str());
    Model* r_puff_model = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_r_puff.obj").c_str());
    Model* r_sad_model = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_r_sad.obj").c_str());
    Model* r_smile_model = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_r_smile.obj").c_str());
    Model* r_suck_model = ResourceManager::loadModel((std::string(ASSET_DIR) + "/models/faces/Mery_r_suck.obj").c_str());

    faceMesh = faceModel->getMeshes()[0];
    Mesh* jaw_open_mesh = jaw_open_model->getMeshes()[0];
    Mesh* kiss_mesh = kiss_model->getMeshes()[0];
    Mesh* l_brow_lower_mesh = l_brow_lower_model->getMeshes()[0];
    Mesh* l_brow_narrow_mesh = l_brow_narrow_model->getMeshes()[0];
    Mesh* l_brow_raise_mesh = l_brow_raise_model->getMeshes()[0];
    Mesh* l_eye_closed_mesh = l_eye_closed_model->getMeshes()[0];
    Mesh* l_eye_lower_open_mesh = l_eye_lower_open->getMeshes()[0];
    Mesh* l_eye_upper_open_mesh = l_eye_upper_open->getMeshes()[0];
    Mesh* l_nose_wrinkle_mesh = l_nose_wrinkle_model->getMeshes()[0];
    Mesh* l_puff_mesh = l_puff_model->getMeshes()[0];
    Mesh* l_sad_mesh = l_sad_model->getMeshes()[0];
    Mesh* l_smile_mesh = l_smile_model->getMeshes()[0];
    Mesh* l_suck_mesh = l_suck_model->getMeshes()[0];
    Mesh* r_brow_lower_mesh = r_brow_lower_model->getMeshes()[0];
    Mesh* r_brow_narrow_mesh = r_brow_narrow_model->getMeshes()[0];
    Mesh* r_brow_raise_mesh = r_brow_raise_model->getMeshes()[0];
    Mesh* r_eye_closed_mesh = r_eye_closed_model->getMeshes()[0];
    Mesh* r_eye_lower_open_mesh = r_eye_lower_open->getMeshes()[0];
    Mesh* r_eye_upper_open_mesh = r_eye_upper_open->getMeshes()[0];
    Mesh* r_nose_wrinkle_mesh = r_nose_wrinkle_model->getMeshes()[0];
    Mesh* r_puff_mesh = r_puff_model->getMeshes()[0];
    Mesh* r_sad_mesh = r_sad_model->getMeshes()[0];
    Mesh* r_smile_mesh = r_smile_model->getMeshes()[0];
    Mesh* r_suck_mesh = r_suck_model->getMeshes()[0];

    FaceManipulation* faceManipulation = new FaceManipulation(faceMesh);
    faceManipulation->addBlendShape(jaw_open_mesh, "jaw_open");
    faceManipulation->addBlendShape(kiss_mesh, "kiss");
    //faceManipulation->addBlendShape(l_brow_lower_mesh, "l_brow_lower");
    //faceManipulation->addBlendShape(l_brow_narrow_mesh, "l_brow_narrow");
    //faceManipulation->addBlendShape(l_brow_raise_mesh, "l_brow_raise");
    faceManipulation->addBlendShape(l_eye_closed_mesh, "l_eye_closed");
    //faceManipulation->addBlendShape(l_eye_lower_open_mesh, "l_eye_lower_open");
    //faceManipulation->addBlendShape(l_eye_upper_open_mesh, "l_eye_upper_open");
    //faceManipulation->addBlendShape(l_nose_wrinkle_mesh, "l_nose_wrinkle");
    //faceManipulation->addBlendShape(l_puff_mesh, "l_puff");
    //faceManipulation->addBlendShape(l_sad_mesh, "l_sad");
    faceManipulation->addBlendShape(l_smile_mesh, "l_smile");
    //faceManipulation->addBlendShape(l_suck_mesh, "l_suck");
    //faceManipulation->addBlendShape(r_brow_lower_mesh, "r_brow_lower");
    //faceManipulation->addBlendShape(r_brow_narrow_mesh, "r_brow_narrow");
    //faceManipulation->addBlendShape(r_brow_raise_mesh, "r_brow_raise");
    faceManipulation->addBlendShape(r_eye_closed_mesh, "r_eye_closed");
    //faceManipulation->addBlendShape(r_eye_lower_open_mesh, "r_eye_lower_open");
    //faceManipulation->addBlendShape(r_eye_upper_open_mesh, "r_eye_upper_open");
    //faceManipulation->addBlendShape(r_nose_wrinkle_mesh, "r_nose_wrinkle");
    //faceManipulation->addBlendShape(r_puff_mesh, "r_puff");
    //faceManipulation->addBlendShape(r_sad_mesh, "r_sad");
    faceManipulation->addBlendShape(r_smile_mesh, "r_smile");
    //faceManipulation->addBlendShape(r_suck_mesh, "r_suck");


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
    //face->addModule(meshManipulatorModule);
    face->addModule(faceManipulation);

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

    ImGuiWrapper::attachGuiFunction("Blend Shapes", [faceManipulation](){faceManipulation->OnGui();});
    ImGuiWrapper::attachGuiFunction("Frame Rate", [](){
        ImGui::Text("Frame Rate: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("Window size: (%.1f, %.1f)", ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
        ImGui::Text("Window size: (%.1d, %.1d)", ResourceManager::getScreenWidth(), ResourceManager::getScreenHeight());
        ImGui::Text("Mouse Position: (%.1f, %.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
        ImGui::Text("Mouse Position: (%.1f, %.1f)", ResourceManager::getMouseX(), ResourceManager::getMouseY());
    });
    ImGuiWrapper::attachGuiFunction("Point Light", [pointLight](){pointLight->OnGui();});
    ImGuiWrapper::attachGuiFunction("EyeBalls", [eyeBallR, eyeBallL, eyeMaterial](){eyeBallR->OnGui(); eyeBallL->OnGui(); eyeMaterial->OnGui();});
    ImGuiWrapper::attachGuiFunction("Control Points", [meshManipulatorModule](){meshManipulatorModule->OnGui();});
    ImGuiWrapper::attachGuiFunction("Pointer", [faceManipulation](){
        keyData key = ResourceManager::getMouseData(GLFW_MOUSE_BUTTON_LEFT);
        float deltaTime = ResourceManager::getDeltaTime();
        glm::vec3 position;
        GameObject* picked;
        if(key.isPressed){
            if(key.pressDuration > 0.0f && key.pressDuration < 1.1f * deltaTime)
            {
                //captureDepth();
                //captureScreenshot();
                position = ResourceManager::getMouseRayHit();
                int vertexIndex = -1;
                picked = ResourceManager::checkMouseVertexPick(position, vertexIndex);
                ResourceManager::setCurrentlySelected(picked);
                if(picked != nullptr){
                    if(picked->getName() == "Face"){
                        picked = nullptr;
                        GameObject* newControlPoint = spawnManipulator(position);
                        faceManipulation->addManipulator(newControlPoint, vertexIndex);
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

                        picked->setPosition(position - offset*1.0f);
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