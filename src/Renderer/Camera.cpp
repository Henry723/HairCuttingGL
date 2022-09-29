#include "Camera.h"

Camera::Camera(vec3 pos = vec3(0.0f, 0.0f, RADIUS), vec3 up = vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f))
{
    position = pos;
    worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    UpdateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f))
{
    position = glm::vec3(posX, posY, posZ);
    worldUp = glm::vec3(upX, upY, upZ);
    this->yaw = yaw;
    this->pitch = pitch;
    UpdateCameraVectors();
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
mat4 Camera::GetViewMatrix()
{
    return lookAt(position, glm::vec3(0.0, 0.0, 0.0), up);
}

vec3 Camera::GetPosition()
{
    return position;
}

void Camera::Debug()
{
    std::cout << "Position x:" << position.x << "Position y:" << position.y << "Position z:" << position.z << std::endl;
    //std::cout << "Pitch:" << Pitch << "Yaw:" << Yaw << std::endl;
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime; //Think about value rotation here
    // Constrain camera y position so it does not get flipped
    if (direction == Camera_Movement::UPWARD) {
        if (position.y < radius - 1)
            pitch += velocity;
    }

    if (direction == Camera_Movement::DOWNWARD) {
        if (position.y > -radius + 1)
            pitch -= velocity;
    }

    if (direction == Camera_Movement::LEFT)
        yaw -= velocity;
        
    if (direction == Camera_Movement::RIGHT)
        yaw += velocity;

    if (direction == Camera_Movement::IN) {
        if(radius > 2)
            radius -= velocity;
    }

    if (direction == Camera_Movement::OUT) {
        if (radius < 12)
            radius += velocity;
    }

    float camX = radius * sin(yaw) * cos((pitch));
    float camY = radius * sin(pitch);
    float camZ = radius * cos((yaw)) * cos((pitch));
    position = vec3(camX, camY, camZ);
}

// calculates the front vector from the Camera's (updated) Euler Angles
void Camera::UpdateCameraVectors()
{
    // calculate the new Front vector
    vec3 front;
    front.x = cos(radians(yaw)) * cos(radians(pitch));
    front.y = sin(radians(pitch));
    front.z = sin(radians(yaw)) * cos(radians(pitch));
    front = normalize(front);
    // also re-calculate the Right and Up vector
    right = normalize(cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up = normalize(cross(right, front));
}