#ifndef FORWARD_SHADER_H
#define FORWARD_SHADER_H

#include "../../shader.h"
#include "../../resourceManager.h"
#include "../../entityModules/renderModule.h"


class ForwardShader : public Shader {
public:
    ForwardShader(const char* PVS, const char* PFS) {
        this->Compile(this->readShaderSource(PVS), this->readShaderSource(PFS));
    }

    void Render() override {
        this->Use();

        // Load camera uniforms

        this->SetVector3f("viewPos", ResourceManager::getActiveCamera()->getParent()->getPosition());
        this->SetMatrix4("view", ResourceManager::getActiveCamera()->getViewMatrix());
        this->SetMatrix4("proj", ResourceManager::getActiveCamera()->getProjectionMatrix());

        // Load point light uniforms

        for (int i = 0; i < pointLightsToRender.size(); i++) {
            this->SetVector3f("pointLights[" + std::to_string(i) + "].position", pointLightsToRender[i]->getPosition());
            this->SetVector3f("pointLights[" + std::to_string(i) + "].ambient", pointLightsToRender[i]->getAmbient());
            this->SetVector3f("pointLights[" + std::to_string(i) + "].diffuse", pointLightsToRender[i]->getDiffuse());
            this->SetVector3f("pointLights[" + std::to_string(i) + "].specular", pointLightsToRender[i]->getSpecular());
            this->SetFloat("pointLights[" + std::to_string(i) + "].strength", pointLightsToRender[i]->getStrength());
        }

        this->SetInteger("numPointLights", pointLightsToRender.size());

        // Load directional light uniforms

        for (int i = 0; i < dirLightsToRender.size(); i++) {
            glm::vec3 origin = glm::vec3(0.0f);
            glm::mat4 lightViewMatrix = glm::lookAt(dirLightsToRender[i]->getPosition(), origin, glm::vec3(0.0f, 1.0f, 0.0f));
            glm::quat rotation = dirLightsToRender[i]->getRotation();
            glm::vec3 front = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
            this->SetVector3f("dirLights[" + std::to_string(i) + "].direction", front);
            this->SetVector3f("dirLights[" + std::to_string(i) + "].ambient", dirLightsToRender[i]->getAmbient());
            this->SetVector3f("dirLights[" + std::to_string(i) + "].diffuse", dirLightsToRender[i]->getDiffuse());
            this->SetVector3f("dirLights[" + std::to_string(i) + "].specular", dirLightsToRender[i]->getSpecular());
            this->SetFloat("dirLights[" + std::to_string(i) + "].strength", dirLightsToRender[i]->getStrength());
        }

        this->SetInteger("numDirLights", dirLightsToRender.size());

        // Load RenderModule uniforms

        for(RenderModule* module : objectsToRender){
            this->SetInteger("isAnimated", 0);
            this->SetInteger("hasShadow", 0);
            this->SetInteger("texture_diffuse1", 1);
            this->SetInteger("texture_normal1", 2, false);
            this->SetMatrix4("model", module->getParent()->getTransform());
            module->material->Draw(this);
            module->model->Draw(this);
        }

    }

};

#endif // FORWARD_SHADER_H