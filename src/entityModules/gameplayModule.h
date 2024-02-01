#ifndef GAMEPLAY_MODULE_H
#define GAMEPLAY_MODULE_H

#include "../entityModule.h"
#include "../shader.h"
#include "../resourceManager.h"

<<<<<<< HEAD
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
=======
class GameplayModule : public entityModule {
    public:
        GameplayModule(int id){
            this->id = id;
        }
        
        ~GameplayModule();
        
        void OnRenderPass(){
            std::cout << "GameplayModule" << id << "::OnRenderPass()" << std::endl;
        }

        void OnUpdate(){
            std::cout << "GameplayModule" << id << "::OnUpdate()" << std::endl;
        }

        void OnStart(){
            std::cout << "GameplayModule" << id << "::OnStart()" << std::endl;
        }
        
    private:
      int id;
>>>>>>> 951bcb1f6a5306fddea8d34d5400f199987be5e5
};

#endif // GAMEPLAY_MODULE_H
