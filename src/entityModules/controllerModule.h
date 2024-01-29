#ifndef CONTROLLER_MODULE_H
#define CONTROLLER_MODULE_H

#include <GLFW/glfw3.h>
#include "../entityModule.h"
#include "../resourceManager.h"

class ControllerModule : public EntityModule {
public:

    ControllerModule(){
        this->enableMovement = true;
        this->enableRotation = true;
    }

    ControllerModule(bool enableMovement, bool enableRotation, bool enableVerticalMovement = true){
        this->enableMovement = enableMovement;
        this->enableRotation = enableRotation;
        this->enableVerticalMovement = enableVerticalMovement;
    }

    ~ControllerModule() = default;

    void setVelocity(float velocity) {
        this->velocity = velocity;
    }

    void setMouseSensitivity(float mouseSensitivity) {
        this->mouseSensitivity = mouseSensitivity;
    }

    void OnUpdate() override{
        if (enableRotation) {
            rotate();
        }
        if (enableMovement) {
            move();
        }
        if (debug) {
            printInfo();
        }
    }

    void OnRenderPass() override{}
    void OnStart() override{}

private:
    bool enableRotation = false;
    bool enableMovement = false;
    bool enableVerticalMovement = true;
    bool debug = false;
    float velocity = 10.0f;
    float mouseSensitivity = 0.1f;

    void move(){
        GameObject* parent = this->getParent();
        
        glm::quat orientation = parent->getRotation(); 

        glm::vec3 forward = orientation * glm::vec3(0, 0, -1); 
        glm::vec3 right = orientation * glm::vec3(1, 0, 0);   
        glm::vec3 up = orientation * glm::vec3(0, 1, 0);      

        glm::vec3 direction(0.0f);

        if (ResourceManager::isKeyPressed(GLFW_KEY_W)) {
            direction += forward;
        }
        if (ResourceManager::isKeyPressed(GLFW_KEY_S)) {
            direction -= forward;
        }
        if (ResourceManager::isKeyPressed(GLFW_KEY_D)) {
            direction += right;
        }
        if (ResourceManager::isKeyPressed(GLFW_KEY_A)) {
            direction -= right;
        }
        if(enableVerticalMovement){
            if (ResourceManager::isKeyPressed(GLFW_KEY_SPACE)) {
                direction += up;
            }
            if (ResourceManager::isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
                direction -= up;
            }
        }

        if (glm::length(direction) > 0.0f) {
            direction = glm::normalize(direction);
            glm::vec3 newPosition = parent->getPosition() + direction * velocity * ResourceManager::getDeltaTime();
            parent->setPosition(newPosition);
        }

    }

    void rotate() {
        GameObject* parent = this->getParent();

        float yaw = ResourceManager::getMouseDeltaX() * mouseSensitivity;
        float pitch = ResourceManager::getMouseDeltaY() * mouseSensitivity;

        glm::quat orientation = parent->getRotation();
        glm::quat yawRotation = glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0));
        glm::quat pitchRotation = glm::angleAxis(glm::radians(pitch), glm::vec3(1, 0, 0));

        glm::quat newRotation = glm::normalize(pitchRotation * yawRotation * orientation);
        parent->rotate(newRotation);
    }

    void printInfo() {
        GameObject* parent = this->getParent();
        glm::vec3 position = parent->getPosition();
        glm::quat rotation = parent->getRotation();

        std::cout << "Position: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
        std::cout << "Rotation: (" << rotation.x << ", " << rotation.y << ", " << rotation.z << ", " << rotation.w << ")" << std::endl;
    }

};
#endif // CONTROLLER_MODULE_H