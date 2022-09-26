#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include<iostream>
enum class Camera_Movement {
    UPWARD,
    DOWNWARD,
    LEFT,
    RIGHT,
    IN,
    OUT
};

// Default camera values
const float YAW = 0.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float RADIUS = 10.0f;


class Camera
{
public:
    // camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    // euler Angles
    float yaw;
    float pitch;
    // camera Options
    float movementSpeed;
    float radius;

    Camera() { 
        position = glm::vec3(0.0f, 0.0f, RADIUS);
        worldUp = glm::vec3(0.0f, 1.0f, 0.0f); 
        yaw = YAW; 
        pitch = PITCH; 
        front = glm::vec3(0.0f, 0.0f, -1.0f); 
        movementSpeed = SPEED; 
        radius = RADIUS;
        updateCameraVectors();
    };
    Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    glm::mat4 GetViewMatrix();
    glm::vec3 GetPosition();
    void Debug();
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

private:
    void updateCameraVectors();
};
