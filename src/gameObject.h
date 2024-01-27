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

    void addChild(GameObject* child) {
        children.push_back(child);
        child->parent = this;
    }

    void addModule(entityModule* module) {
        modules.push_back(module);
        module->setParent(this);
    }

    void OnRenderPass() {
        for (auto module : modules) {
            module->OnRenderPass();
        }
        for (auto child : children) {
            child->OnRenderPass();
        }
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
    std::vector<GameObject*> children;
    GameObject* parent;
    std::vector<entityModule*> modules;
};

#endif // GAMEOBJECT_H
