#ifndef IK_SOLVER_H
#define IK_SOLVER_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Entity.h" // Assuming Entity.h is included in the same directory

class IKSolver {
public:
    IKSolver(Entity* root, Entity* offsetEntity = nullptr){
        this->root = root;
        this->offsetEntity = offsetEntity;
        extractJointChain();
    }

    void solveIK(Entity* goal) {

        Initialize();

        glm::vec3 finalTarget;

        if(offsetEntity){
            finalTarget = (goal->getWorldPosition() - offsetEntity->getWorldPosition()) / offsetEntity->getScale();
        } else {
            finalTarget = goal->getWorldPosition();
        }

        float distance = glm::length(finalTarget - root->getWorldPosition());
        float totalLength = 0.0f;
        for (float length : jointLengths) {
            totalLength += length;
        }
        if(distance > totalLength){
            setRotationToTargetRecursive(root, finalTarget);
            return;
        }

        solveFABRIK(finalTarget);

    }

    void solveFABRIK(glm::vec3 target){
        int currentIteration = this->iterations;
        while(currentIteration > 0){
            backward(target);
            forward(target);
            currentIteration--;
        }
        setFinalRotations();
    }

    void backward(glm::vec3 target){
        currentPosition.back() = target;
        for(int i = jointChain.size() - 2; i > 0; i--){ // dont shift root and the end effector
            currentPosition[i] = shiftAlongLine(currentPosition[i + 1], glm::normalize(currentPosition[i] - currentPosition[i + 1]), jointLengths[i]);
        }
    }

    void forward(glm::vec3 target){
        currentPosition[0] = root->getWorldPosition();
        for(int i = 0; i < jointChain.size() - 2; i++){
            currentPosition[i+1] = shiftAlongLine(currentPosition[i], glm::normalize(currentPosition[i + 1] - currentPosition[i]), jointLengths[i]);
        }
    }

    void setFinalRotations(){
        for(int i = 1; i < jointChain.size() - 1; i++){
            glm::vec3 direction = glm::normalize(currentPosition[i] - currentPosition[i-1]);
            setRotationToDirection(jointChain[i-1], direction);
        }
    }

    glm::vec3 shiftAlongLine(glm::vec3 initialPosition, glm::vec3 lineVector, float jointLength){
        return initialPosition + lineVector * jointLength;
    }

    void setRotationToTargetRecursive(Entity* entity, glm::vec3 target) {
        glm::vec3 currentPosition = entity->getWorldPosition();

        glm::vec3 direction = glm::normalize(target - currentPosition);

        glm::quat newRotation = glm::rotation(glm::vec3(0.0f, 1.0f, 0.0f), direction);
        entity->setWorldRotation(newRotation);
        if (entity->getChildren().size() == 1) {
            setRotationToTargetRecursive(entity->getChildren()[0], target);
        }
    }

    void setRotationToDirection(Entity* entity, glm::vec3 direction) {
        glm::quat newRotation = glm::rotation(glm::vec3(0.0f, 1.0f, 0.0f), direction);
        entity->setWorldRotation(newRotation);
    }

    void Initialize() {
        storeInitialTransforms();
        calculateJointLengths();
        currentPosition = initialPosition;
        currentRotation = initialRotation;
    }

    void extractJointChain() {
        jointChain.clear();
        Entity* currentEntity = root;
        while(currentEntity->getChildren().size()){
            jointChain.push_back(currentEntity);
            currentEntity = currentEntity->getChildren()[0];
            if(currentEntity->getChildren().size() > 1){
                jointChain.push_back(currentEntity);
                currentEntity = currentEntity->getChildren()[1];
                if(endOnMultiple){
                    break;
                }
            }
        }
        jointChain.push_back(currentEntity);
    }

    void storeInitialTransforms() {
        initialPosition.clear();
        initialRotation.clear();

        for(Entity* entity : jointChain){
            initialPosition.push_back(entity->getWorldPosition());
            initialRotation.push_back(entity->getWorldRotation());
        }
    }

    void calculateJointLengths() {
        jointLengths.clear();
        for(int i = 0;  i < jointChain.size() - 1; i++){
            jointLengths.push_back(glm::length(jointChain[i + 1]->getWorldPosition() - jointChain[i]->getWorldPosition()));
        }
    }

    std::vector<glm::vec3> getCurrentPosition() {
        return currentPosition;
    }

    void OnGui() {
        ImGui::Text("IK Solver");
        ImGui::SliderFloat("Error", &error, 0.0f, 1.0f);
        ImGui::SliderInt("Iterations", &iterations, 1, 1000);
        if(ImGui::Checkbox("End on Multiple", &endOnMultiple)){
            extractJointChain();
        }
    }

private:
    void printInitialTransforms() {
        for (int i = 0; i < initialPosition.size(); i++) {
            std::cout << "Initial Position: " << initialPosition[i].x << " " << initialPosition[i].y << " " << initialPosition[i].z << std::endl;
            std::cout << "Initial Rotation: " << initialRotation[i].x << " " << initialRotation[i].y << " " << initialRotation[i].z << " " << initialRotation[i].w << std::endl;
        }
    }

    float error = 0.01f;
    int iterations = 100;
    bool endOnMultiple = true;
    Entity* root;
    Entity* offsetEntity; // entity of the game object bound to a skeleton
    
    std::vector<glm::vec3> initialPosition;
    std::vector<glm::quat> initialRotation;

    std::vector<glm::vec3> currentPosition;
    std::vector<glm::quat> currentRotation;

    std::vector<Entity*> jointChain;

    std::vector<float> jointLengths;

};

#endif // IK_SOLVER_H