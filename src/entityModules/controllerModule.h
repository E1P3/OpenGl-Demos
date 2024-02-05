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

    void setRotationSensitivity(float rotationSensitivity) {
        this->rotationSensitivity = rotationSensitivity;
    }

    void OnUpdate() override{

        if (enableRotation) {
            if(ResourceManager::getIsMouseEnabled())
            {
                rotate();
            }
        }
        if (enableMovement) {
            move();
        }
        if (debug) {
            printInfo();
        }

    }
    void setDebug(bool debug){
        this->debug = debug;
    }

    void setEnableRotation(bool enableRotation){
        this->enableRotation = enableRotation;
    }

    void setEnableMovement(bool enableMovement){
        this->enableMovement = enableMovement;
    }

    void setEnableVerticalMovement(bool enableVerticalMovement){
        this->enableVerticalMovement = enableVerticalMovement;
    }

    void OnStart() override{}

    void setActive(bool isActive){
        this->isActive = isActive;
    }

private:
    bool isActive = false;
    bool enableRotation = false;
    bool enableMovement = false;
    bool enableVerticalMovement = true;
    bool debug = false;
    float velocity = 10.0f;
    float rotationSensitivity = 0.1f;
    float currentPitch = 0.0f;

    void move(){
        GameObject* parent = this->getParent(); 

        glm::quat rotation = parent->getRotation();
        rotation.x = 0.0f;
        rotation.z = 0.0f;
        
        glm::vec3 forward = this->getParent()->getFront();
        glm::vec3 right = this->getParent()->getRight();
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

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
        glm::quat rotation = parent->getRotation();
        glm::vec3 right = parent->getRight();
        glm::vec3 up = parent->getUp();

        float yawDelta = ResourceManager::getMouseDeltaX() * rotationSensitivity;
        float pitchDelta = ResourceManager::getMouseDeltaY() * rotationSensitivity;

        glm::quat yawRotation = glm::angleAxis(glm::radians(-yawDelta), up);
        glm::quat pitchRotation = glm::angleAxis(glm::radians(-pitchDelta), right);

        float currentPitch = glm::degrees(glm::pitch(rotation));
        float newPitch = currentPitch - pitchDelta;
        if(
            ((currentPitch <= 89.0f && currentPitch >= 0.0f) && newPitch > 89.0f) || 
            ((currentPitch <= 0.0f && currentPitch >= -89.0f) && newPitch < -89.0f) ||
            ((currentPitch <= 91.0f && currentPitch >= 180.0f) && newPitch > 91.0f) ||
            ((currentPitch <= 180.0f && currentPitch >= -91.0f) && newPitch < -91.0f)
        ){
            pitchRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        }
        rotation = glm::normalize(yawRotation * rotation);

        parent->setRotation(rotation);
    }

    void printInfo() {
        GameObject* parent = this->getParent();
        glm::quat rotation = parent->getRotation();
        glm::vec3 rotationEuler= glm::degrees(glm::eulerAngles(rotation));

        std::cout << "Quaternion: " << rotation.x << " " << rotation.y << " " << rotation.z << " " << rotation.w << " | ";
        std::cout << "Euler: " << rotationEuler.x << " " << rotationEuler.y << " " << rotationEuler.z << " | ";
        
    }


};
#endif // CONTROLLER_MODULE_H