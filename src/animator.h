#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "resourceManager.h"

class Animator{

public:
    Animator(Entity* target, float speed){
        this->target = target;
        this->speed = speed;
    }

    void update(){
        if(isPlaying){
            float deltaTime = ResourceManager::getDeltaTime();
            currentFrame += deltaTime * speed;

            if(currentFrame > controlPoints.size() - 1){
                if(isLoop){
                    currentFrame = 0;
                } else{
                    currentFrame = controlPoints.size() - 1;
                }
            }

            if(controlPoints.size() > 1){
                if(isCubic){
                    updateCubic(currentFrame);
                } else{
                    updateLinear(currentFrame);
                }
            } else{
                target->setPosition(controlPoints[0]->getWorldPosition());
            }

        } else{
            if(currentFrame != 0){
                currentFrame = 0;
                if(controlPoints.size() > 1){
                    target->setPosition(controlPoints[0]->getWorldPosition());
                }
            }
        }
        


    }

    void updateLinear(float deltaTime){
        if(isPlaying){
            int pointAIndex = (int)currentFrame;
            int pointBIndex = (int)currentFrame + 1;
            if(pointBIndex > controlPoints.size() - 1){
                pointBIndex = 0;
            }
            glm::vec3 pointA = controlPoints[pointAIndex]->getWorldPosition();
            glm::vec3 pointB = controlPoints[pointBIndex]->getWorldPosition();
            glm::vec3 newPosition = glm::mix(pointA, pointB, currentFrame - int(currentFrame));
            target->setPosition(newPosition);
        }
    }

    void updateCubic(float deltaTime){
        if(isPlaying){
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
                newPosition = interpolateCubic(pointA, pointB, pointC, pointD, currentFrame - int(currentFrame));
            }

            target->setPosition(newPosition);
        }
        else{
            currentFrame = 0;
        }
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

    void OnGui() {
        ImGui::SliderFloat("Speed", &speed, 0.0f, 10.0f);
        if (ImGui::Button("Reset")) {
            currentFrame = 0;
            update();
            isPlaying = false;
        }

        if (ImGui::Button("Play")) {
            isPlaying = true;
        }

        if (ImGui::Button("Stop")) {
            isPlaying = false;
        }

        if(isPlaying){
            ImGui::Text("Current Frame: %f", currentFrame);
        }

        ImGui::Checkbox("Loop", &isLoop);
        ImGui::Checkbox("Cubic", &isCubic);

        ImGui::Text("Control Points------------------");

        for (int i = 0; i < controlPoints.size(); i++) {
            ImGui::Text("\nControl Point %d", i);
            ImGui::PushID(i); //prevents name collision
            controlPoints[i]->OnGui();
            ImGui::PopID();
        }
    }


private:
    Entity* target;
    std::vector<Entity*> controlPoints;
    bool isPlaying = false;
    bool isLoop = true;
    bool isCubic = false;
    float speed = 1.0f;
    float currentFrame = 0.0f;
    

};

#endif // ANIMATOR_H