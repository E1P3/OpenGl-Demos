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

    void setTransform(glm::mat4 transform) {
        this->transform = transform;
    }

    void translate(glm::vec3 translation) {
        transform = glm::translate(transform, translation);
    }

    void rotate(glm::quat rotation) {
        transform = glm::mat4_cast(rotation) * transform;
    }

    void scale(glm::vec3 scale) {
        transform = glm::scale(transform, scale);
    }

private:
    
    glm::mat4 transform;

};

#endif // ENTITY_H
