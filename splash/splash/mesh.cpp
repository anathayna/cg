//
//  mesh.cpp
//  splash
//
//  Created by Ana Thayna Franca on 10/09/20.
//  Copyright © 2020 Ana Thayna Franca. All rights reserved.
//

#include "mesh.hpp"

mesh::mesh() {
    VAO = 0;
    VBO = 0;
    IBO = 0;
}

void mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices) {
    indexCount = numOfIndices;

    //Vertex Array Object: VAO
    glGenVertexArrays(1, &VAO); //gera um VAO ID
    glBindVertexArray(VAO); //atribui o ID ao VAO

        glGenBuffers(1, &IBO); //gera um IBO ID
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); //atribui o ID ao IBO
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW); //atribuindo os valores dos indices para o IBO

            //carrega os dados do vértice para a memória da placa de vídeo
            //Vertex Buffer Object: VBO
            glGenBuffers(1, &VBO); //gera um VBO ID
            glBindBuffer(GL_ARRAY_BUFFER, VBO); //atribui o ID ao VBO
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW); //atribuindo os valores do vertice para o VBO
                    //GL_STATIC_DRAW: utilizado com desenhos que são fixos
                    //GL_DYNAMIC_DRAW: utilizado com desenhos que podem ter alterações durtante a apresentação
                    //GL_STREAM_DRAW: desenho é utilizado somente 1 frame

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); //0:  shader location | 3: numero de valores de vertice (x,y,z)
                                                                       //GL_FLOAT: Tipo de dados | GL_FALSE: normalizado | 0: pular elemento (cor)
                                                                       //0: offset
                glEnableVertexAttribArray(0); //0: shader location

        glBindBuffer(GL_ARRAY_BUFFER, 0); //remove o VBO da memória
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //remove o IBO da memória
    glBindVertexArray(0); //remove o VAO da memória
}

void mesh::RenderMesh() {
    glBindVertexArray(VAO); //deixa na memória os links para serem utilizados
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); //para garantir que placas de vídeo antigas que não trabalham com Indices irão receber os indices
            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //remove o IBO da memória
    glBindVertexArray(0); //removo o VAO da memória
}

mesh::~mesh() {
    if (VAO != 0) {
        glDeleteBuffers(1, &VAO);
    }
    if (VBO != 0) {
        glDeleteBuffers(1, &VBO);
    }
    if (IBO != 0) {
        glDeleteBuffers(1, &IBO);
    }
    indexCount = 0;
}
