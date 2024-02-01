#ifndef TOON_MATERIAL_H
#define TOON_MATERIAL_H

#include "../material.h"
#include "imgui.h"

class ToonMaterial : public Material {
public:
    
    ToonMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, int numOfBands, float outlineThickness) {
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->shininess = shininess;
        this->numOfBands = numOfBands;
        this->outlineThickness = outlineThickness;
    }

    // Getters
    glm::vec3 getAmbient() const { return this->ambient; }
    glm::vec3 getDiffuse() const { return this->diffuse; }
    glm::vec3 getSpecular() const { return this->specular; }
    float getShininess() const { return this->shininess; }
    int getNumOfBands() const { return this->numOfBands; }
    float getOutlineTreshold() const { return this->outlineThickness; }

    // Setters
    void setAmbient(glm::vec3 ambient) { this->ambient = ambient; }
    void setDiffuse(glm::vec3 diffuse) { this->diffuse = diffuse; }
    void setSpecular(glm::vec3 specular) { this->specular = specular; }
    void setShininess(float shininess) { this->shininess = shininess; }
    void setNumOfBands(int numOfBands) { this->numOfBands = numOfBands; }
    void setOutlineTreshold(float outlineThickness) { this->outlineThickness = outlineThickness; }

    void Draw(Shader* shader) override {
        shader->SetVector3f("material.ambient", this->ambient);
        shader->SetVector3f("material.diffuse", this->diffuse);
        shader->SetVector3f("material.specular", this->specular);
        shader->SetFloat("material.shininess", this->shininess);
        shader->SetInteger("numOfBands", this->numOfBands);
        shader->SetFloat("outlineThickness", this->outlineThickness);
    }

    void OnGui() override {
        ImGui::ColorEdit3("Ambient", (float*)&ambient);
        ImGui::ColorEdit3("Diffuse", (float*)&diffuse);
        ImGui::ColorEdit3("Specular", (float*)&specular);
        ImGui::SliderFloat("Shininess", &shininess, 0.0f, 10.0f);
        ImGui::SliderInt("Num of Bands", &numOfBands, 1, 25);
        ImGui::SliderFloat("Outline Treshold", &outlineThickness, 0.0f, 1.0f);
    }

    glm::vec3 ambient = glm::vec3(0.5f);
    glm::vec3 diffuse = glm::vec3(0.5f);
    glm::vec3 specular = glm::vec3(0.5f);
    int numOfBands = 4;
    float shininess = 1.0f;
    float outlineThickness = 0.5f;

};

#endif // TOON_MATERIAL_H