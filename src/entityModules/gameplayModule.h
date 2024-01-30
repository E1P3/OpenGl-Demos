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
            float rotationSpeed = 0.5f;
            this->getParent()->Rotate(glm::quat(glm::vec3(0.0f, rotationSpeed * deltaTime, 0.0f)));
        }

        void OnStart(){
            std::cout << "GameplayModule" << gameid << "::OnStart()" << std::endl;
        }
        
    private:
      int gameid;
};

#endif // GAMEPLAY_MODULE_H
