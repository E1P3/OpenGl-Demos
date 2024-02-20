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

void printBones(Bone* bone, int level = 0) {
    if (!bone)
        return;

    for (int i = 0; i < level; ++i)
        std::cout << "  ";
    std::cout << bone->getName() << std::endl;

    for (auto child : bone->getChildren())
        printBones(child, level + 1);
}

void setUpScene(){

    std::string batemanPath = std::string(ASSET_DIR) + "/models/woltor.dae";
    std::string spherePath = std::string(ASSET_DIR) + "/models/defaultSphere.fbx";
    std::string vAnimShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/animated/animShader.vert";
    std::string vShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhongTex.vert";
    std::string fShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhongTex.frag";

    Camera* camera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), Camera_Projection::PERSP, 45.0f, 16.0f / 9.0f, 0.1f, 10000.0f);
    ResourceManager::setActiveCamera(camera);
    camera->setMode(Camera_Mode::FREE);

    PointLight* pointLight = ResourceManager::loadPointLight(0.1f, glm::vec3(3.0f, 3.0f, 3.0f), 1.0f, 0.09f, 0.032f);
    DirectionalLight* directionalLight = ResourceManager::loadDirectionalLight(0.1f, glm::vec3(0.0f, 0.0f, 1.0f));

    BasicMaterial *phongMaterial = new BasicMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.5f,0.5f,0.5f), glm::vec3(0.0f,0.0f,0.0f), 100.0f);
    blinnPhongShader* phongAnimShader = new blinnPhongShader(vAnimShaderPath.c_str(), fShaderPath.c_str());
    blinnPhongShader* phongShader = new blinnPhongShader(vShaderPath.c_str(), fShaderPath.c_str());
    ResourceManager::addShader(phongAnimShader);
    ResourceManager::addShader(phongShader);

    Model* bateman = ResourceManager::loadModel(batemanPath.c_str());
    Model* sphere = ResourceManager::loadModel(spherePath.c_str());

    GameObject* batemanObject = ResourceManager::loadGameObject();
    RenderModule* batemanRenderModule = new RenderModule(bateman, phongMaterial, phongAnimShader);
    batemanObject->addModule(batemanRenderModule);
    batemanObject->Scale(glm::vec3(0.1f, 0.1f, 0.1f));

    GameObject* endEffectorLeft = ResourceManager::loadGameObject();
    endEffectorLeft->setName("Left End Effector");
    RenderModule* sphereRenderModule = new RenderModule(sphere, phongMaterial, phongShader);
    endEffectorLeft->addModule(sphereRenderModule);
    endEffectorLeft->setPosition(glm::vec3(10.0f, 10.0f, 10.0f));
    endEffectorLeft->Scale(glm::vec3(0.1f, 0.1f, 0.1f));

    GameObject* endEffectorRight = ResourceManager::loadGameObject();
    endEffectorRight->setName("Right End Effector");
    RenderModule* sphereRenderModule1 = new RenderModule(sphere, phongMaterial, phongShader);
    endEffectorRight->addModule(sphereRenderModule1);
    endEffectorRight->setPosition(glm::vec3(-10.0f, 10.0f, 10.0f));
    endEffectorRight->Scale(glm::vec3(0.1f, 0.1f, 0.1f));

    camera->setTarget(batemanObject);
    camera->setMode(Camera_Mode::TPS);

    Bone* root = bateman->getRootBone();

    Bone* IKRightArm = bateman->findBone("mixamorig_RightArm", root);
    IKSolver* solverRight = new IKSolver(IKRightArm, batemanObject);

    Bone* IKLeftArm = bateman->findBone("mixamorig_LeftArm", root);
    IKSolver* solverLeft = new IKSolver(IKLeftArm, batemanObject);

    if (root) {
        ImGuiWrapper::attachGuiFunction("Skeleton", ([root](){root->OnGui();}));
        ImGuiWrapper::attachGuiFunction("Right Arm", ([endEffectorRight, solverRight](){
            endEffectorRight->OnGui();
            solverRight->OnGui();
            solverRight->solveIK(endEffectorRight);
            std::vector<glm::vec3> positions = solverRight->getCurrentPosition();
        }));
        ImGuiWrapper::attachGuiFunction("Left Arm", ([endEffectorLeft, solverLeft](){
            endEffectorLeft->OnGui();
            solverLeft->OnGui();
            solverLeft->solveIK(endEffectorLeft);
            std::vector<glm::vec3> positions = solverLeft->getCurrentPosition();
        }));
    }

}

#endif // ANIMATION2_H    