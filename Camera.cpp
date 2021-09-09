#include "Camera.h"

Camera::Camera(){}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMovementSpeed, GLfloat startTurnSpeed) {
    position = startPosition;
    worldUp = startUp;
    yaw = startYaw;
    pitch = startPitch;

    movementSpeed = startMovementSpeed;
    turnSpeed = startTurnSpeed;

    update();
}

void Camera::update() {
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::keyControl(bool* keys, GLfloat deltaTime) {
    GLfloat velocity = movementSpeed * deltaTime;
    if (keys[GLFW_KEY_UP] || keys[GLFW_KEY_W]) {
        position += front * velocity;
    }
    if (keys[GLFW_KEY_DOWN] || keys[GLFW_KEY_S]) {
        position -= front * velocity;
    }
    if (keys[GLFW_KEY_RIGHT] || keys[GLFW_KEY_D]) {
        position += right * velocity;
    }
    if (keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_A]) {
        position -= right * velocity;
    }
    if (keys[GLFW_KEY_Q]) {
        yaw -= turnSpeed * 10;
    } 
    if (keys[GLFW_KEY_E]) {
        yaw += turnSpeed * 10;
    }

}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange) {
    xChange *= turnSpeed;
    yChange *= turnSpeed;

    yaw += xChange;
    pitch += yChange;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }

    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    update();
}


glm::mat4 Camera::calculateViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition() {
    return position;
}

glm::vec3 Camera::getCameraDirection() {
    return front;
}

Camera::~Camera() {}