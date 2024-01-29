#ifndef GAMEPLAY_MODULE_H
#define GAMEPLAY_MODULE_H

#include "../entityModule.h"
#include "../shader.h"
#include "../resourceManager.h"

class GameplayModule : public EntityModule {
    public:
        GameplayModule(int gameid){
            this->gameid = gameid;
        }
        
        ~GameplayModule();
        
        void OnRenderPass(){
            std::cout << "GameplayModule" << gameid << "::OnRenderPass()" << std::endl;
        }

        void OnUpdate(){
            std::cout << "GameplayModule" << gameid << "::OnUpdate()" << std::endl;
        }

        void OnStart(){
            std::cout << "GameplayModule" << gameid << "::OnStart()" << std::endl;
        }
        
    private:
      int gameid;
};

#endif // GAMEPLAY_MODULE_H
