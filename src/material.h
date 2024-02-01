#ifndef MATERIAL_H
#define MATERIAL_H

#include "shader.h"

class Material {
public:
    
    Material() {
        this->ID = 0;
    }

    virtual void Draw(Shader* shader) = 0;

    virtual void OnGui() = 0;

    unsigned int getID() {
        return ID;
    }

    void setID(unsigned int ID) {
        this->ID = ID;
    }

    void Draw(Shader* shader) {
        shader->SetFloat("material.ambient", m_ambient, true);
        shader->SetFloat("material.diffuse", m_diffuse, true);
        shader->SetFloat("material.specular", m_specular, true);
    }

    void Draw(Shader* shader) {
        shader->SetFloat("material.ambient", m_ambient, true);
        shader->SetFloat("material.diffuse", m_diffuse, true);
        shader->SetFloat("material.specular", m_specular, true);
    }

private:
    unsigned int ID;
};

#endif // MATERIAL_H
