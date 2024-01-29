#ifndef ENTITY_MODULE_H
#define ENTITY_MODULE_H

class GameObject;

class EntityModule
{
public:
    EntityModule() = default;

    ~EntityModule() = default;

    void setParent(GameObject* parent) {
        this->parent = parent;
    }

    GameObject* getParent() {
        return parent;
    }

    unsigned int getID() {
        return ID;
    }

    void setID(unsigned int ID) {
        this->ID = ID;
    }

    virtual void OnRenderPass() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnStart() = 0;

private:
    unsigned int ID;
    GameObject* parent;
};

#endif // ENTITY_MODULE_H
