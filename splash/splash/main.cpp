//
//  main.cpp
//  splash
//
//  Created by Ana Thayna Franca on 20/08/20.
//  Copyright © 2020 Ana Thayna Franca. All rights reserved.
//

#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <time.h>
#include <chrono>
#include <math.h>

#include "mesh.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


const GLint WIDTH = 800, HEIGHT = 600;
GLuint pShader;

//variaveis globais
bool moveToRight = true;
bool sizeDirection = true;
bool angleDirection = true;

float triOffset = 0.0f, triOffsetMAX = 0.7f, triIncrement = 0.01f;
float size = 0.4f, maxSize = 0.8f, minSize = 0.1f, sizeIncrement = 0.01f;
float angle = 0.0f, maxAngle = 360.0f,minAngle = 0.0f, angleIncrement = 0.8f;


static const char* vShader = "                                  \n\
#version 330                                                    \n\
                                                                \n\
layout(location=0) in vec3 pos;                                 \n\
                                                                \n\
out vec4 vCol;                                                  \n\
                                                                \n\
uniform mat4 model;                                             \n\
uniform mat4 projection;                                        \n\
                                                                \n\
void main(){                                                    \n\
  gl_Position =  projection * model * vec4(pos, 1.0);           \n\
  vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);                    \n\
}                                                               \n\
";

static const char* fShader = "                   \n\
#version 330                                     \n\
                                                 \n\
uniform vec3 triangleColor;                      \n\
                                                 \n\
in vec4 vCol;                                    \n\
                                                 \n\
out vec4 color;                                  \n\
                                                 \n\
void main(){                                     \n\
  color =  vCol;                                 \n\
}                                                \n\
";

void CreateTriangle() {
    GLfloat vertices[] = {
         0.0f,  1.0f, 0.0f,   //Vértice 0 (x,y,z)
         1.0f, -1.0f, 0.0f,   //Vértice 1 (x,y,z)
        -1.0f, -1.0f, 0.0f,   //Vértice 2 (x,y,z)
         0.0f,  0.0f, 1.0f    //Vértice 3 (x,y,z)
    };
    
    //0 0 0 (Preto)
    //1 0 0 (Preto)
    //0 1 0 (Preto)
    
    unsigned int indices[] = {
        0,1,2,  // frente da piramide
        0,1,3,  // Parede esquerda
        0,2,3,  // Parede direita
        1,2,3  // Base
    };
    
    mesh* obj = new mesh();
    obj->CreateMesh(vertices, indices, sizeof(vertices), sizeof(indices))
}

void AddShader(GLenum shaderType, const char* shaderCode) {
    //Começamos a compilar cada shader
    //1. Criar um shader
    GLuint shader = glCreateShader(shaderType);
    
    //2. Atribui o código do GLSL para o shader
    //2.1 Guarda e converte a variável char em GLchar
    const GLchar* code[1];
    code[0] = shaderCode;
    
    //2.2 Anexa o código ao shader
    glShaderSource(shader, 1, code, NULL); //1: número de códigos que estão sendo enviados | NULL: final da string, termina o código

    //3. Compila o shader
    glCompileShader(shader); //compila o shader

    //4. Tratamento de erros de compilação
    GLint result = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result); //Colocar o valor do o status da compilação na variável result
    if (!result) {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(shader, sizeof(log), NULL, log); //Busca o texto caso de algum erro na compilação
        printf("Erro ao compilar o %d shader '%s'\n", shaderType, log);
        return;
    }

    //5. Anexa o shader ao programa
    glAttachShader(pShader, shader); //Anexa shader ao programa "variável global"
}

