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

        void OnStart() override{
            if(shader != nullptr){
                shader->bindRenderModule(this);
            }
        }

        void OnUpdate() override{
            
        }

        glm::vec3 getClosestVertexToMouseClick() {
            int mouseX = ResourceManager::getMouseX();
            int mouseY = ResourceManager::getMouseY();
            int screenWidth = ResourceManager::getScreenWidth();
            int screenHeight = ResourceManager::getScreenHeight();
            glm::mat4 projectionMatrix = ResourceManager::getActiveCamera()->getProjectionMatrix();
            glm::mat4 viewMatrix = ResourceManager::getActiveCamera()->getViewMatrix();
            glm::mat4 modelMatrix = this->getParent()->getTransform();
            std::vector<glm::vec3> vertices = this->model->getVertices();

            glm::mat4 MV = modelMatrix * viewMatrix;

            glm::vec3 window = glm::vec3(mouseX, screenHeight - mouseY - 1, 0);
            glReadPixels(mouseX, screenHeight - mouseY - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &window.z);

            std::cout << "Window: " << window.x << " " << window.y << " " << window.z << std::endl;
            glm::vec3 world = glm::unProject(window, MV, projectionMatrix, glm::vec4(0, 0, screenWidth, screenHeight));

            glm::vec3 closestVertex = glm::vec3(0, 0, 0);
            float closestDistance = 1000000;
            for (int i = 0; i < vertices.size(); i++) {
                glm::vec4 vertex = modelMatrix * glm::vec4(vertices[i], 1);
                float distance = glm::distance(glm::vec3(vertex), world);
                if (distance < closestDistance) {
                    closestDistance = distance;
                    closestVertex = glm::vec3(vertex);
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
