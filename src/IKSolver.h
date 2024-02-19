#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Entity.h" // Assuming Entity.h is included in the same directory

class IKSolver {
public:
    IKSolver(Entity* root, Entity* goal, Entity* offsetEntity){
        this->root = root;
        this->goal = goal;
        this->offsetEntity = offsetEntity;
    }

    void solveIK() {

        storeInitialTransforms();
        calculateJointLengths();

        glm::vec3 finalTarget = (goal->getWorldPosition() - offsetEntity->getWorldPosition()) / offsetEntity->getScale();

        float distance = glm::length(finalTarget - root->getWorldPosition());
        float totalLength = 0.0f;
        for (float length : jointLengths) {
            totalLength += length;
        }
        if(distance > totalLength){
            setRotationToTargetRecursive(root, finalTarget);
            return;
        }

        setRotationToTargetRecursive(root, finalTarget);
    }

    void setRotationToTargetRecursive(Entity* entity, glm::vec3 target) {
        glm::vec3 currentPosition = entity->getWorldPosition();

        glm::vec3 direction = glm::normalize(target - currentPosition);

        glm::quat newRotation = glm::rotation(glm::vec3(0.0f, 1.0f, 0.0f), direction);
        entity->setWorldRotation(newRotation);
        for (Entity* child : entity->getChildren()) {
            setRotationToTargetRecursive(child, target);
        }
    }

    void storeInitialTransforms() {
        initialPosition.clear();
        initialRotation.clear();

        storeInitialTransformsRecursive(root); 
    }

    void storeInitialTransformsRecursive(Entity* entity) {
        initialPosition.push_back(entity->getWorldPosition());
        initialRotation.push_back(entity->getWorldRotation());

        std::vector<Entity*> children = entity->getChildren();

        for (Entity* child : children) {
            storeInitialTransformsRecursive(child);
        }
    }

    void calculateJointLengths() {
        jointLengths.clear();

        calculateJointLengthsRecursive(root);
    }

    void calculateJointLengthsRecursive(Entity* entity) {
        for (Entity* child : entity->getChildren()) {
            jointLengths.push_back(glm::length(child->getPosition() - entity->getPosition()));
            calculateJointLengthsRecursive(child);
        }
    }

private:
    void printInitialTransforms() {
        for (int i = 0; i < initialPosition.size(); i++) {
            std::cout << "Initial Position: " << initialPosition[i].x << " " << initialPosition[i].y << " " << initialPosition[i].z << std::endl;
            std::cout << "Initial Rotation: " << initialRotation[i].x << " " << initialRotation[i].y << " " << initialRotation[i].z << " " << initialRotation[i].w << std::endl;
        }
    }

    Entity* root;
    Entity* goal;
    Entity* offsetEntity; // entity of the game object bound to a skeleton
    std::vector<glm::vec3> initialPosition;
    std::vector<glm::quat> initialRotation;

    std::vector<glm::vec3> currentPosition;
    std::vector<glm::quat> currentRotation;

    std::vector<float> jointLengths;

};