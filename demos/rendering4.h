#ifndef RENDERING4_H
#define RENDERING4_H

// Scene to showcase different shading techniques

#include "../src/resourceManager.h"
#include "../src/entityModules/controllerModule.h"
#include "../src/entityModules/renderModule.h"
#include "../src/entityModules/gameplayModule.h"
#include "../src/materials/basicMaterial.h"
#include "../src/shaders/forwardPass/phong/blinnPhongTexShader.h"
#include "../src/imgui/imguiWrapper.h"

void setUpScene(){

    std::string potPath = std::string(ASSET_DIR) + "/models/teapot.fbx";
    std::string spherePath = std::string(ASSET_DIR) + "/models/defaultSphere.fbx";
    std::string dragonPath = std::string(ASSET_DIR) + "/models/dragon.fbx";
    std::string planePath = std::string(ASSET_DIR) + "/models/defaultPlane.fbx";

    std::string vShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhongTex.vert";
    std::string fShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/phong/blinnPhongTex.frag";

    std::string texturePath = std::string(ASSET_DIR) + "/textures/seamless_default.jpg";

    Texture* textureMML = new Texture(DIFFUSE, texturePath.c_str(), true, GL_LINEAR);
    Texture* textureMMN = new Texture(DIFFUSE, texturePath.c_str(), true, GL_NEAREST);
    Texture* textureL = new Texture(DIFFUSE, texturePath.c_str(), false, GL_LINEAR);
    Texture* textureN = new Texture(DIFFUSE, texturePath.c_str(), false, GL_NEAREST);

    std::vector<Texture*> textureList = {textureMML, textureMMN, textureL, textureN};
    std::vector<std::string> textureNames = {"Linear MipMap", "Nearest MipMap", "Linear", "Nearest"};

    DirectionalLight* directionalLight = ResourceManager::loadDirectionalLight(0.1f, glm::vec3(0.0f, 0.0f, 1.0f));
    PointLight* pointLight = ResourceManager::loadPointLight(0.1f, glm::vec3(3.0f, 3.0f, 3.0f), 1.0f, 0.09f, 0.032f);

    blinnPhongTexShader* objectShader = new blinnPhongTexShader(vShaderPath.c_str(), fShaderPath.c_str(), false);
    blinnPhongTexShader* objectFarShader = new blinnPhongTexShader(vShaderPath.c_str(), fShaderPath.c_str(), false);
    blinnPhongTexShader* texturedShader = new blinnPhongTexShader(vShaderPath.c_str(), fShaderPath.c_str(), false);
    ResourceManager::addShader(objectShader);
    ResourceManager::addShader(objectFarShader);
    ResourceManager::addShader(texturedShader);
    texturedShader->addTexture(textureMML);
    objectShader->addTexture(textureMML);
    objectFarShader->addTexture(textureMML);

    BasicMaterial *phongMaterial = new BasicMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.5f,0.5f,0.5f), glm::vec3(0.01f,0.01f,0.01f), 32.0f);

    Model* sphere = ResourceManager::loadModel(spherePath.c_str());
    Model* pot = ResourceManager::loadModel(potPath.c_str());
    Model* dragon = ResourceManager::loadModel(dragonPath.c_str());
    Model* plane = ResourceManager::loadModel(planePath.c_str());

    Camera* camera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), Camera_Projection::PERSP, 45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    ResourceManager::setActiveCamera(camera);
    camera->setMode(Camera_Mode::FREE);

    GameObject* surface = ResourceManager::loadGameObject();
    RenderModule* renderModule = new RenderModule(plane, phongMaterial, texturedShader);
    surface->addModule(renderModule);
    surface->Translate(glm::vec3(0.0f, -15.0f, 0.0f));
    surface->Rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
    surface->Scale(glm::vec3(1000.0f, 1000.0f, 1.0f));

    GameObject* potObject = ResourceManager::loadGameObject();
    RenderModule* potRenderModule = new RenderModule(pot, phongMaterial, objectShader);
    GameplayModule* potGameplayModule = new GameplayModule();
    potObject->addModule(potRenderModule);
    potObject->addModule(potGameplayModule);
    potObject->Translate(glm::vec3(0.0f, 0.0f, -10.0f));

    GameObject* sphereObject = ResourceManager::loadGameObject();
    RenderModule* sphereRenderModule = new RenderModule(sphere, phongMaterial, objectShader);
    GameplayModule* sphereGameplayModule = new GameplayModule();
    sphereObject->addModule(sphereRenderModule);
    sphereObject->addModule(sphereGameplayModule);
    sphereObject->Translate(glm::vec3(3.0f, 0.7f, -10.0f));

    GameObject* potFarObject = ResourceManager::loadGameObject();
    RenderModule* potFarRenderModule = new RenderModule(pot, phongMaterial, objectFarShader);
    GameplayModule* potFarGameplayModule = new GameplayModule();
    potFarObject->addModule(potFarRenderModule);
    potFarObject->addModule(potFarGameplayModule);
    potFarObject->Translate(glm::vec3(-160.0f, 35.0f, -500.0f));
    potFarObject->Scale(glm::vec3(100.0f, 100.0f, 100.0f));

    GameObject* sphereFarObject = ResourceManager::loadGameObject();
    RenderModule* sphereFarRenderModule = new RenderModule(sphere, phongMaterial, objectFarShader);
    GameplayModule* sphereFarGameplayModule = new GameplayModule();
    sphereFarObject->addModule(sphereFarRenderModule);
    sphereFarObject->addModule(sphereFarGameplayModule);
    sphereFarObject->Translate(glm::vec3(120.0f, 100.0f, -500.0f));
    sphereFarObject->Scale(glm::vec3(100.0f, 100.0f, 100.0f));

    float* textureScale =  new float(1000.0f);
    int* textureIndex = new int(0);

    camera->lookAt(potObject->getPosition(), glm::vec3(0.0f, 1.0f, 0.0f));


    ImGuiWrapper::attachGuiFunction("Frame Rate", [](){
        ImGui::Text("Frame Rate: %.1f", ImGui::GetIO().Framerate);
    });

    ImGuiWrapper::attachGuiFunction("Texture Properties", [textureScale, texturedShader, objectShader, objectFarShader, textureList, textureIndex, textureNames](){
        ImGui::SliderFloat("Texture Scale", textureScale, 0.0f, 1000.0f);
        texturedShader->setTextureScale(*textureScale);
        objectFarShader->setTextureScale(*textureScale);
        if(ImGui::Button("Next Texture"))
        {
            *textureIndex = (*textureIndex - 1 + textureList.size()) % textureList.size();
            texturedShader->swapTextureArray({textureList[*textureIndex]});
            objectShader->swapTextureArray({textureList[*textureIndex]});
            objectFarShader->swapTextureArray({textureList[*textureIndex]});
        }
        ImGui::SameLine();
        ImGui::Text("Current Texture: %s", textureNames[*textureIndex].c_str());
    });
}

#endif // RENDERING4_H