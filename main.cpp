#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include "src/resourceManager.h"
<<<<<<< HEAD
#include <string>
#include "demos/rendering1.h"
#include "src/imgui/imguiWrapper.h"

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


=======
#include "src/gameObject.h"
#include "src/entityModules/gameplayModule.h"
>>>>>>> 951bcb1f6a5306fddea8d34d5400f199987be5e5

int main() {

    GLFWwindow* window = ResourceManager::createWindow(1920, 1080);

<<<<<<< HEAD
    ImGuiWrapper* imguiWrapper = new ImGuiWrapper();
    setUpScene(imguiWrapper);
=======
    // Create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Graphics", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
>>>>>>> 951bcb1f6a5306fddea8d34d5400f199987be5e5

    ResourceManager::initialize();


<<<<<<< HEAD



    float timer = 0;
=======
    // std::string textureFile = std::string(ASSET_DIR) + "/textures/bateman_texture.png";
    // std::cout << textureFile << std::endl;
    // Texture* texture = ResourceManager::loadTexture(NORMAL, textureFile.c_str());

    // std::string modelFile = std::string(ASSET_DIR) + "/models/bateman.dae";
    // std::cout << modelFile << std::endl;
    // Model* model = ResourceManager::loadModel(modelFile.c_str());

    GameObject gameObject;
    GameplayModule* gameplayModule1 = new GameplayModule(1);
    GameplayModule* gameplayModule2 = new GameplayModule(2);
    gameObject.addModule(gameplayModule1);
    gameObject.addModule(gameplayModule2);

    gameObject.OnStart();
>>>>>>> 951bcb1f6a5306fddea8d34d5400f199987be5e5

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        //clearCommandLine();
        // Process events
        glfwPollEvents();

<<<<<<< HEAD
        

        // center camera on start
        if (timer < 0.5f) {
            ResourceManager::getActiveCamera()->getParent()->setRotation(glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)));
            timer += ResourceManager::getDeltaTime();
        }

        // Update
        ResourceManager::runGameLoop();
        imguiWrapper->update();
        imguiWrapper->render();
=======
        gameObject.OnUpdate();
        gameObject.OnRenderPass();
        // Rendering code goes here
        glClear(GL_COLOR_BUFFER_BIT);
>>>>>>> 951bcb1f6a5306fddea8d34d5400f199987be5e5

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Clean up
    glfwTerminate();
    return 0;
}
