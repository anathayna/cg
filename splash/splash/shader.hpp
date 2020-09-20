//
//  shader.hpp
//  splash
//
//  Created by Ana Thayna Franca on 20/09/20.
//  Copyright © 2020 Ana Thayna Franca. All rights reserved.
//

#ifndef shader_hpp
#define shader_hpp

#pragma once
#include <stdio.h>
#include <string>
#include <GL/glew.h>

#include <iostream>
#include <fstream>

class shader {
public:
    shader();
    ~shader();
    void CreateFromString(const char* vertexCode, const char* fragmentCode);
    void CreateFromFile(const char* vertexLocation, const char* fragmentLocation);
    void UseProgram();

    GLuint getUniformProjection() { return uniformProjection; };
    GLuint getUniformModel() { return uniformModel; };

private:
    GLuint shaderID, uniformProjection, uniformModel;
    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void AddShader(GLenum shaderType, const char* shaderCode);
    std::string ReadFile(const char* fileLocation);
};

#endif /* shader_hpp */
