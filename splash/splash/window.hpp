//
//  window.hpp
//  splash
//
//  Created by Ana Thayna Franca on 24/09/20.
//  Copyright © 2020 Ana Thayna Franca. All rights reserved.
//

#ifndef window_hpp
#define window_hpp

#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class window {
public:
    window();
    window(GLint windowWidth, GLint windowHeight);
    ~window();
    int initialize();
    void swapBuffers();

    GLfloat getBufferWidth() { return (GLfloat)bufferWidth; }
    GLfloat getBufferHeight() { return (GLfloat)bufferHeight; }
    bool getWindowShouldClose() { return glfwWindowShouldClose(mainWindow); }
    
private:
    GLFWwindow* mainWindow;
    GLint width, height;
    GLint bufferWidth, bufferHeight;
};

#endif /* window_hpp */
