#ifndef PBR_SHADER_H
#define PBR_SHADER_H

#include "../../../shader.h"
#include "../../../resourceManager.h"
#include "../../../entityModules/renderModule.h"


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
        this->SetVector3f("camPos", ResourceManager::getActiveCamera()->getPosition());

        for(int i = 0; i < pointLightsToRender.size(); i++){
            this->SetVector3f("lightPositions[" + std::to_string(i) + "]", pointLightsToRender[i]->getPosition());
            this->SetVector3f("lightColors[" + std::to_string(i) + "]", glm::vec3(155.0f,155.0f,155.0f));
        }

        this->SetInteger("numPointLights", pointLightsToRender.size());

        // Load RenderModule uniforms

        for(RenderModule* module : objectsToRender){
            this->SetMatrix4("model", module->getParent()->getTransform());
            module->material->Draw(this);
            module->model->Draw(this);
        }
    }

};

#endif // PBR_SHADER_H