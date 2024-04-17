#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include "src/resourceManager.h"
#include <string>
#include "demos/rendering3/rendering3.h"
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



int main() {

    GLFWwindow* window = ResourceManager::createWindow(1920, 1080);

    ImGuiWrapper::init();
    setUpScene();

    ResourceManager::initialize();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        //clearCommandLine();
        // Process events
        glfwPollEvents();

        // Update
        ResourceManager::runGameLoop();
        ImGuiWrapper::update();
        ImGuiWrapper::render();

        // Swap buffers
        glfwSwapBuffers(window);
    }

    ImGuiWrapper::shutdown();
    // Clean up
    glfwTerminate();
    return 0;
}
