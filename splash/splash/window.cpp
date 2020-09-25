//
//  window.cpp
//  splash
//
//  Created by Ana Thayna Franca on 24/09/20.
//  Copyright © 2020 Ana Thayna Franca. All rights reserved.
//

#include "window.hpp"

window::window() {
    width = 800;
    height = 600;
}

window::window(GLint windowWidth, GLint windowHeight) {
    width = windowWidth;
    height = windowHeight;
}

window::~window() {
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}

int window::initialize() {
    //init GLFW
    if (!glfwInit()) {
        printf("GLFW nao foi inicializado");
        glfwTerminate();
        return 1;
    };

    //GLFW OpenGL Version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //Core Profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //Forward Functions
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mainWindow = glfwCreateWindow(width, height, "splash", NULL, NULL);
    if (!mainWindow) {
        printf("GLFW nao criou a janela");
        glfwTerminate();
        return 1;
    }

    //pegar o buffer size da largura e altura
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    //configurando a janela principal
    glfwMakeContextCurrent(mainWindow);

    //GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Glew nao foi iniciado");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    };

    glEnable(GL_DEPTH_TEST); //habilitar o Depth Test

    //configurando viewport
    glViewport(0, 0, bufferWidth, bufferHeight);
}


void window::swapBuffers() {
    glfwSwapBuffers(mainWindow);
}
