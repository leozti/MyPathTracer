#include "camera.hpp"

using glm::vec3;
using glm::radians;
using glm::normalize;

void Camera::updateCameraVectors() {
    // calculate the new front vector
    vec3 newFront;
    newFront.x = cos(radians(yaw)) * cos(radians(pitch));
    newFront.y = sin(radians(pitch));
    newFront.z = sin(radians(yaw)) * cos(radians(pitch));
    front = normalize(newFront);
    // also re-calculate the right and up vectors
    if (front != WORLD_UP) {
        right = normalize(glm::cross(front, WORLD_UP));
    }
    up = normalize(glm::cross(right, front));
}

void Camera::move(bool cameraIsMoving[], float deltaTime){
    float movementScale = movementSpeed * deltaTime;
    vec3 movement(0.0f,0.0f,0.0f);

    if (cameraIsMoving[FORWARD]) movement += normalize(vec3(front.x,0.0f,front.z));
    if (cameraIsMoving[BACKWARD]) movement -= normalize(vec3(front.x,0.0f,front.z));
    if (cameraIsMoving[LEFT]) movement -= right; 
    if (cameraIsMoving[RIGHT]) movement += right; 
    if (cameraIsMoving[UP]) movement += WORLD_UP;
    if (cameraIsMoving[DOWN]) movement -= WORLD_UP; 

    position += movement*movementScale; 
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch){
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, the screen doesn't get flipped
    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    updateCameraVectors();
}