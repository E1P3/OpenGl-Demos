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

            if(currentFrame > keyFrames.size() - 1){
                if(isLoop){
                    currentFrame = 0;
                } else{
                    currentFrame = keyFrames.size() - 1;
                }
            }

            if(keyFrames.size() > 1){
                if(isCubic){
                    updateCubic(currentFrame);
                } else{
                    updateLinear(currentFrame);
                }
            } else{
                target->setPosition(keyFrames[0]->getWorldPosition());
            }

        } else{
            if(currentFrame != 0){
                currentFrame = 0;
                if(keyFrames.size() > 1){
                    target->setPosition(keyFrames[0]->getWorldPosition());
                }
            }
        }
        


    }

    void updateLinear(float deltaTime){
        if(isPlaying){
            int pointAIndex = (int)currentFrame;
            int pointBIndex = (int)currentFrame + 1;
            if(pointBIndex > keyFrames.size() - 1){
                pointBIndex = 0;
            }
            glm::vec3 pointA = keyFrames[pointAIndex]->getWorldPosition();
            glm::vec3 pointB = keyFrames[pointBIndex]->getWorldPosition();
            glm::vec3 newPosition = glm::mix(pointA, pointB, currentFrame - int(currentFrame));
            target->setPosition(newPosition);
        }
    }

    void updateCubic(float deltaTime){
        if(isPlaying){
            int pointAIndex = std::max(0, (int)currentFrame - 1);
            int pointBIndex = (int)currentFrame;
            int pointCIndex = std::min((int)keyFrames.size() - 1, (int)currentFrame + 1);
            int pointDIndex = std::min((int)keyFrames.size() - 1, (int)currentFrame + 2);

            glm::vec3 pointA = keyFrames[pointAIndex]->getWorldPosition();
            glm::vec3 pointB = keyFrames[pointBIndex]->getWorldPosition();
            glm::vec3 pointC = keyFrames[pointCIndex]->getWorldPosition();
            glm::vec3 pointD = keyFrames[pointDIndex]->getWorldPosition();

            glm::vec3 newPosition;

            if (keyFrames.size() == 2) {
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
        return keyFrames.size();
    }

    Entity* getLastControlPoint(){
        return keyFrames[keyFrames.size() - 1];
    }

    void addControlPoint(Entity* controlPoint){
        keyFrames.push_back(controlPoint);
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
            ImGui::DragFloat("Current Frame", &currentFrame, 0.1f, 0.0f, keyFrames.size() - 1);
        }
        

        ImGui::Checkbox("Loop", &isLoop);
        ImGui::Checkbox("Cubic", &isCubic);

        ImGui::Text("Control Points------------------");

        for (int i = 0; i < keyFrames.size(); i++) {
            ImGui::Text("\nControl Point %d", i);
            ImGui::PushID(i); //prevents name collision
            keyFrames[i]->OnGui();
            ImGui::PopID();
        }
    }


private:
    Entity* target;
    std::vector<Entity*> keyFrames;
    bool isPlaying = false;
    bool isLoop = true;
    bool isCubic = false;
    float speed = 1.0f;
    float currentFrame = 0.0f;
    

};

#endif // ANIMATOR_H

#ifndef FLOAT_ANIMATOR_H
#define FLOAT_ANIMATOR_H

#include "resourceManager.h"

class FloatAnimator{

public:
    FloatAnimator(float* target, float speed){
        this->target = target;
        this->speed = speed;
    }

    void update(){
        if(isPlaying){
            float deltaTime = ResourceManager::getDeltaTime();
            currentFrame += deltaTime * speed;

            if(currentFrame > keyFrames.size() - 1){
                if(isLoop){
                    currentFrame = 0;
                } else{
                    currentFrame = keyFrames.size() - 1;
                }
            }

            if(keyFrames.size() > 1){
                if(isCubic){
                    updateCubic(currentFrame);
                } else{
                    updateLinear(currentFrame);
                }
            } else{
                *target = keyFrames[0];
            }

        } else{
            if(currentFrame != 0){
                currentFrame = 0;
                if(keyFrames.size() > 1){
                    *target = keyFrames[0];
                }
            }
        }
    }

    void updateLinear(float deltaTime){
        if(isPlaying){
            int pointAIndex = (int)currentFrame;
            int pointBIndex = (int)currentFrame + 1;
            if(pointBIndex > keyFrames.size() - 1){
                pointBIndex = 0;
            }
            float pointA = keyFrames[pointAIndex];
            float pointB = keyFrames[pointBIndex];
            float newPosition = interpolateLinear(pointA, pointB, currentFrame - int(currentFrame));
            *target = newPosition;
        }
    }

    void updateCubic(float deltaTime){
        if(isPlaying){
            int pointAIndex = std::max(0, (int)currentFrame - 1);
            int pointBIndex = (int)currentFrame;
            int pointCIndex = std::min((int)keyFrames.size() - 1, (int)currentFrame + 1);
            int pointDIndex = std::min((int)keyFrames.size() - 1, (int)currentFrame + 2);

            float pointA = keyFrames[pointAIndex];
            float pointB = keyFrames[pointBIndex];
            float pointC = keyFrames[pointCIndex];
            float pointD = keyFrames[pointDIndex];

            float newPosition;

            if (keyFrames.size() == 2) {
                float t = currentFrame - pointAIndex;
                newPosition = interpolateLinear(pointA, pointB, t);
            } else {
                newPosition = interpolateCubic(pointA, pointB, pointC, pointD, currentFrame - int(currentFrame));
            }

            *target = newPosition;
        }
        else{
            currentFrame = 0;
        }
    }

    float interpolateLinear(float pointA, float pointB, float time){
        return pointA + (pointB - pointA) * time;
    }

    float interpolateCubic(float pointA, float pointB, float pointC, float pointD, float time){
        float t2 = time * time;
        float t3 = t2 * time;

        float result = 0.5f * ((2.0f * pointB) +
            (-pointA + pointC) * time +
            (2.0f * pointA - 5.0f * pointB + 4.0f * pointC - pointD) * t2 +
            (-pointA + 3.0f * pointB - 3.0f * pointC + pointD) * t3);

        return result;
    }

    int getControlPointCount(){
        return keyFrames.size();
    }

    float getLastControlPoint(){
        return keyFrames[keyFrames.size() - 1];
    }

    void addControlPoint(float controlPoint){
        keyFrames.push_back(controlPoint);
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
            ImGui::DragFloat("Current Frame", &currentFrame, 0.1f, 0.0f, keyFrames.size() - 1);
        }
        

        ImGui::Checkbox("Loop", &isLoop);
        ImGui::Checkbox("Cubic", &isCubic);

        ImGui::Text("Control Points------------------");
    }

    void setSpeed(float speed){
        this->speed = speed;
    }

    void setLoop(bool isLoop){
        this->isLoop = isLoop;
    }

    void setCubic(bool isCubic){
        this->isCubic = isCubic;
    }

    void setPlaying(bool isPlaying){
        this->isPlaying = isPlaying;
    }

    void setCurrentFrame(float currentFrame){
        this->currentFrame = currentFrame;
    }

    void setKeyFrames(std::vector<float> keyFrames){
        this->keyFrames = keyFrames;
    }

    float getCurrentFrame(){
        return currentFrame;
    }

private:
    float* target;
    std::vector<float> keyFrames;
    bool isPlaying = false;
    bool isLoop = true;
    bool isCubic = false;
    float speed = 1.0f;
    float currentFrame = 0.0f;
    

};


#endif // FLOAT_ANIMATOR_H