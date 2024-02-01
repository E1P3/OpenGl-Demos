#ifndef GAMEPLAY_MODULE_H
#define GAMEPLAY_MODULE_H

#include "../entityModule.h"
#include "../shader.h"
#include "../resourceManager.h"

// Module template

class GameplayModule : public EntityModule {
    public:
        GameplayModule(){
        }
        
        ~GameplayModule();

        void OnUpdate(){
            float deltaTime = ResourceManager::getDeltaTime();
            float rotationSpeed = 0.5f * deltaTime;
            bool moved = false;

            this->getParent()->Rotate(glm::quat(glm::vec3(0.0f, rotationSpeed, 0.0f)));
        }

        void OnStart(){

        }
};

#endif // GAMEPLAY_MODULE_H
