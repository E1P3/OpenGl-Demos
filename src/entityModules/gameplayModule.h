#ifndef GAMEPLAY_MODULE_H
#define GAMEPLAY_MODULE_H

#include "../entityModule.h"
#include "../shader.h"
#include "../resourceManager.h"

// Module template

class GameplayModule : public EntityModule {
    public:
        GameplayModule(){
            this->rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
            this->rotationSpeed = 0.5f;
        }

        GameplayModule(glm::vec3 rotationAxis, float rotationSpeed){
            this->rotationAxis = rotationAxis;
            this->rotationSpeed = rotationSpeed;
        }
        
        ~GameplayModule();

        void OnUpdate(){
            float deltaTime = ResourceManager::getDeltaTime();
            if(isControlled){
                rotationAxis = glm::vec3(0.0f, 0.0f, 0.0f);   
                if(ResourceManager::isKeyPressed(GLFW_KEY_W)){
                    rotationAxis.z = 1.0f;
                }
                if(ResourceManager::isKeyPressed(GLFW_KEY_S)){
                    rotationAxis.z = -1.0f;
                }
                if(ResourceManager::isKeyPressed(GLFW_KEY_A)){
                    rotationAxis.x = -1.0f;
                }
                if(ResourceManager::isKeyPressed(GLFW_KEY_D)){
                    rotationAxis.x = 1.0f;
                }
                if(ResourceManager::isKeyPressed(GLFW_KEY_Q)){
                    rotationAxis.y = 1.0f;
                }
                if(ResourceManager::isKeyPressed(GLFW_KEY_E)){
                    rotationAxis.y = -1.0f;
                }
            }

            if(isEuler){
                this->getParent()->Rotate(glm::degrees(rotationAxis * rotationSpeed * deltaTime));
            } else {
                this->getParent()->Rotate(glm::quat(rotationAxis * rotationSpeed * deltaTime));
            }
        }

        void OnStart(){

        }
        
        void setControlled(bool isControlled){
            this->isControlled = isControlled;
        }

        void setEuler(bool isEuler){
            this->isEuler = isEuler;
        }
    private:
        bool isControlled = false;
        bool isEuler = false;
        glm::vec3 rotationAxis;
        float rotationSpeed;
};

#endif // GAMEPLAY_MODULE_H
