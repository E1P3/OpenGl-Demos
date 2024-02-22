#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <vector>
#include <imgui.h>
#include <iostream>

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

    glm::mat4 getLocalTransform() const {
        return calculateLocalTransform();
    }

    glm::vec3 getPosition() const {
        return position;
    }

    glm::vec3 getWorldPosition() const {
        glm::mat4 worldTransform = getTransform();
        return glm::vec3(worldTransform[3][0], worldTransform[3][1], worldTransform[3][2]);
    }

    glm::quat getWorldRotation() const {
        glm::quat rotation = glm::quat_cast(getTransform());
        return rotation;
    }

    glm::quat getRotation() const {
        return rotation;
    }

    glm::vec3 getRotationEuler() const {
        return eulerRotation;
    }

    glm::vec3 getScale() const {
        return scale;
    }

    std::vector<Entity*> getChildren() const {
        return entityChildren;
    }

    void setPosition(const glm::vec3& newPosition) {
        position = newPosition;
        updateTransform();
    }

    void setWorldRotation(const glm::quat& newRotation) {
        if (entityParent) {
            glm::quat parentRotation = glm::inverse(entityParent->getWorldRotation());
            rotation = parentRotation * newRotation;
        } else {
            rotation = newRotation;
        }
        updateTransform();
    }

    void setWorldRotation(const glm::vec3& rotationEuler) {
        glm::quat newRotation = glm::quat(glm::radians(rotationEuler));
        setWorldRotation(newRotation);
    }

    void setRotation(const glm::quat& newRotation) {
        rotation = glm::normalize(newRotation);
        updateTransform();
    }

    void setRotation(const glm::vec3& rotationEuler) {
        eulerRotation = rotationEuler;
        updateTransform(rotationEuler);
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
        eulerRotation += rotationEuler;
        updateTransform(eulerRotation);
    }

    void Scale(const glm::vec3& deltaScale) {
        scale *= deltaScale;
        updateTransform();
    }

    glm::vec3 getFront() const {
        return glm::normalize(rotation * glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::vec3 getRight() const {
        return glm::normalize(rotation * glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 getUp() const {
        return glm::normalize(rotation * glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void addChildEntity(Entity* child) {
        if (child->entityParent) {
            child->entityParent->removeChildEntity(this);
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

    void OnGui() {
        ImGui::Text("\nTransform\n");
        ImGui::DragFloat3("Position", &position[0], 0.1f);

        // Convert the quaternion rotation to Euler angles for editing
        glm::vec3 rotationEuler = glm::degrees(glm::eulerAngles(rotation));
        if (ImGui::DragFloat3("Rotation", &rotationEuler[0], 1.0f, -90.0f, 90.0)) {
            // Convert the edited Euler angles back to a quaternion rotation
            rotation = glm::quat(glm::radians(rotationEuler));
            // Update the transform
            updateTransform(rotationEuler);
        }

        ImGui::DragFloat3("Scale", &scale[0], 0.1f);
        updateTransform();
    }


protected:

    virtual void overrideTransform(const glm::mat4& newTransform) {
        transform = newTransform;
    }

    virtual glm::mat4 calculateLocalTransform() const{
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
        return translationMatrix * rotationMatrix * scaleMatrix;
    }

    virtual glm::mat4 calculateLocalTransform(glm::vec3 eulerAngles) const {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotationMatrix = glm::yawPitchRoll(glm::radians(eulerAngles.y), glm::radians(eulerAngles.x), glm::radians(eulerAngles.z));
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
        return translationMatrix * rotationMatrix * scaleMatrix;
    }
    
    virtual void updateTransform() {
        glm::mat4 localTransform = calculateLocalTransform();
        eulerRotation = glm::degrees(glm::eulerAngles(rotation));
        if (entityParent) {
            transform = entityParent->getTransform() * localTransform;
        } else {
            transform = localTransform;
        }

        for (Entity* child : entityChildren) {
            child->updateTransform();
        }
    }

    virtual void updateTransform(glm::vec3 eulerAngles) {
        glm::mat4 localTransform = calculateLocalTransform(eulerAngles);
        rotation = glm::quat(glm::radians(eulerAngles));
        if (entityParent) {
            transform = entityParent->getTransform() * localTransform;
        } else {
            transform = localTransform;
        }

        for (Entity* child : entityChildren) {
            child->updateTransform();
        }
    }

private:
    glm::mat4 transform;
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 eulerRotation;
    glm::vec3 scale;
    Entity* entityParent = nullptr;
    std::vector<Entity*> entityChildren; 
};

#endif // ENTITY_H
