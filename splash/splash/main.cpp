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
#include "shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

const GLint WIDTH = 800, HEIGHT = 600;
GLuint pShader;

std::vector<mesh*> meshList;
std::vector<Shader> shaderList;

static const char* vertexLocation = "Shaders/VertexShader.glsl";
static const char* fragmentLocation = "Shaders/FragmentShader.glsl";

//variaveis globais
bool direction = true, sizeDirection = true, angleDirection = true; //se for true andar para a direita se for false andar para a esquerda
float triOffset = 0.0f, triOffsetMax = 0.7f, triIncrement = 0.005f;
float size = 0.4f, maxSize = 0.8f, minSize = 0.1f, sizeIncrement = 0.005f;
float angle = 0.0, maxAngle = 360.0f, minAngle = 0.0f, angleIncrement = 0.1f;

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

void CreateShader() {
    Shader* shader1 = new Shader();
    shader1->CreateFromFile(vertexLocation, fragmentLocation);
    shaderList.push_back(*shader1);
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

    //criar o triangulo
    CreateTriangle(); //coloca os dados na memória da placa de vídeo
    CreateShader(); //cria os Shaders
    
    glm::mat4 projection = glm::perspective(1.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(mainWindow)) {
        //ativa inputs e eventos
        glfwPollEvents();

        /********************************
        * cor de fundo da tela
        *********************************/
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //limpa a janela, cor
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /********************************
        * piramides
        *********************************/
        shaderList[0].UseProgram(); //usar o programa
        glUniformMatrix4fv(shaderList[0].getUniformProjection(), 1, GL_FALSE, glm::value_ptr(projection)); //movimentação da projeção da camera
        
            /********************************
            * piramide 1
            *********************************/
            glm::mat4 model(1.0f); //cria uma matriz 4x4 e coloca os valores 1.0f em todas as posições
            model = glm::translate(model, glm::vec3(0.0f, -0.25f, -2.5f)); //traduz o modelo para movimentar a posição (x,y,z)
            model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
            //model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(shaderList[0].getUniformModel(), 1, GL_FALSE, glm::value_ptr(model));
            meshList[0]->RenderMesh();

            /********************************
            * piramide 2
            *********************************/
            model = glm::mat4(1.0f); //cria uma matriz 4x4 colocando 1.0f em cada uma das posições
            model = glm::translate(model, glm::vec3(0.0f, 0.75f, -2.5f)); //traduz o modelo para movimentar a posição (x,y,z)
            model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
            glUniformMatrix4fv(shaderList[0].getUniformModel(), 1, GL_FALSE, glm::value_ptr(model));
            meshList[1]->RenderMesh();
        
        glUseProgram(0); //removo o programa da memória

        //atualiza a tela
        glfwSwapBuffers(mainWindow);
    }

    return 0;
}
