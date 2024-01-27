#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include "src/resourceManager.h"
#include "src/gameObject.h"
#include "src/entityModules/gameplayModule.h"

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Mac m1 will automatically load OpenGL 2.1 without hints enabled
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Graphics", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize Glad" << std::endl;
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL Version: " << GLVersion.major << "." << GLVersion.minor << std::endl;


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

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Process events
        glfwPollEvents();

        gameObject.OnUpdate();
        gameObject.OnRenderPass();
        // Rendering code goes here
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Clean up
    glfwTerminate();
    return 0;
}
