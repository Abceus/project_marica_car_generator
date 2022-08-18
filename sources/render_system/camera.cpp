#include "render_system/camera.h"

glm::mat4 Camera::getProjectionMatrix() const {
    glm::vec3 direction;
    direction.x = cos(glm::radians(getRotation().y)) * cos(glm::radians(getRotation().x));
    direction.y = sin(glm::radians(getRotation().x));
    direction.z = sin(glm::radians(getRotation().y)) * cos(glm::radians(getRotation().x));

    glm::vec3 cameraDirection = glm::normalize(direction);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

    return glm::lookAt(getLocation(), 
            getLocation()+cameraDirection, 
            cameraUp);
}
