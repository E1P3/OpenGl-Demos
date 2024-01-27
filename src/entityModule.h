#ifndef ENTITY_MODULE_H
#define ENTITY_MODULE_H

class GameObject;

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

    void setParent(GameObject* parent) {
        this->parent = parent;
    }

    GameObject* getParent() {
        return parent;
    }

private:
    GameObject* parent;
};

#endif // ENTITY_MODULE_H
