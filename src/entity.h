#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

class Entity {
public:
    Entity() : position(glm::vec3(0.0f)), rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), scale(glm::vec3(1.0f)), entityParent(nullptr) {
        updateTransform();
    }

    ~Entity() {
        for (Entity* child : entityChildren) {
            delete child;
        }
        this->entityParent->removeChildEntity(this);
    }

    glm::mat4 getTransform() const {
        return transform;
    }

    glm::vec3 getPosition() const {
        return position;
    }

    glm::quat getRotation() const {
        return rotation;
    }

    glm::vec3 getScale() const {
        return scale;
    }

    void setPosition(const glm::vec3& newPosition) {
        position = newPosition;
        updateTransform();
    }

    void setRotation(const glm::quat& newRotation) {
        rotation = glm::normalize(newRotation);
        updateTransform();
    }

    void setRotation(const glm::vec3& rotationEuler) {
        rotation = glm::normalize(glm::quat(glm::radians(rotationEuler)));
        updateTransform();
    }

    void setScale(const glm::vec3& newScale) {
        scale = newScale;
        updateTransform();
    }

    void Translate(const glm::vec3& translation) {
        position += translation;
        updateTransform();
    }

    void Rotate(const glm::quat& deltaRotation) {
        rotation = glm::normalize(rotation * deltaRotation);
        updateTransform();
    }

    void Rotate(const glm::vec3& rotationEuler) {
        glm::quat deltaRotation = glm::quat(glm::radians(rotationEuler));
        rotation = glm::normalize(rotation * deltaRotation);
        updateTransform();
    }

    void Scale(const glm::vec3& deltaScale) {
        scale *= deltaScale;
        updateTransform();
    }

    glm::vec3 getForward() const {
        return glm::normalize(rotation * glm::vec3(0.0f, 0.0f, 1.0f));
    }

    glm::vec3 getRight() const {
        return glm::normalize(rotation * glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 getUp() const {
        return glm::normalize(rotation * glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void addChildEntity(Entity* child) {
        if (entityParent) {
            entityParent->removeChildEntity(this);
        }
        child->entityParent = this;
        entityChildren.push_back(child);
        updateTransform();
    }

    void removeChildEntity(Entity* child) {
        auto it = std::find(entityChildren.begin(), entityChildren.end(), child);
        if (it != entityChildren.end()) {
            entityChildren.erase(it);
            child->entityParent = nullptr;
            child->updateTransform();
        }
    }

private:
    glm::mat4 transform;
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
    Entity* entityParent;
    std::vector<Entity*> entityChildren; 

    void updateTransform() {
        glm::mat4 localTransform = calculateLocalTransform();
        if (entityParent) {
            transform = entityParent->getTransform() * localTransform;
        } else {
            transform = localTransform;
        }

        for (Entity* child : entityChildren) {
            child->updateTransform();
        }
    }

    glm::mat4 calculateLocalTransform() const {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
        return translationMatrix * rotationMatrix * scaleMatrix;
    }
};

#endif // ENTITY_H
