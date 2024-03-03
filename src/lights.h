#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include "entity.h"
#include <iostream>
#include "shader.h"
#include <string>

enum LightType{
    DIRECTIONAL_LIGHT,
    POINT_LIGHT,
    NOT_SET
};


//Directional light
class Light : public Entity{
public:
    // Constructor
    Light() = default;

    Light(float strength){
        this->strength = strength;
    }

    Light(float strength, glm::vec3 position, glm::quat rotation){
        this->strength = strength;
        this->Translate(position);
        this->Rotate(rotation);
    }

    // Destructor
    ~Light() = default;

    // Other member functions
    void turnOn() {isOn = true;}
    void turnOff(){isOn = false;}
    bool isLightOn(){return isOn;}
    void setStrength(float strength){this->strength = strength;}
    float getStrength(){return strength;}
    glm::vec3 getDiffuse(){return diffuse;}
    glm::vec3 getAmbient(){return ambient;}
    glm::vec3 getSpecular(){return specular;}
    void setDiffuse(glm::vec3 diffuse){this->diffuse = diffuse;}
    void setAmbient(glm::vec3 ambient){this->ambient = ambient;}
    void setSpecular(glm::vec3 specular){this->specular = specular;}
    void setType(LightType type){this->type = type;}
    LightType getType(){return type;}
    int getIndex(){return index;}
    void setIndex(int index){this->index = index;}

private:
    bool isOn;
    int index;
    LightType type = NOT_SET;
    float strength = 1.0f;
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 ambient = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
};

#endif // LIGHT_H

#ifndef DIR_LIGHT_H
#define DIR_LIGHT_H

class DirectionalLight : public Light{
public:
    // Constructor
    DirectionalLight(){this->setType(LightType::DIRECTIONAL_LIGHT);}

    DirectionalLight(float strength, glm::vec3 rotation, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular){
        this->setStrength(strength);
        this->setRotation(rotation);
        this->setAmbient(ambient);
        this->setDiffuse(diffuse);
        this->setSpecular(specular);
        this->setType(LightType::DIRECTIONAL_LIGHT);
    }

    // Destructor
    ~DirectionalLight() = default;

private:
    glm::mat4 lightViewMatrix;
    glm::mat4 lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, -60.0f, 60.f);


};

#endif // DIR_LIGHT_H

#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

class PointLight : public Light{
public:
    // Constructor
    PointLight(){this->setType(LightType::POINT_LIGHT);}

    PointLight(float strength, glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 ambient = glm::vec3(0.5f,0.5f,0.5f) , glm::vec3 diffuse = glm::vec3(0.5f,0.5f,0.5f), glm::vec3 specular = glm::vec3(0.5f,0.5f,0.5f)){
        this->setStrength(strength);
        this->setPosition(position);
        this->setAmbient(ambient);
        this->setDiffuse(diffuse);
        this->setSpecular(specular);
        this->constant = constant;
        this->linear = linear;
        this->quadratic = quadratic;
        this->setType(LightType::POINT_LIGHT);
    }

    // Destructor
    ~PointLight() = default;

    // Other member functions
    void setConstant(float constant){this->constant = constant;}
    void setLinear(float linear){this->linear = linear;}
    void setQuadratic(float quadratic){this->quadratic = quadratic;}
    float getConstant(){return constant;}
    float getLinear(){return linear;}
    float getQuadratic(){return quadratic;}
    void OnGui(){
        Entity::OnGui();
        ImGui::SliderFloat("Constant", &constant, 0.0f, 1.0f);
        ImGui::SliderFloat("Linear", &linear, 0.0f, 1.0f);
        ImGui::SliderFloat("Quadratic", &quadratic, 0.0f, 1.0f);
    }
private:
	float constant = 1.0f;
	float linear = 0.22f;
	float quadratic = 0.20f;
    
};


#endif // POINT_LIGHT_H

