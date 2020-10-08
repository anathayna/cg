//
//  camera.hpp
//  splash
//
//  Created by Ana Thayna Franca on 01/10/20.
//  Copyright © 2020 Ana Thayna Franca. All rights reserved.
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
