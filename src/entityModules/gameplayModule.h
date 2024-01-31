#ifndef GAMEPLAY_MODULE_H
#define GAMEPLAY_MODULE_H

#include "../entityModule.h"
#include "../shader.h"
#include "../resourceManager.h"

// Module template

class GameplayModule : public EntityModule {
    public:
        GameplayModule(int gameid){
            this->gameid = gameid;
        }
        
        ~GameplayModule();

        void OnUpdate(){
            float deltaTime = ResourceManager::getDeltaTime();
            float rotationSpeed = 0.5f * deltaTime;
            bool moved = false;
            if(ResourceManager::isKeyPressed(GLFW_KEY_Q)){
                this->getParent()->Rotate(glm::quat(glm::vec3(0.0f, rotationSpeed, 0.0f)));
                moved = true;
            }

            if(ResourceManager::isKeyPressed(GLFW_KEY_E)){
                this->getParent()->Rotate(glm::quat(glm::vec3(rotationSpeed, 0.0f, 0.0f)));
                moved = true;
            }

            if(ResourceManager::isKeyPressed(GLFW_KEY_R)){
                this->getParent()->Rotate(glm::quat(glm::vec3(0.0f, 0.0f, rotationSpeed)));
                moved = true;
            }

            if(moved){
                glm::vec3 rotationEuler = glm::degrees(glm::eulerAngles(this->getParent()->getRotation()));
                std::cout << "NewRotation " << rotationEuler.x << " " << rotationEuler.y << " " << rotationEuler.z << std::endl; 
            }

        }

        void OnStart(){
            std::cout << "GameplayModule" << gameid << "::OnStart()" << std::endl;
        }
        
    private:
      int gameid;
};

#endif // GAMEPLAY_MODULE_H
