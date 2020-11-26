//
//  mesh.hpp
//  block
//
//  Created by ana thayna on 26/11/20.
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
