#ifndef RENDER_MODEL_MODULE_H
#define RENDER_MODEL_MODULE_H

#include "../entityModule.h"
#include "../shader.h"
#include "../resourceManager.h"

class renderModelModule : public entityModule {
    public:
        renderModelModule(Model* model, Material* material, Shader* shader){
            this->model = model;
            this->material = material;
            this->shader = shader;
        }
        
        ~renderModelModule();
        
        void OnRenderPass(){
            shader->Use();
            material->Draw(shader);
            model->Draw(shader);
        }

        void OnUpdate(){
            
        }

        void OnStart(){
            
        }
        
    private:
        Shader* shader;
        Model* model;
        Material* material;        
};

#endif // RENDER_MODEL_MODULE_H
