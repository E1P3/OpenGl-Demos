#ifndef RENDER_MODULE_H
#define RENDER_MODULE_H

#include "../entityModule.h"
#include "../resourceManager.h"
#include <glm/gtc/matrix_inverse.hpp>

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

        RenderModule(GameObject* parent, Model* model, Material* material, Shader* shader, bool exposeGometry = false){
            this->model = model;
            this->material = material;
            this->shader = shader;
            parent->addModule(this);
            if(exposeGometry){
                std::vector<Vertex> vertices = this->model->getVertices();
                std::vector<glm::vec3> positions;
                for(Vertex vertex : vertices){
                    positions.push_back(vertex.Position);
                }
                ResourceManager::addGeometryInfo(parent, positions);
            }
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
        bool isEnabled = true;     
};

#endif // RENDER_MODULE_H
