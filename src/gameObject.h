#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "entity.h"
#include "entityModule.h"
#include <vector>

class GameObject : public Entity {

public:
    GameObject() {
        parent = nullptr;
    }

    ~GameObject() {
        for (auto module : modules) {
            delete module;
        }
        for (auto child : children) {
            delete child;
        }
    }

<<<<<<< HEAD
    unsigned int getID() {
        return ID;
    }

    void setID(unsigned int ID) {
        this->ID = ID;
    }

=======
>>>>>>> 951bcb1f6a5306fddea8d34d5400f199987be5e5
    void addChild(GameObject* child) {
        children.push_back(child);
        child->parent = this;
    }

<<<<<<< HEAD
    void addModule(EntityModule* module) {
=======
    void addModule(entityModule* module) {
>>>>>>> 951bcb1f6a5306fddea8d34d5400f199987be5e5
        modules.push_back(module);
        module->setParent(this);
    }

<<<<<<< HEAD
    std::string getName() {
        return name;
    }

    void setName(std::string name) {
        this->name = name;
=======
    void OnRenderPass() {
        for (auto module : modules) {
            module->OnRenderPass();
        }
        for (auto child : children) {
            child->OnRenderPass();
        }
>>>>>>> 951bcb1f6a5306fddea8d34d5400f199987be5e5
    }

    void OnUpdate() {
        for (auto module : modules) {
            module->OnUpdate();
        }
        for (auto child : children) {
            child->OnUpdate();
        }
    }

    void OnStart() {
        for (auto module : modules) {
            module->OnStart();
        }
        for (auto child : children) {
            child->OnStart();
        }
    }
<<<<<<< HEAD
    
private:
    std::string name;
    unsigned int ID;
    std::vector<GameObject*> children;
    GameObject* parent;
    std::vector<EntityModule*> modules;
=======
private:
    std::vector<GameObject*> children;
    GameObject* parent;
    std::vector<entityModule*> modules;
>>>>>>> 951bcb1f6a5306fddea8d34d5400f199987be5e5
};

#endif // GAMEOBJECT_H
