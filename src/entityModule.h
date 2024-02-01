#ifndef ENTITY_MODULE_H
#define ENTITY_MODULE_H

class GameObject;

<<<<<<< HEAD
class EntityModule
{
public:
    EntityModule() = default;

    ~EntityModule() = default;
=======
class IRenderable {
public:
    virtual void OnRenderPass() = 0;
};

class IUpdate {
public:
    virtual void OnUpdate() = 0;
};

class IStart {
public:
    virtual void OnStart() = 0;
};

class entityModule : public IRenderable, public IUpdate, public IStart
{
public:
    entityModule() = default;

    ~entityModule() = default;
>>>>>>> 951bcb1f6a5306fddea8d34d5400f199987be5e5

    void setParent(GameObject* parent) {
        this->parent = parent;
    }

    GameObject* getParent() {
        return parent;
    }

<<<<<<< HEAD
    unsigned int getID() {
        return ID;
    }

    void setID(unsigned int ID) {
        this->ID = ID;
    }

    virtual void OnUpdate() = 0;
    virtual void OnStart() = 0;

private:
    unsigned int ID;
=======
private:
>>>>>>> 951bcb1f6a5306fddea8d34d5400f199987be5e5
    GameObject* parent;
};

#endif // ENTITY_MODULE_H
