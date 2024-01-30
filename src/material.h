#ifndef MATERIAL_H
#define MATERIAL_H

#include "shader.h"

class Material {
public:

    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) {
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->shininess = shininess;
    }
    // Getters
    glm::vec3 getAmbient() const { return this->ambient; }
    glm::vec3 getDiffuse() const { return this->diffuse; }
    glm::vec3 getSpecular() const { return this->specular; }
    float getShininess() const { return this->shininess; }

    // Setters
    void setAmbient(glm::vec3 ambient) { this->ambient = ambient; }
    void setDiffuse(glm::vec3 diffuse) { this->diffuse = diffuse; }
    void setSpecular(glm::vec3 specular) { this->specular = specular; }
    void setShininess(float shininess) { this->shininess = shininess; }

    void Draw(Shader* shader) {
        shader->SetVector3f("material.ambient", this->ambient);
        shader->SetVector3f("material.diffuse", this->diffuse);
        shader->SetVector3f("material.specular", this->specular);
        shader->SetFloat("material.shininess", this->shininess);
    }

    unsigned int getID() {
        return ID;
    }

    void setID(unsigned int ID) {
        this->ID = ID;
    }

private:
    unsigned int ID;
    glm::vec3 ambient = glm::vec3(0.5f);
    glm::vec3 diffuse = glm::vec3(0.5f);
    glm::vec3 specular = glm::vec3(0.5f);
    float shininess = 32.0f;
};

#endif // MATERIAL_H
