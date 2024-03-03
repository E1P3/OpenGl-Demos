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
                ResourceManager::addGeometryInfo(parent, this->model->getVertices());
            }
        }

        void OnStart() override{
            if(shader != nullptr){
                shader->bindRenderModule(this);
            }
        }

        void OnUpdate() override{
            
        }

        glm::vec3 getClosestVertexToMouseClick(glm::vec3 mouseRayDirection) {
            glm::mat4 modelMatrix = this->getParent()->getTransform();
            std::vector<glm::vec3> vertices = this->model->getVertices();

            glm::vec3 closestVertex = glm::vec3(0, 0, 0);
            glm::vec3 initialVertex = glm::vec3(0, 0, 0);
            float closestDistance = 1000000;
            for (int i = 0; i < vertices.size(); i++) {
                glm::vec4 vertex = modelMatrix * glm::vec4(vertices[i], 1);
                float distance = glm::distance(glm::vec3(vertex), mouseRayDirection);
                if (distance < closestDistance) {
                    closestDistance = distance;
                    closestVertex = glm::vec3(vertex);
                    initialVertex = vertices[i];
                }
            }
            
            return closestVertex;
        }

        ~RenderModule();
        
        Shader* shader;
        Model* model;
        Material* material;
        bool isEnabled = true;     
};

#endif // RENDER_MODULE_H
