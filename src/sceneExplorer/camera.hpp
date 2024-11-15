#pragma once

#include "mySceneExp.hpp"

// Direction of Camera Movement
enum CamMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NUM_DIRECTIONS,
    NO_MOVEMENT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 0.005f;  // units per second
const float SENSITIVITY = 0.1f;

class Camera {
    public:
        Camera (const glm::vec3& position = glm::vec3(0.0f,0.0f,0.0f),
                float yaw = YAW, float pitch = PITCH)
                : position(position), yaw(yaw), pitch(pitch),
                  movementSpeed(SPEED), mouseSensitivity(SENSITIVITY) {
            updateCameraVectors();
        }
    
        glm::mat4 getViewMatrix() const{
            return glm::lookAt(position, position + front, up);
        }
    
        void move(bool cameraIsMoving[], float deltaTime);
    
        void processMouseMovement(float xoffset, float yoffset, bool constrainPitch=true);
    
        const glm::vec3& getPosition(){return position;}
        const glm::vec3& getFront(){return front;}
        const glm::vec3& getUp(){return up;}
        const glm::vec3& getRight(){return right;}

    private:
        // In the scene explorer we don't consider roll angles,
        // so the `WORLD_UP` direction is always the same
        const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        float yaw;
        float pitch;
        float movementSpeed;
        float mouseSensitivity;
        
        // Use the new `yaw` and `pitch` values to update the camera direction vectors
        void updateCameraVectors();
};