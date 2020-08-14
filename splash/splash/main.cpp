//
//  main.cpp
//  splash
//
//  Created by Ana Thayna Franca on 13/08/20.
//  Copyright © 2020 Ana Thayna Franca. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

int main() {

    //INICIALIZAR O GLFW
    if (!glfwInit()) {
        printf("GLFW Não foi inicializado");
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

    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Nova Janela", NULL, NULL);
    if (!mainWindow) {
        printf("GLWF não criou a janela");
        glfwTerminate();
        return 1;
    }

    //Pegar o buffer size da largura e altura
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    //Configurando a janela principal
    glfwMakeContextCurrent(mainWindow);

    //GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Glew não foi iniciado");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    };

    //Configurando viewport
    glViewport(0, 0, bufferWidth, bufferHeight);

    while (!glfwWindowShouldClose(mainWindow)) {
        //Ativa inputs e eventos
        glfwPollEvents();

        //Limpa a janela, cor
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}
