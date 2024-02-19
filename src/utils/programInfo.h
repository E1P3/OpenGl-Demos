#ifndef PROGRAM_INFO_H
#define PROGRAM_INFO_H

#include <iostream>
#include "../resourceManager.h"

class ProgramInfo {
public:

    static void printFrameRate() {
        float frameRate = 1.0f / ResourceManager::getDeltaTime();
        std::cout << "Frame rate: " << frameRate << std::endl;
    }

    static void printMousePosition() {
        std::cout << "Mouse Position: (" << ResourceManager::getMouseX() << ", " << ResourceManager::getMouseY() << ")" << std::endl;
    }

    static void printKeysPressed() {
            std::cout << "Keys pressed: ";
        for (int i = 0; i < 1024; i++) {
            if (ResourceManager::isKeyPressed(i)) {
                const char* keyName = glfwGetKeyName(i, 0);
                if (keyName != nullptr) {
                    std::cout << keyName << " ";
                }
            }
        }
        std::cout << std::endl;
    }

    static void printAllInfo() {
        printFrameRate();
        printMousePosition();
        printKeysPressed();
    }

};

#endif
