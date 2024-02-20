#ifndef ANIMATOR_H
#define ANIMATOR_H

class Animator{

#include "gameobject.h"

public:
    Animator(Entity* target, float speed){
        this->target = target;
        this->speed = speed;
    }

    void update(float deltaTime){
        if(isPlaying){
            currentFrame += deltaTime * speed;
            if(currentFrame > controlPoints.size() - 1){
                currentFrame = 0;
            }
            int pointAIndex = (int)currentFrame;
            int pointBIndex = (int)currentFrame + 1;
            if(pointBIndex > controlPoints.size() - 1){
                pointBIndex = 0;
            }
            glm::vec3 pointA = controlPoints[pointAIndex]->getWorldPosition();
            glm::vec3 pointB = controlPoints[pointBIndex]->getWorldPosition();
            glm::vec3 newPosition = interpolate(pointA, pointB, currentFrame);
            target->setPosition(newPosition);
        }
    }

    void updateCubic(float deltaTime){
        if(isPlaying){
            currentFrame += deltaTime * speed;
            currentFrame = glm::clamp(currentFrame, 0.0f, float(controlPoints.size() - 1));

            int pointAIndex = std::max(0, (int)currentFrame - 1);
            int pointBIndex = (int)currentFrame;
            int pointCIndex = std::min((int)controlPoints.size() - 1, (int)currentFrame + 1);
            int pointDIndex = std::min((int)controlPoints.size() - 1, (int)currentFrame + 2);

            glm::vec3 pointA = controlPoints[pointAIndex]->getWorldPosition();
            glm::vec3 pointB = controlPoints[pointBIndex]->getWorldPosition();
            glm::vec3 pointC = controlPoints[pointCIndex]->getWorldPosition();
            glm::vec3 pointD = controlPoints[pointDIndex]->getWorldPosition();

            glm::vec3 newPosition;

            if (controlPoints.size() == 2) {
                float t = currentFrame - pointAIndex;
                newPosition = glm::mix(pointA, pointB, t);
            } else {
                newPosition = interpolateCubic(pointA, pointB, pointC, pointD, currentFrame);
            }

            target->setPosition(newPosition);
        }
    }



    glm::vec3 interpolate(glm::vec3 pointA, glm::vec3 pointB, float time){
        glm::vec3 result = pointA + (pointB - pointA) * time;
        return result;
    }

glm::vec3 interpolateCubic(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC, glm::vec3 pointD, float time){
    float t2 = time * time;
    float t3 = t2 * time;

    glm::vec3 result = 0.5f * ((2.0f * pointB) +
        (-pointA + pointC) * time +
        (2.0f * pointA - 5.0f * pointB + 4.0f * pointC - pointD) * t2 +
        (-pointA + 3.0f * pointB - 3.0f * pointC + pointD) * t3);

    return result;
}


    int getControlPointCount(){
        return controlPoints.size();
    }

    void addControlPoint(Entity* controlPoint){
        controlPoints.push_back(controlPoint);
    }

    void OnGui(){
        ImGui::SliderFloat("Speed", &speed, 0.0f, 1.0f);
        if(ImGui::Button("Reset")){
            currentFrame = 0;
            isPlaying = false;
        }

        if(ImGui::Button("Play")){
            isPlaying = true;
        }

        if(ImGui::Button("Stop")){
            isPlaying = false;
        }

        // ImGui::Text("Control Points------------------");

        // for(int i = 0; i < controlPoints.size(); i++){
        //     ImGui::Text("Control Point %d", i);
        //     controlPoints[i]->OnGui();
        // }
    }

private:
    Entity* target;
    std::vector<Entity*> controlPoints;
    bool isPlaying = false;
    float speed = 1.0f;
    float currentFrame = 0.0f;
    

};

#endif // ANIMATOR_H