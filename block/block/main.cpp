//
//  main.cpp
//  block
//
//  Created by ana thayna on 19/11/20.
//

#define STB_IMAGE_IMPLEMENTATION
#include "communValues.h"

#include <iostream>
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
#include "texture.hpp"
#include "directionalLight.hpp"
#include "pointLight.hpp"
#include "spotLight.hpp"
#include "material.hpp"

std::vector<mesh*> meshList;
std::vector<shader> shaderList;
Window mainWindow;
Camera camera;

directionalLight mainLight;
pointLight pointLights[MAX_POINT_LIGHTS];
spotLight spotLights[MAX_SPOT_LIGHTS];

material metalMaterial;
material woodMaterial;

texture blockTexture;
texture brickTexture;

GLfloat deltaTime = 0.0f, lastime = 0.0f;

static const char* vertexLocation = "/Users/anathayna/Documents/docs/senac-bcc/cg/block/block/shaders/VertexShader.glsl";
static const char* fragmentLocation = "/Users/anathayna/Documents/docs/senac-bcc/cg/block/block/shaders/FragmentShader.glsl";

void calcAverageNormal(unsigned int* indices, unsigned int indiceCount,
                       GLfloat* vertices, unsigned int verticeCount,
                       unsigned int vLength, unsigned int normalOffset) {
    for (int i = 0; i < indiceCount; i = i + 3) {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i+1] * vLength;
        unsigned int in2 = indices[i+2] * vLength;

        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

        vertices[in0 + normalOffset] += normal.x; vertices[in0 + 1 + normalOffset] += normal.y; vertices[in0 + 2 + normalOffset] += normal.z;
        vertices[in1 + normalOffset] += normal.x; vertices[in1 + 1 + normalOffset] += normal.y; vertices[in1 + 2 + normalOffset] += normal.z;
        vertices[in2 + normalOffset] += normal.x; vertices[in2 + 1 + normalOffset] += normal.y; vertices[in2 + 2 + normalOffset] += normal.z;
    }

    for (int i = 0; i < verticeCount / vLength; i++) {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x;
        vertices[nOffset + 1] = vec.y;
        vertices[nOffset + 2] = vec.z;
    }
}

void CreateTriangle() {
    GLfloat vertices[] = {
        -1.0, 0.0f, -1.0f,         0.0f,   0.0f,      0.0f, -1.0f, 0.0f,    // vértice 0 (x,y,z, u,v, nx,ny,nz)
         1.0, 0.0f, -1.0f,        10.0f,   0.0f,      0.0f, -1.0f, 0.0f,    // vértice 1 (x,y,z, u,v, nx,ny,nz)
        -1.0, 0.0f,  1.0f,         0.0f,  10.0f,      0.0f, -1.0f, 0.0f,    // vértice 2 (x,y,z, u,v, nx,ny,nz)
         1.0, 0.0f,  1.0f,        10.0f,  10.0f,      0.0f, -1.0f, 0.0f,    // vértice 3 (x,y,z, u,v, nx,ny,nz)
        
        -1.0, 2.0f, -1.0f,         0.0f,   0.0f,      0.0f, -1.0f, 0.0f,    // vértice 4 (x,y,z, u,v, nx,ny,nz)
         1.0, 2.0f, -1.0f,        10.0f,   0.0f,      0.0f, -1.0f, 0.0f,    // vértice 5 (x,y,z, u,v, nx,ny,nz)
        -1.0, 2.0f,  1.0f,         0.0f,  10.0f,      0.0f, -1.0f, 0.0f,    // vértice 6 (x,y,z, u,v, nx,ny,nz)
         1.0, 2.0f,  1.0f,        10.0f,  10.0f,      0.0f, -1.0f, 0.0f,    // vértice 7 (x,y,z, u,v, nx,ny,nz)
    };

    unsigned int indices[] = {
        0,1,2,  // quadrado de cima
        1,2,3,
        
        4,5,6,  // quadrado de baixo
        5,6,7,
        
        3,1,7,  // quadrado da direita
        1,7,5,
        
        2,0,6,  // quadrado da esquerda
        0,6,4,
        
        2,3,6,  // quadrado da frente
        3,6,7,
        
        0,1,4,  // quadrado de trás
        1,4,5
        
    };

    GLfloat floorVertices[] = {
        -10.0, 0.0f, -10.0f,         0.0f,   0.0f,      0.0f, -1.0f, 0.0f,
         10.0, 0.0f, -10.0f,        10.0f,   0.0f,      0.0f, -1.0f, 0.0f,
        -10.0, 0.0f,  10.0f,         0.0f,  10.0f,      0.0f, -1.0f, 0.0f,
         10.0, 0.0f,  10.0f,        10.0f,  10.0f,      0.0f, -1.0f, 0.0f,
    };

    unsigned int floorIndices[] = {
        0, 2, 1,
        1, 2, 3
    };

    calcAverageNormal(indices, 36, vertices, 64, 8, 5);

    mesh* obj1 = new mesh();
    obj1->CreateMesh(vertices, indices, 64, 36);
    meshList.push_back(obj1);

    mesh* obj2 = new mesh();
    obj2->CreateMesh(vertices, indices, 64, 36);
    meshList.push_back(obj2);

    mesh* obj3 = new mesh();
    obj3->CreateMesh(floorVertices, floorIndices, 64, 36);
    meshList.push_back(obj3);
}

