//
//  mesh.hpp
//  splash
//
//  Created by Ana Thayna Franca on 10/09/20.
//  Copyright © 2020 Ana Thayna Franca. All rights reserved.
//

#ifndef mesh_hpp
#define mesh_hpp

#pragma once
#include <stdio.h>
#include <GL/glew.h>

class mesh {
public:
    mesh();
    ~mesh();
    void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
    void RenderMesh();
private:
    GLuint VAO, VBO, IBO;
    GLsizei indexCount;
};

#endif /* mesh_hpp */
