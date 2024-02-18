#ifndef ANIM_SHADER_H
#define ANIM_SHADER_H

#include "../../../shader.h"
#include "../../../resourceManager.h"
#include "../../../entityModules/renderModule.h"
#include "../../../bone.h"

class AnimShader : public Shader {

public:
    AnimShader(const char* PVS, const char* PFS) {
        this->Compile(this->readShaderSource(PVS), this->readShaderSource(PFS));
    }

    void Render() override {
        this->Use();

        this->SetMatrix4("view", ResourceManager::getActiveCamera()->getViewMatrix());
        this->SetMatrix4("projection", ResourceManager::getActiveCamera()->getProjectionMatrix());

        for(RenderModule* module : objectsToRender){
            this->SetMatrix4("model", module->getParent()->getTransform());
            module->material->Draw(this); // use basic matrial
            module->model->Draw(this, true, false);
        }

    }
};

#endif // ANIM_SHADER_H
