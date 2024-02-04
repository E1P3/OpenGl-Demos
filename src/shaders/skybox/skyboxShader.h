#ifndef SKYBOX_SHADER_H
#define SKYBOX_SHADER_H

#include "../../shader.h"
#include "../../resourceManager.h"
#include "../../cubemap.h"
#include <vector>

class SkyboxShader : public Shader {
public:
    SkyboxShader(const char* PVS, const char* PFS, Cubemap* skybox) {
        this->Compile(this->readShaderSource(PVS), this->readShaderSource(PFS));
        this->skybox = skybox;
        this->setupBox();
    }

    void Render() override {    

        this->Use();
        this->SetMatrix4("view", glm::mat4(glm::mat3(ResourceManager::getActiveCamera()->getViewMatrix())));
        this->SetMatrix4("projection", ResourceManager::getActiveCamera()->getProjectionMatrix());
        DrawBox();

    }

    void setupBox(){
        float vertices[] = {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    }

    void DrawBox(){
        glDepthFunc(GL_LEQUAL);
        glBindVertexArray(VAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getID());
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    }

    void setCubeMap(Cubemap* skybox) {
        this->skybox = skybox;
    }

    Cubemap* getCubeMap() {
        return this->skybox;
    }

private:
    unsigned int VAO, VBO, EBO;
    Cubemap* skybox;
    
};

#endif // SKYBOX_SHADER_H