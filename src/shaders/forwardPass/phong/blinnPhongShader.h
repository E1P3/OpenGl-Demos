#ifndef BLINN_PHONG_SHADER_H
#define BLINN_PHONG_SHADER_H

#include "../../../shader.h"
#include "../../../resourceManager.h"
#include "../../../entityModules/renderModule.h"


class blinnPhongShader : public Shader {
public:
    blinnPhongShader(const char* PVS, const char* PFS) {
        this->Compile(this->readShaderSource(PVS), this->readShaderSource(PFS));
    }

    void Render() override {
        this->Use();

        // Load camera uniforms

        this->SetMatrix4("view", ResourceManager::getActiveCamera()->getViewMatrix());
        this->SetMatrix4("projection", ResourceManager::getActiveCamera()->getProjectionMatrix());

        if(pointLightsToRender[0] != nullptr){
            this->SetVector3f("pointLight.position", pointLightsToRender[0]->getPosition());
            this->SetVector3f("pointLight.ambient", pointLightsToRender[0]->getAmbient());
            this->SetVector3f("pointLight.diffuse", pointLightsToRender[0]->getDiffuse());
            this->SetVector3f("pointLight.specular", pointLightsToRender[0]->getSpecular());
        }

        if(dirLightsToRender[0] != nullptr){
            this->SetVector3f("dirLight.direction", glm::eulerAngles(dirLightsToRender[0]->getRotation()));
            this->SetVector3f("dirLight.ambient", dirLightsToRender[0]->getAmbient());
            this->SetVector3f("dirLight.diffuse", dirLightsToRender[0]->getDiffuse());
            this->SetVector3f("dirLight.specular", dirLightsToRender[0]->getSpecular());
        }


        // Load RenderModule uniforms

        for(RenderModule* module : objectsToRender){
            this->SetMatrix4("model", module->getParent()->getTransform());
            module->material->Draw(this);
            module->model->Draw(this);
        }

    }

};

#endif // BLINN_PHONG_SHADER_H