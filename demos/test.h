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

    std::string batemanPath = std::string(ASSET_DIR) + "/models/model_skeleton.fbx";
    std::string vShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/animated/animShader.vert";
    std::string fShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhongTex.frag";

    Camera* camera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), Camera_Projection::PERSP, 45.0f, 16.0f / 9.0f, 0.1f, 10000.0f);
    ResourceManager::setActiveCamera(camera);
    camera->setMode(Camera_Mode::FREE);

    PointLight* pointLight = ResourceManager::loadPointLight(0.1f, glm::vec3(3.0f, 3.0f, 3.0f), 1.0f, 0.09f, 0.032f);
    DirectionalLight* directionalLight = ResourceManager::loadDirectionalLight(0.1f, glm::vec3(0.0f, 0.0f, 1.0f));

    BasicMaterial *phongMaterial = new BasicMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.5f,0.5f,0.5f), glm::vec3(0.0f,0.0f,0.0f), 100.0f);
    blinnPhongShader* phongShader = new blinnPhongShader(vShaderPath.c_str(), fShaderPath.c_str());
    ResourceManager::addShader(phongShader);

    Model* bateman = ResourceManager::loadModel(batemanPath.c_str());

    GameObject* batemanObject = ResourceManager::loadGameObject();
    RenderModule* batemanRenderModule = new RenderModule(bateman, phongMaterial, phongShader);
    batemanObject->addModule(batemanRenderModule);
    batemanObject->Scale(glm::vec3(0.1f, 0.1f, 0.1f));

    camera->setTarget(batemanObject);
    camera->setMode(Camera_Mode::TPS);

    Bone* root = bateman->getRootBone();

    if (root) {
        ImGuiWrapper::attachGuiFunction("Skeleton", ([root](){root->OnGui();}));
    }

}

#endif // TEST_H    