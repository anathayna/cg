//
//  main.cpp
//  splash
//
//  Created by Ana Thayna Franca on 13/08/20.
//  Copyright © 2020 Ana Thayna Franca. All rights reserved.
//

#include <stdio.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.hpp"
#include "shader.hpp"
#include "window.hpp"
#include "camera.hpp"

std::vector<mesh*> meshList;
std::vector<shader> shaderList;
window mainWindow;
camera camera;

//old version of FPS
GLfloat deltaTime = 0.0f, lastime = 0.0f;

static const char* vertexLocation = "./shaders/VertexShader.glsl";
static const char* fragmentLocation = "./shaders/FragmentShader.glsl";

void CreateTriangle() {
    GLfloat vertices[] = {
        0.0f,  1.0f, 0.0f,  //vértice 0 (x,y,z)
        1.0f, -1.0f, 0.0f,  //vértice 1 (x,y,z)
       -1.0f, -1.0f, 0.0f,  //vértice 2 (x,y,z)
        0.0f, -1.0f, 1.0f   //vértice 3 (x,y,z)
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
    shader* shader1 = new shader();
    shader1->CreateFromFile(vertexLocation, fragmentLocation);
    shaderList.push_back(*shader1);
}

int main() {
    mainWindow = window(800, 600);
    mainWindow.initialize();

    //criar o Triangulo
    CreateTriangle(); //coloca os dados na memória da placa de vídeo
    CreateShader(); //cria os Shaders

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 8.0f);
    
    glm::mat4 projection = glm::perspective(1.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

    while (!mainWindow.getWindowShouldClose()) {
        //old version of FPS
        GLfloat now = glfwGetTime();
        deltaTime = now - lastime;
        lastime = now;

        //ativa inputs e eventos
        glfwPollEvents();

        //controle do teclado
        camera.keyControl(mainWindow.getKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange(), deltaTime);

        /********************************
        * cor de fundo da tela
        *********************************/
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//limpa a janela, cor
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /********************************
        * piramides
        *********************************/
        shaderList[0].UseProgram(); //usar o programa
        glUniformMatrix4fv(shaderList[0].getUniformProjection(), 1, GL_FALSE, glm::value_ptr(projection)); //movimentação da projeção da camera
        glUniformMatrix4fv(shaderList[0].getUniformView(), 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        
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
        mainWindow.swapBuffers();
    }

    return 0;
}
