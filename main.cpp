#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include "src/resourceManager.h"
#include "src/entityModules/controllerModule.h"
#include "src/entityModules/renderModule.h"
#include "src/entityModules/camera.h"
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

int main() {

    GLFWwindow* window = ResourceManager::createWindow(1920, 1080);

    ResourceManager::initialize();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        clearCommandLine();
        // Process events
        glfwPollEvents();

        // Update
        ResourceManager::runGameLoop();

        // Rendering code goes here
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Clean up
    glfwTerminate();
    return 0;
}
