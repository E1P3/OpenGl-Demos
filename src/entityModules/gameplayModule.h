#ifndef GAMEPLAY_MODULE_H
#define GAMEPLAY_MODULE_H

#include "../entityModule.h"
#include "../shader.h"
#include "../resourceManager.h"

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
};

#endif // GAMEPLAY_MODULE_H