void CreateShader() {
    shader* shader1 = new shader();
    shader1->CreateFromFile(vertexLocation, fragmentLocation);
    shaderList.push_back(*shader1);
}

int main() {
    mainWindow = Window(1366, 768);
    mainWindow.initialize();

    CreateTriangle();
    CreateShader();
    
    camera = Camera(glm::vec3 (0.0f, 0.0f, 0.0f), glm::vec3 (0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 8.0f);
    
    metalMaterial = material(1.0f, 32.0f);
    woodMaterial = material(0.3f, 4.0f);

    // luz principal
    mainLight = directionalLight(1.0f,  1.0f, 1.0f,     // rgb
                                 0.3f,  0.4f,           // ambient | diffuse
                                -8.0f, -8.0f, 2.0f);    // x,y,z

    // point light yellow rgb(242, 203, 1)
    unsigned int pointLightCount = 0;
    pointLights[0] = pointLight(0.94f, 0.79f, 0.0f,     // rgb
                                 0.5f,  1.0f,           // ambient | diffuse
                                -2.0f,  1.0f, -2.0f,    // x,y,z
                                 0.3f,  0.2f, 0.1f);    // const, linear, exp
    pointLightCount++;
    pointLights[1] = pointLight(0.94f, 0.79f, 0.0f,     // rgb
                                 0.5f,  1.0f,           // ambient | diffuse
                                 2.0f,  1.0f, -2.0f,    // x,y,z
                                 0.3f,  0.2f, 0.1f);    // const, linear, exp
    pointLightCount++;
    pointLights[2] = pointLight(0.94f, 0.79f, 0.0f,     // rgb
                                 0.5f,  1.0f,           // ambient | diffuse
                                 0.0f,  1.0f, -2.0f,    // x,y,z
                                 0.3f,  0.2f, 0.1f);    // const, linear, exp
    pointLightCount++;

    // spot light
    unsigned int spotLightCount = 0;
    spotLights[0] = spotLight(1.0f,  1.0f, 1.0f,            // rgb
                              0.5f,  0.5f,                  // ambient | diffuse
                              0.0f,  1.0f, 5.0f,            // position (x,y,z)
                              0.0f, -1.0f, 0.0f,            // direction (x,y,z)
                              0.3f,  0.2f, 0.1f, 20.0f);    // const, linear, exp, Edge
    spotLightCount++;

    blockTexture = texture((char*) ("/Users/anathayna/Documents/docs/senac-bcc/cg/block/block/texture/block.png"));
    blockTexture.loadTexture();
    brickTexture = texture((char*)("/Users/anathayna/Documents/docs/senac-bcc/cg/block/block/texture/brick.png"));
    brickTexture.loadTexture();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

    while (!mainWindow.getWindowShouldClose()) {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastime;
        lastime = now;

        // ativa inputs e eventos
        glfwPollEvents();

        // controle do teclado
        camera.keyControl(mainWindow.getKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange(), deltaTime);

        /********************************
        * cor de fundo da tela
        *********************************/
        glClearColor(0.36f, 0.57f, 0.99f, 1.0f); // sky rgb(93, 147, 253)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /********************************
        * cubo
        *********************************/
        shaderList[0].UseProgram();
        glUniformMatrix4fv(shaderList[0].getUniformProjection(), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(shaderList[0].getUniformView(), 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        
            /********************************
            * luzes
            *********************************/
            shaderList[0].setDirectionalLight(&mainLight);
            shaderList[0].setPointLight(pointLights, pointLightCount);
            
            // atualiza a posição da lanterna
            spotLights[0].SetFlash(camera.getCameraPosition(), camera.getCameraDirection());
            shaderList[0].setSpotLight(spotLights, spotLightCount);

            /********************************
            * model
            *********************************/
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));

            /********************************
            * cubo
            *********************************/
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-1.0f, 3.0f, -1.5f));
            glUniformMatrix4fv(shaderList[0].getUniformModel(), 1, GL_FALSE, glm::value_ptr(model));
            blockTexture.useTexture();
            metalMaterial.useMaterial(shaderList[0].getUniformSpecularIntensity(), shaderList[0].getUniformShininess());
            meshList[1]->RenderMesh();

            /********************************
            * chão
            *********************************/
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
            glUniformMatrix4fv(shaderList[0].getUniformModel(), 1, GL_FALSE, glm::value_ptr(model));
            brickTexture.useTexture();
            woodMaterial.useMaterial(shaderList[0].getUniformSpecularIntensity(), shaderList[0].getUniformShininess());
            meshList[2]->RenderMesh();
        
        glUseProgram(0); // removo da memória

        // atualiza a tela
        mainWindow.swapBuffers();
    }

    return 0;
}

