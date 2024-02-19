#ifndef TEST_H
#define TEST_H

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

    std::string IKJointNames[] = {"RightArm", "RightForeArm", "RightHand"};

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

    GameObject* sphereObject = ResourceManager::loadGameObject();
    RenderModule* sphereRenderModule = new RenderModule(sphere, phongMaterial, phongShader);
    sphereObject->addModule(sphereRenderModule);
    sphereObject->setPosition(glm::vec3(0.0f, 0.0f, 10.0f));

    GameObject* sphereObject1 = ResourceManager::loadGameObject();
    RenderModule* sphereRenderModule1 = new RenderModule(sphere, phongMaterial, phongShader);
    sphereObject1->addModule(sphereRenderModule1);
    sphereObject1->setPosition(glm::vec3(0.0f, 0.0f, 20.0f));

    GameObject* sphereObject2 = ResourceManager::loadGameObject();
    RenderModule* sphereRenderModule2 = new RenderModule(sphere, phongMaterial, phongShader);
    sphereObject2->addModule(sphereRenderModule2);
    sphereObject2->setPosition(glm::vec3(10.0f, 0.0f, 30.0f));

    GameObject* sphereObject3 = ResourceManager::loadGameObject();
    RenderModule* sphereRenderModule3 = new RenderModule(sphere, phongMaterial, phongShader);
    sphereObject3->addModule(sphereRenderModule3);
    sphereObject3->setPosition(glm::vec3(0.0f, 10.0f, 40.0f));

    sphereObject1->addChild(sphereObject2);
    sphereObject2->addChild(sphereObject3);

    camera->setTarget(batemanObject);
    camera->setMode(Camera_Mode::TPS);

    Bone* root = bateman->getRootBone();

    // std::vector<Entity*> jointEntities;

    // for(std::string jointName : IKJointNames){
    //     jointName = "mixamorig_" + jointName;
    //     Entity* jointEntity = bateman->findBone(jointName, root);
    //     if(jointEntity){
    //         jointEntities.push_back(jointEntity);
    //     }
    // }

    Bone* IKRoot = bateman->findBone("mixamorig_RightArm", root);
    IKSolver* solver = new IKSolver(IKRoot, sphereObject, batemanObject);

    if (root) {
        ImGuiWrapper::attachGuiFunction("Skeleton", ([root](){root->OnGui();}));
        ImGuiWrapper::attachGuiFunction("Goal", ([sphereObject, solver](){
            sphereObject->OnGui();
            solver->solveIK();
        }));
        ImGuiWrapper::attachGuiFunction("joint1", ([sphereObject1](){sphereObject1->OnGui();}));
        ImGuiWrapper::attachGuiFunction("joint2", ([sphereObject2](){sphereObject2->OnGui();}));
        ImGuiWrapper::attachGuiFunction("joint3", ([sphereObject3](){sphereObject3->OnGui();}));
    }

}

#endif // TEST_H    