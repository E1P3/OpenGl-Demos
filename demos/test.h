#ifndef TEST_H
#define TEST_H

#include "../src/resourceManager.h"
#include "../src/entityModules/controllerModule.h"
#include "../src/entityModules/renderModule.h"
#include "../src/entityModules/gameplayModule.h"
#include "../src/materials/tessMaterial.h"
#include "../src/materials/texturedMaterial.h"
#include "../src/shaders/forwardPass/textured/texturedShader.h"
#include "../src/shaders/forwardPass/tessalation/tessShader.h"
#include "../src/imgui/imguiWrapper.h"

void setUpScene(){
    std::string texturePath = std::string(ASSET_DIR) + "/textures/ireland-heightmap.jpg";
    std::string planePath = std::string(ASSET_DIR) + "/models/terrain.fbx";

    std::string vShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/tessalation/tessShader.vert";
    std::string tcsShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/tessalation/tessShaderView.tesc";
    std::string tesShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/tessalation/tessShader.tese";
    std::string gShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/tessalation/tessShader.geom";
    std::string fShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/tessalation/tessShader.frag";

    DirectionalLight* directionalLight = ResourceManager::loadDirectionalLight(0.1f, glm::vec3(0.0f, 0.0f, 1.0f));
    PointLight* pointLight = ResourceManager::loadPointLight(0.1f, glm::vec3(-3.0f, 3.0f, -3.0f), 1.0f, 0.09f, 0.032f);

    TessalationShader* tessShader = new TessalationShader(vShaderPath.c_str(), fShaderPath.c_str(), gShaderPath.c_str(), tcsShaderPath.c_str(), tesShaderPath.c_str());
    ResourceManager::addShader(tessShader);
    Texture* textureHeight = new Texture(HEIGHT, texturePath.c_str(), true, GL_LINEAR);
    Texture* textureDiffuse = new Texture(DIFFUSE, texturePath.c_str(), true, GL_LINEAR);
    tessShader->addTexture(textureHeight);
    //tessShader->addTexture(textureDiffuse);

    TessalationMaterial* tessMaterial = new TessalationMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.5f,0.5f,0.5f), glm::vec3(1.0f,1.0f,1.0f), 32.0f, 0.0f, 10.0f, 5.0f);

    Model* surfaceModel = ResourceManager::loadModel(planePath.c_str());

    Camera* camera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), Camera_Projection::PERSP, 45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    ResourceManager::setActiveCamera(camera);
    camera->setMode(Camera_Mode::FREE);

    GameObject* surface = ResourceManager::loadGameObject();
    RenderModule* renderModule = new RenderModule(surfaceModel, tessMaterial, tessShader);
    surface->addModule(renderModule);
    surface->Rotate(glm::vec3(-90.0f, 0.0f, 0.0f));

    ImGuiWrapper::attachGuiFunction("Point Light", [pointLight](){pointLight->OnGui();});
    ImGuiWrapper::attachGuiFunction("Material Properties", [tessMaterial](){tessMaterial->OnGui();});
    ImGuiWrapper::attachGuiFunction("Surface Properties", [surface](){surface->OnGui();});
    ImGuiWrapper::attachGuiFunction("Frame Rate", [](){ImGui::Text("Frame Rate: %.1f", ImGui::GetIO().Framerate);});
}

#endif // TEST_H    