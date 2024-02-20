#ifndef RENDER_MODULE_H
#define RENDER_MODULE_H

#include "../entityModule.h"
#include "../resourceManager.h"

class Model;
class Material;
class Shader;

class RenderModule : public EntityModule {
    public:
        RenderModule(Model* model, Material* material, Shader* shader){
            this->model = model;
            this->material = material;
            this->shader = shader;
        }

        void OnStart() override{
            if(shader != nullptr){
                shader->bindRenderModule(this);
            }
        }

        void OnUpdate() override{
            
        }
        ~RenderModule();
        
        Shader* shader;
        Model* model;
        Material* material;        
};

#endif // RENDER_MODULE_H
