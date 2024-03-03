#ifndef TESS_SHADER_H
#define TESS_SHADER_H

#include "../../../shader.h"
#include "../../../resourceManager.h"
#include "../../../entityModules/renderModule.h"


class TessalationShader : public Shader {
public:
    TessalationShader(const char* PVS, const char* PFS, const char* PGS, const char* PTS, const char* TES) {
        this->Compile(this->readShaderSource(PVS), this->readShaderSource(PFS), this->readShaderSource(PGS), this->readShaderSource(PTS), this->readShaderSource(TES));
    }

    void Render() override {
        this->Use();

        bindTextures();

        this->SetMatrix4("view", ResourceManager::getActiveCamera()->getViewMatrix());
        this->SetMatrix4("projection", ResourceManager::getActiveCamera()->getProjectionMatrix());
        this->SetVector3f("viewPos", ResourceManager::getActiveCamera()->getPosition());

        if(pointLightsToRender[0] != nullptr){
            this->SetVector3f("lightPos", pointLightsToRender[0]->getPosition());
        }

        // Load RenderModule uniforms

        Shader::Render();

    }

    void addTexture(Texture* texture){
        textures.push_back(texture);
        if(texture->getType() == DIFFUSE){
            hasDiffuse = true;
        }
        else if(texture->getType() == SPECULAR){
            hasSpecular = true;
        }
        else if(texture->getType() == NORMAL){
            hasNormal = true;
        }
        else if(texture->getType() == HEIGHT){
            hasHeight = true;
        }
    }

    void bindTextures(){

        int diffuseNr = 1;
        int specularNr = 1;
        int normalNr = 1;
        int heightNr = 1;
        int roughnessNr = 1;

        this->SetInteger("hasDiffuse", 0);
        this->SetInteger("hasSpecular", 0);
        this->SetInteger("hasNormal", 0);
        this->SetInteger("hasHeight", 0);
        this->SetInteger("hasRoughness", 0);

        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i + 1); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name;
            TextureType type = textures[i]->getType();
            if (type == DIFFUSE){
                number = std::to_string(diffuseNr++);
                name = "texture_diffuse";
                this->SetInteger((name).c_str(), i + 1);
                this->SetInteger("hasDiffuse", 1);
            }
            else if (type == SPECULAR){
                number = std::to_string(specularNr++); // transfer unsigned int to string
                name = "texture_specular";
                this->SetInteger((name).c_str(), i + 1);
                this->SetInteger("hasSpecular", 1);
            }
            else if (type == NORMAL){
                number = std::to_string(normalNr++); // transfer unsigned int to string
                name = "texture_normal";
                this->SetInteger((name).c_str(), i + 1);
                this->SetInteger("hasNormal", 1);
            }
            else if (type == HEIGHT){
                number = std::to_string(heightNr++); // transfer unsigned int to string
                name = "texture_height";
                this->SetInteger((name).c_str(), i + 1);
                this->SetInteger("hasHeight", 1);
            }
            else if (type == ROUGHNESS){
                number = std::to_string(heightNr++); // transfer unsigned int to string
                name = "texture_roughness";
                this->SetInteger((name).c_str(), i + 1);
                this->SetInteger("hasRoughness", 1);
            }
            glBindTexture(GL_TEXTURE_2D, textures[i]->getID());
        }
    }

private:
    std::vector<Texture*> textures;
    bool hasDiffuse = false;
    bool hasSpecular = false;
    bool hasNormal = false;
    bool hasHeight = false;
    bool hasRoughness = false;
};

#endif // TESS_SHADER_H