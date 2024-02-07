#ifndef TEXTURED_MATERIAL_H
#define TEXTURED_MATERIAL_H

#include "../material.h"
#include "imgui.h"

class TexturedMaterial : public Material {
public:
    
    TexturedMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, float height_scale) {
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->shininess = shininess;
        this->height_scale = height_scale;
    }

    // Getters
    glm::vec3 getAmbient() const { return this->ambient; }
    glm::vec3 getDiffuse() const { return this->diffuse; }
    glm::vec3 getSpecular() const { return this->specular; }
    float getShininess() const { return this->shininess; }
    float getHeightScale() const { return this->height_scale; }

    // Setters
    void setAmbient(glm::vec3 ambient) { this->ambient = ambient; }
    void setDiffuse(glm::vec3 diffuse) { this->diffuse = diffuse; }
    void setSpecular(glm::vec3 specular) { this->specular = specular; }
    void setShininess(float shininess) { this->shininess = shininess; }
    void setHeightScale(float height_scale) { this->height_scale = height_scale; }

    void Draw(Shader* shader) override {
        shader->SetVector3f("material.ambient", this->ambient);
        shader->SetVector3f("material.diffuse", this->diffuse);
        shader->SetVector3f("material.specular", this->specular);
        shader->SetFloat("material.shininess", this->shininess);
        shader->SetFloat("material.height_scale", this->height_scale);
    }

    void OnGui() override {
        ImGui::ColorEdit3("Ambient", (float*)&ambient);
        ImGui::ColorEdit3("Diffuse", (float*)&diffuse);
        ImGui::ColorEdit3("Specular", (float*)&specular);
        ImGui::SliderFloat("Shininess", &shininess, 0.0f, 256.0f);
        ImGui::SliderFloat("Height Scale", &height_scale, -1.0f, 1.0f);
    }

    glm::vec3 ambient = glm::vec3(0.5f);
    glm::vec3 diffuse = glm::vec3(0.5f);
    glm::vec3 specular = glm::vec3(0.5f);
    float shininess = 32.0f;
    float height_scale = 1.0f;
};

#endif // TEXTURED_MATERIAL_H