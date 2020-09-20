//
//  main.cpp
//  splash
//
//  Created by Ana Thayna Franca on 20/08/20.
//  Copyright © 2020 Ana Thayna Franca. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <vector>

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

std::vector<mesh*> meshList;

//variaveis globais
bool direction = true, sizeDirection = true, angleDirection = true; //se for true andar para a direita se for false andar para a esquerda
float triOffset = 0.0f, triOffsetMax = 0.7f, triIncrement = 0.005f;
float size = 0.4f, maxSize = 0.8f, minSize = 0.1f, sizeIncrement = 0.005f;
float angle = 0.0, maxAngle = 360.0f, minAngle = 0.0f, angleIncrement = 0.1f;

static const char* vShader = "                           \n\
#version 330                                             \n\
                                                         \n\
layout(location=0) in vec3 pos;                          \n\
                                                         \n\
out vec4 vCol;                                           \n\
                                                         \n\
uniform mat4 model;                                      \n\
uniform mat4 projection;                                 \n\
                                                         \n\
void main(){                                             \n\
  gl_Position = projection * model * vec4(pos, 1.0);     \n\
  vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);             \n\
}                                                        \n\
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
        0.0f,  1.0f, 0.0f,   //vértice 0 (x,y,z)
        1.0f, -1.0f, 0.0f,   //vértice 1 (x,y,z)
       -1.0f, -1.0f, 0.0f,   //vértice 2 (x,y,z)
        0.0f, -1.0f, 1.0f    //vértice 3 (x,y,z)
    };

    unsigned int indices[] = {
        0,1,2, //frente da pirâmide
        0,1,3, //parede lateral direita
        0,2,3, //parede lateral esquerda
        1,2,3  //base da pirâmide
    };

    mesh* obj1 = new mesh();
    obj1->CreateMesh(vertices, indices, sizeof(vertices), sizeof(indices));
    meshList.push_back(obj1);

    mesh* obj2 = new mesh();
    obj2->CreateMesh(vertices, indices, sizeof(vertices), sizeof(indices));
    meshList.push_back(obj2);
}

void AddShader(GLenum shaderType, const char* shaderCode) {
    //começamos a compilar cada shader
    //1. criar um shader
    GLuint shader = glCreateShader(shaderType);
    
    //2. atribui o código do GLSL para o shader
    //2.1 guarda e converte a variável char em GLchar
    const GLchar* code[1];
    code[0] = shaderCode;
    
    //2.2 anexa o código ao shader
    glShaderSource(shader, 1, code, NULL); //1: número de códigos que estão sendo enviados | NULL: final da string, termina o código

    //3. compila o shader
    glCompileShader(shader); //compila o shader

    //4. tratamento de erros de compilação
    GLint result = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result); //colocar o valor do o status da compilação na variável result
    if (!result) {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(shader, sizeof(log), NULL, log); //busca o texto caso de algum erro na compilação
        printf("Erro ao compilar o %d shader '%s'\n", shaderType, log);
        return;
    }

    //5. anexa o shader ao programa
    glAttachShader(pShader, shader); //anexa shader ao programa "variável global"
}

void CompileShader() {
    //1. criar um programa
    pShader = glCreateProgram(); //inicia o programa
    if (!pShader) {
        printf("Erro ao criar o programa");
        return;
    }

    //2. compila o vertex shader
    AddShader(GL_VERTEX_SHADER, vShader);
    //3. compila o fragment shader
    AddShader(GL_FRAGMENT_SHADER, fShader);

    //4. criar o link do programa
    glLinkProgram(pShader); //programa

    //5. validação de link do programa
    GLint result = 0;
    glGetProgramiv(pShader, GL_LINK_STATUS, &result); //colocar o valor do o status da compilação na variável result
    if (!result) {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(pShader, sizeof(log), NULL, log); //busca o texto caso de algum erro na compilação
        printf("Erro ao linkar o programa: '%s'\n", log);
        return;
    }

    //6. validação do programa
    glValidateProgram(pShader);
    result = 0;
    glGetProgramiv(pShader, GL_VALIDATE_STATUS, &result); //colocar o valor do o status da compilação na variável result
    if (!result) {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(pShader, sizeof(log), NULL, log); //busca o texto caso de algum erro na compilação
        printf("erro ao validar o programa: '%s'\n", log);
        return;
    }
}

int main() {
    //inicializar O GLFW
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

    //pegar o buffer size da largura e altura
    int bufferWidth, bufferHeight;
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

    //criar o Triangulo
    CreateTriangle(); //coloca os dados na memória da placa de vídeo
    CompileShader(); //compila os Shaders
    
    while (!glfwWindowShouldClose(mainWindow)) {
        //ativa inputs e eventos
        glfwPollEvents();

        /********************************
        * cor de fundo da tela
        *********************************/
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//limpa a janela, cor
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //espaço de memória

        /********************************
        * triangulo
        *********************************/
        //desenhar o triangulo
        glUseProgram(pShader); //usar o programa

            /********************************
            * movimenta triangulo
            *********************************/
            //regra para movimentação
            if (direction) //se a direção for true, andar para direita
                triOffset += triIncrement; //incrementa valor no offset para andar para direita
            else //se a direção for false, andar para esquerda
                triOffset -= triIncrement; //decrementa o valor no offset para andar para esquerda

            if (abs(triOffset) >= triOffsetMax) //se o valor for igual ou maior que o máximo, mudar a direção
                direction = !direction; //inverte a direção
            
            //regra para alteração de tamanho
            if (sizeDirection)
                size += sizeIncrement;
            else
                size -= sizeIncrement;

            if (size >= maxSize || size <= minSize)
                sizeDirection = !sizeDirection;
            
            //regra para calcular o angulo
            if (angleDirection)
                angle += angleIncrement;
            else
                angle -= angleIncrement;

            if (angle >= maxAngle || angle <= minAngle)
                angleDirection = !angleDirection;


            //calcula as mudanças da model
            GLint uniModel = glGetUniformLocation(pShader, "model"); //procura a entrada chamada model
            glm::mat4 model(1.0f); //cria uma matriz 4x4 e coloca os valores 1.0f em todas as posições
            
            //movimentação
            model = glm::translate(model, glm::vec3(triOffset, -0.5f, -2.5f)); //traduz o modelo para movimentar a posição (x,y,z)

            //rotate
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

            //ramanho
            model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
            glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

            //calcula as mudanças da projeção
            GLint uniProjection = glGetUniformLocation(pShader, "projection"); //procura a entrada chamada projection
            glm::mat4 projection = glm::perspective(1.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);
            //adiciona no vertex o valor calculado
            glUniformMatrix4fv(uniProjection, 1, GL_FALSE, glm::value_ptr(projection));

        /********************************
        * finaliza o triangulo na tela
        *********************************/
        meshList[0]->RenderMesh();

        model = glm::mat4(1.0f); //cria uma matriz 4x4 colocando 1.0f em cada uma das posições
        model = glm::translate(model, glm::vec3(-triOffset, 0.5f, -2.5f)); //traduz o modelo para movimentar a posição (x,y,z)
        model = glm::rotate(model, glm::radians(-angle), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
        meshList[1]->RenderMesh();
        
        glUseProgram(0); //removo o Programa da memória

        //atualiza a tela
        glfwSwapBuffers(mainWindow);
    }

    return 0;
}
