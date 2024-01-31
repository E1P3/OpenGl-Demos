#include "../material.h"
#include "imgui.h"

class PBRMaterial : public Material {
public:
    
    PBRMaterial(glm::vec3 albedo, float metallic, float roughness, float ao) {
        this->albedo = albedo;
        this->metallic = metallic;
        this->roughness = roughness;
        this->ao = ao;
    }

    // Getters
    glm::vec3 getAlbedo() const { return this->albedo; }
    float getMetallic() const { return this->metallic; }
    float getRoughness() const { return this->roughness; }
    float getAO() const { return this->ao; }

    // Setters
    void setAlbedo(glm::vec3 albedo) { this->albedo = albedo; }
    void setMetallic(float metallic) { this->metallic = metallic; }
    void setRoughness(float roughness) { this->roughness = roughness; }
    void setAO(float ao) { this->ao = ao; }
    
    void Draw(Shader* shader) override {
        shader->SetVector3f("albedo", this->albedo);
        shader->SetFloat("metallic", this->metallic);
        shader->SetFloat("roughness", this->roughness);
        shader->SetFloat("ao", this->ao);
    }

    void OnGui() override {
        ImGui::ColorEdit3("Albedo", (float*)&albedo);
        ImGui::SliderFloat("Metallic", &metallic, 0.0f, 1.0f);
        ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f);
        ImGui::SliderFloat("AO", &ao, 0.0f, 1.0f);
    }

    glm::vec3 albedo;
    float metallic;
    float roughness;
    float ao;
};
