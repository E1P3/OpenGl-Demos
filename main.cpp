#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include "src/resourceManager.h"
#include <string>
#include "demos/rendering1.h"

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
