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

        ResourceManager::setMouseEnabled(ResourceManager::isMousePressed(GLFW_MOUSE_BUTTON_RIGHT));

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

private:
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
        
        glm::vec3 forward = glm::rotate(rotation, glm::vec3(0.0f, 0.0f, -1.0f));
        glm::vec3 right = glm::rotate(rotation, glm::vec3(1.0f, 0.0f, 0.0f));
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

        float yawDelta = ResourceManager::getMouseDeltaX() * rotationSensitivity;
        float pitchDelta = ResourceManager::getMouseDeltaY() * rotationSensitivity;

        glm::quat yawRotation = glm::angleAxis(glm::radians(-yawDelta), glm::vec3(0, 1, 0));
        glm::quat pitchRotation = glm::angleAxis(glm::radians(-pitchDelta), right);

        rotation = glm::normalize(yawRotation * pitchRotation * rotation);

        parent->setRotation(rotation);
    }





    void rotateWSAD(){
        GameObject* parent = this->getParent();

        glm::quat orientation = parent->getRotation();

        float yawDelta = 0.0f; 
        float pitchDelta = 0.0f;

        if (ResourceManager::isKeyPressed(GLFW_KEY_W)) {
            pitchDelta = 1.0f * ResourceManager::getDeltaTime() * rotationSensitivity;
        }
        if (ResourceManager::isKeyPressed(GLFW_KEY_S)) {
            pitchDelta = -1.0f * ResourceManager::getDeltaTime() * rotationSensitivity;
        }
        if (ResourceManager::isKeyPressed(GLFW_KEY_D)) {
            yawDelta = 1.0f * ResourceManager::getDeltaTime() * rotationSensitivity;
        }
        if (ResourceManager::isKeyPressed(GLFW_KEY_A)) {
            yawDelta = -1.0f * ResourceManager::getDeltaTime() * rotationSensitivity;
        }

        glm::quat yawRotation = glm::angleAxis(glm::radians(-yawDelta * rotationSensitivity), glm::vec3(0, 1, 0));
        glm::quat pitchRotation = glm::angleAxis(glm::radians(-pitchDelta * rotationSensitivity), glm::vec3(1, 0, 0));

        orientation = glm::normalize(yawRotation * pitchRotation * orientation);
        parent->setRotation(orientation);
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