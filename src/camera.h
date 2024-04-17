#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Entity;

enum Camera_Projection {
    ORTHO,
    PERSP
};

enum Camera_Mode {
    FPS,
    TPS,
    FREE
};

class Camera {
public:
    Camera(glm::vec3 worldUp, Camera_Projection projection, float fov, float aspect, float near, float far, float tpsOffset = 15.0f);
    Camera(glm::vec3 worldUp, Camera_Projection projection, float left, float right, float bottom, float top, float near, float far);
    ~Camera();

    void OnUpdate();
    void OnStart();
    void OnGui();

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    glm::vec3 getPosition();
    glm::vec3 getFront();
    glm::vec3 getUp();
    glm::vec3 getRight();
    glm::vec3 getRotationEuler();
    void setPosition(glm::vec3 position);
    void lookAt(glm::vec3 target, glm::vec3 up);
    void setActive(bool active);
    void setTarget(Entity* target);
    void setMode(Camera_Mode mode);
    void setTpsOffset(float offset);
    void setSpeed(float speed){freeCameraVelocity = speed;}
    Camera_Mode getMode();

private:
    void updateCameraRotation();
    void updateCameraPosition();
    void updateCameraVectors();
    void updateViewMatrix();

    bool isActive = false;
    int currentMode = 0;
    Camera_Projection projection;
    Camera_Mode mode = FPS;
    float freeCameraVelocity = 0.1f;
    float tpsOffset = 15.0f;
    float rotationSpeed = 0.1f;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    glm::vec3 rotationEuler = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    Entity* target = nullptr;
};

#endif // CAMERA_H
