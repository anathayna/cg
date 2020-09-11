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

//Variaveis globais
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
        0.0f,  1.0f, 0.0f,   //Vértice 0 (x,y,z)
        1.0f, -1.0f, 0.0f,   //Vértice 1 (x,y,z)
       -1.0f, -1.0f, 0.0f,   //Vértice 2 (x,y,z)
        0.0f, -1.0f, 1.0f    //Vértice 3 (x,y,z)
    };

    unsigned int indices[] = {
        0,1,2, //Frente da pirâmide
        0,1,3, //Parede lateral direita
        0,2,3, //Parede lateral esquerda
        1,2,3  //Base da pirâmide
    };

    mesh* obj1 = new mesh();
    obj1->CreateMesh(vertices, indices, sizeof(vertices), sizeof(indices));
    meshList.push_back(obj1);

    mesh* obj2 = new mesh();
    obj2->CreateMesh(vertices, indices, sizeof(vertices), sizeof(indices));
    meshList.push_back(obj2);
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

    glEnable(GL_DEPTH_TEST); //Habilitar o Depth Test

    //Configurando viewport
    glViewport(0, 0, bufferWidth, bufferHeight);

    //Criar o Triangulo
    CreateTriangle(); //Coloca os dados na memória da placa de vídeo
    CompileShader(); //Compila os Shaders
    
    while (!glfwWindowShouldClose(mainWindow)) {
        //Ativa inputs e eventos
        glfwPollEvents();

        /********************************
        * Cor de fundo da tela
        *********************************/
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//Limpa a janela, cor
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Espaço de memória

        /********************************
        * Triangulo
        *********************************/
        //Desenhar o triangulo
        glUseProgram(pShader); //Usar o programa

            /********************************
            * Movimenta triangulo
            *********************************/
            //Regra para movimentação
            if (direction) //se a direção for true, andar para direita
                triOffset += triIncrement; //incrementa valor no offset para andar para direita
            else //se a direção for false, andar para esquerda
                triOffset -= triIncrement; //decrementa o valor no offset para andar para esquerda

            if (abs(triOffset) >= triOffsetMax) //se o valor for igual ou maior que o máximo, mudar a direção
                direction = !direction; //inverte a direção
            
            //Regra para alteração de tamanho
            if (sizeDirection)
                size += sizeIncrement;
            else
                size -= sizeIncrement;

            if (size >= maxSize || size <= minSize)
                sizeDirection = !sizeDirection;
            
            //Rega para calcular o angulo
            if (angleDirection)
                angle += angleIncrement;
            else
                angle -= angleIncrement;

            if (angle >= maxAngle || angle <= minAngle)
                angleDirection = !angleDirection;


            //CALCULA AS MUDANÇAS DA MODEL
            GLint uniModel = glGetUniformLocation(pShader, "model"); //Procura a entrada chamada model
            glm::mat4 model(1.0f); //cria uma matriz 4x4 e coloca os valores 1.0f em todas as posições
            
            //movimentação
            model = glm::translate(model, glm::vec3(triOffset, -0.5f, -2.5f)); //traduz o modelo para movimentar a posição (x,y,z)

            //Rotate
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

            //Tamanho
            model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
            glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

            //CALCULA AS MUDANÇAS DA PROJEÇÃO
            GLint uniProjection = glGetUniformLocation(pShader, "projection"); //Procura a entrada chamada projection
            glm::mat4 projection = glm::perspective(1.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);
            //Adiciona no vertex o valor calculado
            glUniformMatrix4fv(uniProjection, 1, GL_FALSE, glm::value_ptr(projection));

        /********************************
        * Finaliza o triangulo na tela
        *********************************/
        meshList[0]->RenderMesh();

        model = glm::mat4(1.0f); //cria uma matriz 4x4 colocando 1.0f em cada uma das posições
        model = glm::translate(model, glm::vec3(-triOffset, 0.5f, -2.5f)); //traduz o modelo para movimentar a posição (x,y,z)
        model = glm::rotate(model, glm::radians(-angle), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
        meshList[1]->RenderMesh();
        
        glUseProgram(0); //Removo o Programa da memória

        //Atualiza a tela
        glfwSwapBuffers(mainWindow);
    }

    return 0;
}
