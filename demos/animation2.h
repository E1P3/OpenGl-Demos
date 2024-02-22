#ifndef ANIMATION2_H
#define ANIMATION2_H

#include "../src/resourceManager.h"
#include "../src/shaders/forwardPass/phong/blinnPhongShader.h"
#include "../src/materials/basicMaterial.h"
#include "../src/bone.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../src/utils/assimpHelper.h"
#include "../src/imgui/imguiWrapper.h"
#include "../src/IKSolver.h"
#include "../src/animator.h"

Model* sphere;
blinnPhongShader* phongShader;

void printBones(Bone* bone, int level = 0) {
    if (!bone)
        return;

    for (int i = 0; i < level; ++i)
        std::cout << "  ";
    std::cout << bone->getName() << std::endl;

    for (auto child : bone->getChildren())
        printBones(child, level + 1);
}

GameObject* SpawnSphere(std::string name, glm::vec3 position, glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f)){
    GameObject* newControlPoint = ResourceManager::loadGameObject();
    BasicMaterial* sphereMaterial = new BasicMaterial(glm::vec3(0.0f,0.0f,0.0f), color, glm::vec3(0.0f,0.0f,0.0f), 100.0f);
    RenderModule* sphereRenderModule = new RenderModule(sphere, sphereMaterial, phongShader);
    newControlPoint->addModule(sphereRenderModule);
    newControlPoint->setPosition(position);
    newControlPoint->Scale(glm::vec3(0.1f, 0.1f, 0.1f));
    newControlPoint->setName(name);
    return newControlPoint;
}

void spawnSpheresOnSkeleton(Bone* bone, std::vector<GameObject*>& spheres) {
    spheres.push_back(SpawnSphere(bone->getName(), bone->getWorldPosition()));
    for (auto child : bone->getChildren())
        spawnSpheresOnSkeleton(child, spheres);
}

void updateSkeletonSpheres(Bone* bone, std::vector<GameObject*> spheres) {
    for(GameObject* sphere : spheres){
        if(sphere->getName() == bone->getName()){
            sphere->setPosition(bone->getWorldPosition());
        }
    }
    for (auto child : bone->getChildren())
        updateSkeletonSpheres(child, spheres);
}

void setUpScene(){

    std::string characterPath = std::string(ASSET_DIR) + "/models/hazmat";
    std::string spherePath = std::string(ASSET_DIR) + "/models/defaultSphere.fbx";
    std::string vAnimShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/animated/animShader.vert";
    std::string vShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhongTex.vert";
    std::string fShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhongTex.frag";

    Camera* camera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), Camera_Projection::PERSP, 45.0f, 16.0f / 9.0f, 0.1f, 10000.0f, 30.0f);
    ResourceManager::setActiveCamera(camera);

    GameObject* cameraTarget = ResourceManager::loadGameObject();
    cameraTarget->setPosition(glm::vec3(0.0f, 10.0f, 0.0f));
    camera->setTarget(cameraTarget);
    camera->setMode(Camera_Mode::TPS);

    PointLight* pointLight = ResourceManager::loadPointLight(0.1f, glm::vec3(25.0f, 70.0f, 70.0f), 1.0f, 0.09f, 0.032f);
    DirectionalLight* directionalLight = ResourceManager::loadDirectionalLight(0.1f, glm::vec3(0.0f, 0.0f, 1.0f));

    BasicMaterial* phongMaterial = new BasicMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.5f,0.5f,0.5f), glm::vec3(0.0f,0.0f,0.0f), 100.0f);
    blinnPhongShader* phongAnimShader = new blinnPhongShader(vAnimShaderPath.c_str(), fShaderPath.c_str());
    phongShader = new blinnPhongShader(vShaderPath.c_str(), fShaderPath.c_str());
    ResourceManager::addShader(phongAnimShader);
    ResourceManager::addShader(phongShader);

    std::string characterFbxPath = characterPath + ".fbx";
    std::string characterDaePath = characterPath + ".dae";

    Model* character = ResourceManager::loadModel(characterFbxPath.c_str());
    Model* characterSkeleton = ResourceManager::loadModel(characterDaePath.c_str());

    character->setRootBone(characterSkeleton->getRootBone()); // 

    sphere = ResourceManager::loadModel(spherePath.c_str());

    GameObject* characterObject = ResourceManager::loadGameObject();
    RenderModule* characterRenderModule = new RenderModule(character, phongMaterial, phongAnimShader);
    characterObject->addModule(characterRenderModule);
    characterObject->Scale(glm::vec3(0.1f, 0.1f, 0.1f));

    GameObject* endEffectorLeft = SpawnSphere("Left End Effector", glm::vec3(3.0f, 7.0f, 1.0f));
    GameObject* endEffectorRight = SpawnSphere("Right End Effector", glm::vec3(-3.0f, 7.0f, 1.0f));

    Bone* root = character->getRootBone();

    Bone* IKRightArm = character->findBone("mixamorig_RightArm", root);
    IKSolver* solverRight = new IKSolver(IKRightArm, characterObject);

    Bone* IKLeftArm = character->findBone("mixamorig_LeftArm", root);
    IKSolver* solverLeft = new IKSolver(IKLeftArm, characterObject);

    solverLeft->setAxis(glm::vec3(1.0f, 1.0f, 0.0f));

    Animator* animator = new Animator(endEffectorRight, 1.0f);

    GameObject* controlPoint0 = SpawnSphere("Control Point 0", glm::vec3(-5.0f, 10.0f, -5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    GameObject* controlPoint1 = SpawnSphere("Control Point 1", glm::vec3(-5.0f, 10.0f, 5.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    animator->addControlPoint(controlPoint0);
    animator->addControlPoint(controlPoint1);

    bool* is2D = new bool(false);

    if (root) {
        ImGuiWrapper::attachGuiFunction("Skeleton", ([root](){root->OnGui();}));
        ImGuiWrapper::attachGuiFunction("Right Arm", ([endEffectorRight, solverRight](){
            endEffectorRight->OnGui();
            solverRight->OnGui();
            solverRight->solveIK(endEffectorRight);
            std::vector<glm::vec3> positions = solverRight->getCurrentPosition();
        }));
        ImGuiWrapper::attachGuiFunction("Left Arm", ([endEffectorLeft, solverLeft, is2D](){
            endEffectorLeft->OnGui();
            solverLeft->OnGui();
            solverLeft->solveIK(endEffectorLeft);
            std::vector<glm::vec3> positions = solverLeft->getCurrentPosition();
            ImGui::SameLine();
            ImGui::Checkbox("2D", is2D);
            if(*is2D){
                solverLeft->setAxis(glm::vec3(1.0f, 1.0f, 0.0f));
            } else {
                solverLeft->setAxis(glm::vec3(1.0f, 1.0f, 1.0f));
            }
        }));
        ImGuiWrapper::attachGuiFunction("Animator", ([animator](){
            animator->OnGui();
            animator->update();
            (ResourceManager::getDeltaTime());
            if(ImGui::Button("Add Control Point")){
                GameObject* newControlPoint = SpawnSphere("Control Point",animator->getLastControlPoint()->getWorldPosition(), glm::vec3(1.0f, 0.0f, 0.0f));
                newControlPoint->OnStart();
                newControlPoint->Translate(glm::vec3(0.0f, 0.1f, 0.0f));
                animator->addControlPoint(newControlPoint);
            }
        }));
        ImGuiWrapper::attachGuiFunction("Point Light", ([pointLight](){pointLight->OnGui();}));
    }

}

#endif // ANIMATION2_H    