void CompileShader() {
    //1. Criar um programa
    pShader = glCreateProgram(); //Inicia o programa
    if (!pShader) {
        printf("Erro ao criar o programa");
        return;
    }

    //2. Compila o Vertex Shader
    AddShader(GL_VERTEX_SHADER, vShader);
    //3. Compila o Fragment Shader
    AddShader(GL_FRAGMENT_SHADER, fShader);

    //4. Criar o link do programa
    glLinkProgram(pShader); //Programa

    //5. Validação de link do programa
    GLint result = 0;
    glGetProgramiv(pShader, GL_LINK_STATUS, &result); //Colocar o valor do o status da compilação na variável result
    if (!result) {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(pShader, sizeof(log), NULL, log); //Busca o texto caso de algum erro na compilação
        printf("Erro ao linkar o programa: '%s'\n", log);
        return;
    }

    //6. Validação do programa
    glValidateProgram(pShader);
    result = 0;
    glGetProgramiv(pShader, GL_VALIDATE_STATUS, &result); //Colocar o valor do o status da compilação na variável result
    if (!result) {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(pShader, sizeof(log), NULL, log); //Busca o texto caso de algum erro na compilação
        printf("Erro ao validar o programa: '%s'\n", log);
        return;
    }
}

int main() {
    auto t_start = std::chrono::high_resolution_clock::now();
    //INICIALIZAR O GLFW
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

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "splash", NULL, NULL);
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
    
    glEnable(GL_DEPTH_TEST);

    //Configurando viewport
    glViewport(0, 0, bufferWidth, bufferHeight);

    //Criar o Triangulo
    CreateTriangle(); //Coloca os dados na memória da placa de vídeo
    CompileShader(); //Compila os Shaders
    
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat) bufferWidth / (GLfloat) bufferHeight, 0.1f, 100.0f);

    
    while (!glfwWindowShouldClose(mainWindow)) {
        //Ativa inputs e eventos
        glfwPollEvents();

        //Limpa a janela, cor
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT);

        //Desenhar o triangulo
        glUseProgram(pShader); //Usar o programa
        
        auto t_now = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
            
        GLint uniColor = glGetUniformLocation(pShader, "triangleColor"); //Procura a entrada chamada triangleColor
        float r = (sin(time * 4.0f) + 1.0f) / 2.0f;
        float g = 0.0f;
        float b = 0.0f;
        glUniform3f(uniColor, r, g, b);

        // Movimenta o triangulo
        if (moveToRight) {
            triOffset += triIncrement;
        } else {
            triOffset -= triIncrement;
        }
        
        if (abs(triOffset) >= triOffsetMAX) {
            moveToRight = !moveToRight;
        }
        
        // Alteração de tamanho
        if (sizeDirection) {
            size += sizeIncrement;
        } else {
            size -= sizeIncrement;
        }
        
        if (size >= maxSize || size <= minSize) {
            sizeDirection = !sizeDirection;
        }
        
        // Calcular o angulo
        if (angleDirection) {
            angle += angleIncrement;
        } else {
            angle -= angleIncrement;
        }
        
        if (angle >= maxAngle || angle <= minAngle) {
            angleDirection = !angleDirection;
        }
        
        GLint uniModel = glGetUniformLocation(pShader, "model");
        glm::mat4 model(1.0f); // cria uma matriz 4x4 e coloca os valores 1.0f em todas as posições
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f)); // traduz o modelo para movimentar a posição (x,y,z)
        
        // Tamanho
        model = glm::scale(model, glm::vec3(0.4f, 0.4, 0.4f));
        
        // Rotate
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        
        
        
        // movimentação (uniModel, 1 uma matriz, GL_FALSE: não está transposta, glm::value_ptr(model): transforma para pontos)
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
        
        GLint uniProjection = glGetUniformLocation(pShader, "projection");
        glUniformMatrix4fv(uniProjection, 1, GL_FALSE, glm::value_ptr(projection));
        
        //glDrawArrays(GL_TRIANGLES, 0, 3); //Desenha na tela GL_TRIANGLE | 0: Primeira  | 3: Quantidade de Vértices (-1-1, 1,-1, 0,1)
                
        glBindVertexArray(0); //Removo o VAO da memória
        glUseProgram(0); //Removo o Programa da memória

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}
