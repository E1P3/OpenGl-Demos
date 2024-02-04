#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "entity.h"
#include "entityModule.h"
#include <vector>
#include <string>

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

    unsigned int getID() {
        return ID;
    }

    void setID(unsigned int ID) {
        this->ID = ID;
    }

    void addChild(GameObject* child) {
        children.push_back(child);
        child->parent = this;
        this->addChildEntity(child);
    }

    void removeChild(GameObject* child) {
        for (int i = 0; i < children.size(); i++) {
            if (children[i] == child) {
                children.erase(children.begin() + i);
                child->parent = nullptr;
                this->removeChildEntity(child);
                break;
            }
        }
    }

    void addModule(EntityModule* module) {
        modules.push_back(module);
        module->setParent(this);
    }

    std::string getName() {
        return name;
    }

    void setName(std::string name) {
        this->name = name;
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
    
private:
    std::string name;
    unsigned int ID;
    std::vector<GameObject*> children;
    GameObject* parent;
    std::vector<EntityModule*> modules;
};

#endif // GAMEOBJECT_H
