#ifndef LIGHTS_H
#define LIGHTS_H

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
    glm::vec3 diffuse = glm::vec3(0.5f);
	glm::vec3 ambient = glm::vec3(0.01f);
	glm::vec3 specular = glm::vec3(0.22f);
};

class DirectionalLight : public Light{
public:
    // Constructor
    DirectionalLight(){this->setType(LightType::DIRECTIONAL_LIGHT);}

    DirectionalLight(float strength, glm::vec3 position, glm::quat rotation){
        this->setStrength(strength);
        this->setPosition(position);
        this->setRotation(rotation);
        this->setType(LightType::DIRECTIONAL_LIGHT);
    }

    // Destructor
    ~DirectionalLight() = default;

private:
    glm::mat4 lightViewMatrix;
    glm::mat4 lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, -60.0f, 60.f);


};

class PointLight : public Light{
public:
    // Constructor
    PointLight(){this->setType(LightType::POINT_LIGHT);}

    PointLight(float strength, glm::vec3 position, float constant, float linear, float quadratic){
        this->setStrength(strength);
        this->setPosition(position);
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

private:
	float constant = 1.0f;
	float linear = 0.22f;
	float quadratic = 0.20f;
    
};


#endif // LIGHTS_H

