#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Entity {
public:
    Entity() : transform(glm::mat4(1.0f)), position(glm::vec3(0.0f)), rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), scale(glm::vec3(1.0f)) {}

    ~Entity() = default;

    glm::mat4 getTransform() {
        // Reconstruct the transformation matrix from position, rotation, and scale
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
        transform = translationMatrix * rotationMatrix * scaleMatrix;
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

    void setTransform(const glm::mat4& newTransform) {
        transform = newTransform;

        // Extract position
        position = glm::vec3(transform[3]);

        // Extract scale
        scale.x = glm::length(glm::vec3(transform[0]));
        scale.y = glm::length(glm::vec3(transform[1]));
        scale.z = glm::length(glm::vec3(transform[2]));

        // Normalize scale from the matrix
        glm::mat4 normTransform = transform;
        normTransform[0] /= scale.x;
        normTransform[1] /= scale.y;
        normTransform[2] /= scale.z;

        // Extract rotation quaternion from the normalized transformation matrix
        rotation = glm::quat_cast(normTransform);
    }


    void setPosition(const glm::vec3& newPosition) {
        position = newPosition;
    }

    void setRotation(const glm::quat& newRotation) {
        rotation = glm::normalize(newRotation);
    }

    void setScale(const glm::vec3& newScale) {
        scale = newScale;
    }

    void Translate(const glm::vec3& translation) {
        position += translation;
    }

    void Rotate(const glm::quat& deltaRotation) {
        rotation = glm::normalize(rotation * deltaRotation);
    }

    void Scale(const glm::vec3& deltaScale) {
        scale *= deltaScale;
    }

    void testRotation() {
        glm::quat testRotation;
        glm::vec3 eulerAngle(30.0f, 30.0f, 30.0f);
        testRotation = glm::quat(glm::radians(eulerAngle));

        std::cout << "Before Rotation:" << std::endl;
        printRotationInfo();

        Rotate(testRotation);

        std::cout << "After Rotation:" << std::endl;
        printRotationInfo();
    }

private:
    glm::mat4 transform;
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    void printRotationInfo() {
        glm::vec3 eulerAngle = glm::degrees(glm::eulerAngles(rotation));
        std::cout << "Euler Angle: (" << eulerAngle.x << ", " << eulerAngle.y << ", " << eulerAngle.z << ")" << std::endl;
        std::cout << "Quaternion: (" << rotation.x << ", " << rotation.y << ", " << rotation.z << ", " << rotation.w << ")" << std::endl;
    }
};

#endif // ENTITY_H
