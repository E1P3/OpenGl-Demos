#ifndef GLASS_SHADER_H
#define GLASS_SHADER_H

#include "../../../shader.h"
#include "../../../resourceManager.h"
#include "../../../entityModules/renderModule.h"
#include "../../../cubemap.h"


class GlassShader : public Shader {
public:
    GlassShader(const char* PVS, const char* PFS, Cubemap* cubemap) {
        this->Compile(this->readShaderSource(PVS), this->readShaderSource(PFS));
        this->cubemap = cubemap;
    }

    void Render() override {
        this->Use();

        // Load camera uniforms

        this->SetMatrix4("view", ResourceManager::getActiveCamera()->getViewMatrix());
        this->SetMatrix4("projection", ResourceManager::getActiveCamera()->getProjectionMatrix());
        this->SetVector3f("camPos", ResourceManager::getActiveCamera()->getPosition());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->getID());

        // Load RenderModule uniforms

        Shader::Render();

    }


    void setCubeMap(Cubemap* cubemap) {
        this->cubemap = cubemap;
    }

    Cubemap* getCubeMap() {
        return this->cubemap;
    }
    
private:
    Cubemap* cubemap;

};

#endif // GLASS_SHADER_H