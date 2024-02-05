#include <vector>
#include <iostream>
#include "camera.h"
#include "resourceManager.h"

Camera::Camera(glm::vec3 worldUp, Camera_Projection projection, float fov, float aspect, float near, float far){
    this->worldUp = worldUp;
    this->projection = projection;
    this->projectionMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
    this->front = glm::vec3(0.0f, 0.0f, -1.0f);
    this->up = worldUp;
}
Camera::Camera(glm::vec3 worldUp, Camera_Projection projection, float left, float right, float bottom, float top, float near, float far){
    this->worldUp = worldUp;
    this->projection = projection;
    this->projectionMatrix = glm::ortho(left, right, bottom, top, near, far);
    this->front = glm::vec3(0.0f, 0.0f, -1.0f);
    this->up = worldUp;
}

void Camera::OnUpdate(){
    ResourceManager::setMouseEnabled(ResourceManager::isMousePressed(GLFW_MOUSE_BUTTON_RIGHT));
    this->updateCameraPosition();
    if(ResourceManager::getIsMouseEnabled()){
        this->updateCameraRotation();
    }
    this->updateCameraVectors();
    this->updateViewMatrix();
}

void Camera::OnStart(){
    
}

glm::mat4 Camera::getViewMatrix(){
    return this->viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix(){
    return this->projectionMatrix;
}

glm::vec3 Camera::getPosition(){
    return this->position;
}

glm::vec3 Camera::getFront(){
    return this->front;
}

glm::vec3 Camera::getUp(){
    return this->up;
}

glm::vec3 Camera::getRight(){
    return this->right;
}

glm::vec3 Camera::getRotationEuler(){
    return this->rotationEuler;
}

void Camera::lookAt(glm::vec3 target){
    this->front = glm::normalize(target - this->position);
    this->rotationEuler.y = glm::degrees(atan2(this->front.z, this->front.x));
    this->rotationEuler.x = glm::degrees(asin(this->front.y));
}

void Camera::setActive(bool active){
    this->isActive = active;
}

void Camera::setTarget(Entity* target){
    this->target = target;
}

void Camera::setMode(Camera_Mode mode){
    this->mode = mode;
    currentMode = mode;
}

void Camera::updateCameraRotation(){
    float yawDelta = ResourceManager::getMouseDeltaX() * rotationSpeed;
    float pitchDelta = ResourceManager::getMouseDeltaY() * rotationSpeed;

    rotationEuler.y += yawDelta;
    rotationEuler.x += pitchDelta;

    if(rotationEuler.x > 89.0f){
        rotationEuler.x = 89.0f;
    }
    if(rotationEuler.x < -89.0f){
        rotationEuler.x = -89.0f;
    }
}

void Camera::updateCameraPosition(){
    float fPress = ResourceManager::getKeyData(GLFW_KEY_F).pressDuration;
    if(fPress < 0.01f && fPress > 0.0f){
        currentMode = (currentMode + 1) % 3;
        if(currentMode == 0){
            mode = FPS;
        } else if(currentMode == 1){
            mode = TPS;
        } else if(currentMode == 2){
            mode = FREE;
        }

        std::cout << "Mode: " << mode << std::endl;
    }
    if(mode == FREE) {
        if(ResourceManager::isKeyPressed(GLFW_KEY_W)){
            this->position += this->front * freeCameraVelocity;
        }
        if(ResourceManager::isKeyPressed(GLFW_KEY_S)){
            this->position -= this->front * freeCameraVelocity;
        }
        if(ResourceManager::isKeyPressed(GLFW_KEY_A)){
            this->position -= this->right * freeCameraVelocity;
        }
        if(ResourceManager::isKeyPressed(GLFW_KEY_D)){
            this->position += this->right * freeCameraVelocity;
        }
        if(ResourceManager::isKeyPressed(GLFW_KEY_SPACE)){
            this->position += this->up * freeCameraVelocity;
        }
        if(ResourceManager::isKeyPressed(GLFW_KEY_LEFT_SHIFT)){
            this->position -= this->up * freeCameraVelocity;
        } 
    } else {
        if(target != nullptr){
            this->position = target->getWorldPosition();
        }
    }
}

void Camera::updateCameraVectors(){
    this->front.x = cos(glm::radians(rotationEuler.y)) * cos(glm::radians(rotationEuler.x));
    this->front.y = sin(glm::radians(rotationEuler.x));
    this->front.z = sin(glm::radians(rotationEuler.y)) * cos(glm::radians(rotationEuler.x));
    this->front = glm::normalize(this->front);
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}

void Camera::updateViewMatrix(){
    if(target != nullptr){
        if(mode == TPS){
            this->viewMatrix = glm::lookAt(this->position - this->front * 15.0f, this->position, this->up);
        } else if(mode == FPS){
            this->viewMatrix = glm::lookAt(this->position, this->position + this->front, this->up);
        } else if(mode == FREE){
            this->viewMatrix = glm::lookAt(this->position, this->position + this->front, this->up);
        }
    } else {
        this->viewMatrix = glm::lookAt(this->position, this->position + this->front, this->up);
    }
}