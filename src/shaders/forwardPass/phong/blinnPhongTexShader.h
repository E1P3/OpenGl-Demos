#ifndef BLINN_PHONG_TEX_SHADER_H
#define BLINN_PHONG_TEX_SHADER_H

#include "../../../shader.h"
#include "../../../resourceManager.h"
#include "../../../entityModules/renderModule.h"


class blinnPhongTexShader : public Shader {
public:
    blinnPhongTexShader(const char* PVS, const char* PFS, bool useOwnTextures = true) {
        this->Compile(this->readShaderSource(PVS), this->readShaderSource(PFS));
        this->useOwnTextures = useOwnTextures;
    }

    void setTextureScale(float textureScale){
        this->textureScale = textureScale;
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

        if(!useOwnTextures){
            bindTextures();
        }

        this->SetFloat("textureScale", textureScale);

        Shader::Render();

    }

    void addTextures(std::vector<Texture*> textures){
        for(Texture* texture : textures){
            addTexture(texture);
        }
    }

    void swapTextureArray(std::vector<Texture*> textures){
        this->textures = textures;
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
    bool useOwnTextures;
    float textureScale = 1.0f;
};

#endif // BLINN_PHONG_TEX_SHADER_H