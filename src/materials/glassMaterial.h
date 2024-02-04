#ifndef GLASS_MATERIAL_H
#define GLASS_MATERIAL_H

#include "../material.h"
#include "imgui.h"

class GlassMaterial : public Material {
public:

    GlassMaterial(float eta, float fresnelPower, float chromaticAberrationFactor) {
        this->eta = eta;
        this->fresnelPower = fresnelPower;
        this->chromaticAberrationFactor = chromaticAberrationFactor;
    }

    // Getters
    float getEta() const { return this->eta; }
    float getFresnelPower() const { return this->fresnelPower; }
    float getChromaticAberrationFactor() const { return this->chromaticAberrationFactor; }

    // Setters  
    void setEta(float eta) { this->eta = eta; }
    void setFresnelPower(float fresnelPower) { this->fresnelPower = fresnelPower; }
    void setChromaticAberrationFactor(float chromaticAberrationFactor) { this->chromaticAberrationFactor = chromaticAberrationFactor; }
    
    void Draw(Shader* shader) override {
        shader->SetFloat("eta", this->eta);
        shader->SetFloat("fresnelPower", this->fresnelPower);
        shader->SetFloat("chromaticFactor", this->chromaticAberrationFactor);
    }

    void OnGui() override {
        ImGui::SliderFloat("Eta", &eta, 0.0f, 3.0f);
        ImGui::SliderFloat("Fresnel Power", &fresnelPower, 0.0f, 10.0f);
        ImGui::SliderFloat("Chromatic Factor", &chromaticAberrationFactor, 0.0f, 10.0f);
    }

private:
    float eta = 1.5f;
    float fresnelPower = 5.0f;
    float chromaticAberrationFactor = 0.01f;

};

#endif // GLASS_MATERIAL_H