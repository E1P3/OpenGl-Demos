#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include "src/resourceManager.h"
#include "src/entityModules/controllerModule.h"
#include "src/entityModules/renderModule.h"
#include "src/entityModules/camera.h"
#include "src/shaders/forwardPass/forwardShader.h"
#include "src/shaders/forwardPass/blinnPhongShader.h"
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

void clearCommandLine() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void setUpScene(){

    std::string modelPath = std::string(ASSET_DIR) + "/models/sphere.dae";
    std::string vShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/blinnPhong.vert";
    std::string fShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/blinnPhong.frag";

    DirectionalLight* directionalLight = ResourceManager::loadDirectionalLight(0.1f, glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)));
    PointLight* pointLight = ResourceManager::loadPointLight(0.1f, glm::vec3(3.0f, 0.0f, 3.0f), 1.0f, 0.09f, 0.032f);

    Shader* shader = new blinnPhongShader(vShaderPath.c_str(), fShaderPath.c_str());
    shader->setDebug(false);
    ResourceManager::addShader(shader);
    Material *material = ResourceManager::loadMaterial(glm::vec3(1.0f,1.0f,1.0f), glm::vec3(0.5f,0.5f,0.5f), glm::vec3(1.0f,1.0f,1.0f), 32.0f);
    Model* model = ResourceManager::loadModel(modelPath.c_str());

    GameObject* gameObject = ResourceManager::loadGameObject();
    Camera* camera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), Camera_Projection::PERSP, 45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    ControllerModule* controllerModule = new ControllerModule(true, true, true);
    gameObject->addModule(controllerModule);
    gameObject->addModule(camera);
    ResourceManager::setActiveCamera(camera);

    GameObject* gameObject2 = ResourceManager::loadGameObject();
    RenderModule* renderModule = new RenderModule(model, material, shader);
    gameObject2->addModule(renderModule);
    gameObject2->Translate(glm::vec3(0.0f, 0.0f, -10.0f));

    gameObject->Translate(glm::vec3(0.0f, 0.0f, 10.0f));
    camera->lookAt(gameObject2->getPosition());
}

int main() {

    GLFWwindow* window = ResourceManager::createWindow(1920, 1080);

    setUpScene();

    ResourceManager::initialize();

    float timer = 0;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        //clearCommandLine();
        // Process events
        glfwPollEvents();

        if (timer < 1.0f) {
            ResourceManager::getActiveCamera()->getParent()->setRotation(glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)));
            timer += ResourceManager::getDeltaTime();
        }

        // Update
        ResourceManager::runGameLoop();

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Clean up
    glfwTerminate();
    return 0;
}
