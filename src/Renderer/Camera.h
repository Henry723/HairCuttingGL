#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include<iostream>

using namespace glm;

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
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 worldUp;
    // euler Angles
    float yaw;
    float pitch;
    // camera Options
    float movementSpeed;
    float radius;

    Camera() { 
        position = vec3(0.0f, 0.0f, RADIUS);
        worldUp = vec3(0.0f, 1.0f, 0.0f); 
        yaw = YAW; 
        pitch = PITCH; 
        front = vec3(0.0f, 0.0f, -1.0f); 
        movementSpeed = SPEED; 
        radius = RADIUS;
        UpdateCameraVectors();
    };
    Camera(vec3 pos, vec3 up, float yaw, float pitch);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    mat4 GetViewMatrix();
    vec3 GetPosition();
    void Debug();
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

private:
    void UpdateCameraVectors();
};
