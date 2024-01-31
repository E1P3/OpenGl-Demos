#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Entity {
public:
    Entity() : position(glm::vec3(0.0f)), rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), scale(glm::vec3(1.0f)) {
        updateTransform();
    }

    ~Entity() = default;

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

private:
    glm::mat4 transform;
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    void updateTransform() {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
        transform = translationMatrix * rotationMatrix * scaleMatrix;
    }
};

#endif // ENTITY_H
