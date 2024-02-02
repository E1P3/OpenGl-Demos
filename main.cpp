#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include "src/resourceManager.h"
#include <string>
#include "demos/animation1.h"
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

    ImGuiWrapper* imguiWrapper = new ImGuiWrapper();
    setUpScene(imguiWrapper);

    ResourceManager::initialize();

    float timer = 0;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        //clearCommandLine();
        // Process events
        glfwPollEvents();

        

        // center camera on start
        if (timer < 0.5f) {
            ResourceManager::getActiveCamera()->getParent()->setRotation(glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)));
            timer += ResourceManager::getDeltaTime();
        }

        // Update
        ResourceManager::runGameLoop();
        imguiWrapper->update();
        imguiWrapper->render();

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Clean up
    glfwTerminate();
    return 0;
}
