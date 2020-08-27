//
//  main.cpp
//  splash
//
//  Created by Ana Thayna Franca on 20/08/20.
//  Copyright © 2020 Ana Thayna Franca. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, pShader;

static const char* vShader = "                   \n\
#version 330                                     \n\
                                                 \n\
layout(location=0) in vec3 pos;                  \n\
                                                 \n\
void main(){                                     \n\
  gl_Position =  vec4(pos.x, pos.y, pos.z, 1.0); \n\
}                                                \n\
";

static const char* fShader = "                   \n\
#version 330                                     \n\
                                                 \n\
out vec4 color;                                  \n\
                                                 \n\
void main(){                                     \n\
  color =  vec4(1.0, 0.0, 0.0, 1.0);             \n\
}                                                \n\
";

void CreateTriangle() {
    GLfloat vertices[] = {
        //X      Y     Z
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    //VAO
    glGenVertexArrays(1, &VAO); //Gera um VAO ID
    glBindVertexArray(VAO); //Atribui o ID ao VAO

        //VBO
        glGenBuffers(1, &VBO); //Gera um VBO ID
        glBindBuffer(GL_ARRAY_BUFFER, VBO); //Atribui o ID ao VBO
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Atribuindo os valores do vertice para o VBO

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); //0:  shader location | 3: numero de valores de vertice (x,y,z)
                                                                   //GL_FLOAT: Tipo de dados | GL_FALSE: normalizado | 0: pular elemento (cor)
                                                                   //0: offset
            glEnableVertexAttribArray(0); //0:  shader location

        glBindBuffer(GL_ARRAY_BUFFER, 0); //remove o VBO da memÛria
    glBindVertexArray(0); //remove o VAO da memÛria
}

void CompileShader() {
    pShader = glCreateProgram(); //Inicia o programa
    if (!pShader) {
        printf("Erro ao criar o programa");
        return;
    }

    GLuint shader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* code[1];
    code[0] = vShader;

    GLint codeLenght[1];
    codeLenght[0] = strlen(vShader);

    glShaderSource(shader, 1, code, codeLenght);
    glCompileShader(shader); //compila o shader
    glAttachShader(pShader, shader); //attachch

    shader = glCreateShader(GL_FRAGMENT_SHADER);
    code[0] = fShader;
    codeLenght[0] = strlen(fShader);

    glShaderSource(shader, 1, code, codeLenght);
    glCompileShader(shader); //compila o shader
    glAttachShader(pShader, shader); //attachch

    glLinkProgram(pShader); //Programa
}


int main() {

    //INICIALIZAR O GLFW
    if (!glfwInit()) {
        printf("GLFW Nao foi inicializado");
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

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Nova Janela", NULL, NULL);
    if (!mainWindow) {
        printf("GLWF nao criou a janela");
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
        printf("Glew nao foi iniciado");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    };

    //Configurando viewport
    glViewport(0, 0, bufferWidth, bufferHeight);

    //Criar o Triangulo
    CreateTriangle();
    CompileShader();

    while (!glfwWindowShouldClose(mainWindow)) {
        //Ativa inputs e eventos
        glfwPollEvents();

        //Limpa a janela, cor
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //Desenhar o triangulo
        glUseProgram(pShader);
            glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}
