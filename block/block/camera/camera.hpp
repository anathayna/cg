//
//  camera.hpp
//  block
//
//  Created by ana thayna on 26/11/20.
//

#ifndef camera_hpp
#define camera_hpp

#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera();
    Camera(glm::vec3 startPosition, glm::vec3 startWorldUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
    ~Camera();

    void keyControl(bool* keys, GLfloat deltaTime);
    void mouseControl(GLfloat xChange, GLfloat yChange, GLfloat deltaTime);

    glm::mat4 calculateViewMatrix();

    glm::vec3 getCameraPosition() { return position; };
    glm::vec3 getCameraDirection() { return glm::normalize(front); };

private:
    void update();

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    GLfloat yaw;
    GLfloat pitch;

    GLfloat moveSpeed, trunSpeed;
};

#endif /* camera_hpp */
