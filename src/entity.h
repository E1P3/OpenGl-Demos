#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Entity {
public:
    Entity() {
        transform = glm::mat4(1.0f);
    }

    ~Entity() = default;

    glm::mat4 getTransform() {
        return transform;
    }

    glm::vec3 getPosition() {
        return glm::vec3(transform[3][0], transform[3][1], transform[3][2]);
    }

    glm::quat getRotation() {
        return glm::quat_cast(transform);
    }

    glm::vec3 getScale() {
        return glm::vec3(transform[0][0], transform[1][1], transform[2][2]);
    }

    void setTransform(glm::mat4 transform) {
        this->transform = transform;
    }

    void setPosition(glm::vec3 position) {
        transform[3][0] = position.x;
        transform[3][1] = position.y;
        transform[3][2] = position.z;
    }

    void setRotation(glm::quat rotation) {
        rotation = glm::normalize(rotation);
        glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
        this->transform = rotationMatrix * transform;
    }

    void setScale(glm::vec3 scale) {
        transform[0][0] = scale.x;
        transform[1][1] = scale.y;
        transform[2][2] = scale.z;
    }
    
    void translate(glm::vec3 translation) {
        transform = glm::translate(transform, translation);
    }

    void rotate(glm::quat rotation) {
        rotation = glm::normalize(rotation);
        glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
        this->transform = transform * rotationMatrix;
    }

    void scale(glm::vec3 scale) {
        transform = glm::scale(transform, scale);
    }

private:
    
    glm::mat4 transform;

};

#endif // ENTITY_H
