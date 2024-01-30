#ifndef PBR_SHADER_H
#define PBR_SHADER_H

#include "../../shader.h"
#include "../../resourceManager.h"
#include "../../entityModules/renderModule.h"


class PBRShader : public Shader {
public:
    PBRShader(const char* PVS, const char* PFS) {
        this->Compile(this->readShaderSource(PVS), this->readShaderSource(PFS));
    }

    void Render() override {        
        this->Use();

        // Load camera uniforms

        this->SetMatrix4("view", ResourceManager::getActiveCamera()->getViewMatrix());
        this->SetMatrix4("projection", ResourceManager::getActiveCamera()->getProjectionMatrix());
        this->SetVector3f("camPos", pointLightsToRender[0]->getPosition());

        if(pointLightsToRender[0] != nullptr){
            this->SetVector3f("lightPositions[" + std::to_string(0) + "]", pointLightsToRender[0]->getPosition());
            this->SetVector3f("lightColors[" + std::to_string(0) + "]", glm::vec3(20.0f,20.0f,20.0f));
        }


        // Load RenderModule uniforms

        for(RenderModule* module : objectsToRender){
            this->SetMatrix4("model", module->getParent()->getTransform());
            this->SetVector3f("albedo", glm::vec3(1.0f,1.0f,1.0f));
            this->SetFloat("metallic", 0.1f);
            this->SetFloat("roughness", 0.3f);
            this->SetFloat("ao", 0.1f);
            module->model->Draw(this);
        }
    }

};

#endif // PBR_SHADER_H