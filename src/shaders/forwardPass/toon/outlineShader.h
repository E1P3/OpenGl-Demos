#ifndef OUTLINE_SHADER_H
#define OUTLINE_SHADER_H

#include "../../../shader.h"
#include "../../../resourceManager.h"
#include "../../../entityModules/renderModule.h"


class OutlineShader : public Shader {
public:
    OutlineShader(const char* PVS, const char* PFS) {
        this->Compile(this->readShaderSource(PVS), this->readShaderSource(PFS));
    }

    void Render() override {    
        this->Use();
        glCullFace(GL_FRONT);
        glEnable(GL_CULL_FACE);
        // Load camera uniforms

        this->SetMatrix4("view", ResourceManager::getActiveCamera()->getViewMatrix());
        this->SetMatrix4("projection", ResourceManager::getActiveCamera()->getProjectionMatrix());
        // Load RenderModule uniforms

        Shader::Render();
        glDisable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
};

#endif // OUTLINE_SHADER_H