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
#include "communValues.h"

#include <stdio.h>
#include <string>
#include <GL/glew.h>

#include <iostream>
#include <fstream>

#include "directionalLight.hpp"
#include "pointLight.hpp"
#include "spotLight.hpp"

class shader {
public:
    shader();
    ~shader();
    void CreateFromString(const char* vertexCode, const char* fragmentCode);
    void CreateFromFile(const char* vertexLocation, const char* fragmentLocation);
    void UseProgram();

    void setDirectionalLight(directionalLight *dLight);
    void setPointLight(pointLight* pLight, unsigned int lightCount);
    void setSpotLight(spotLight* sLight, unsigned int lightCount);

    GLuint getUniformProjection() { return uniformProjection; };
    GLuint getUniformModel() { return uniformModel; };
    GLuint getUniformView() { return uniformView; };
    GLuint getUniformEyePosition() { return uniformEyePosition; };
    GLuint getUniformSpecularIntensity() { return uniformSpecularIntensity; };
    GLuint getUniformShininess() { return uniformShininess; };

private:
    GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition,
           uniformSpecularIntensity, uniformShininess;

    struct {
        GLuint uniformAmbientIntensity;
        GLuint uniformAmbientColor;
        GLuint uniformDiffuseIntensity;
        GLuint uniformDirection;
    } uniformDirectionalLight;

    GLuint uniformPointLightCount;
    struct {
        GLuint uniformAmbientIntensity;
        GLuint uniformAmbientColor;
        GLuint uniformDiffuseIntensity;
        GLuint uniformPosition;
        GLuint uniformConstant;
        GLuint uniformLinear;
        GLuint uniformExponent;
    } uniformPointLight[MAX_POINT_LIGHTS];

    GLuint uniformSpotLightCount;
    struct {
        GLuint uniformAmbientIntensity;
        GLuint uniformAmbientColor;
        GLuint uniformDiffuseIntensity;
        GLuint uniformPosition;
        GLuint uniformDirection;
        GLuint uniformConstant;
        GLuint uniformLinear;
        GLuint uniformExponent;
        GLuint uniformEdge;
    } uniformSpotLight[MAX_SPOT_LIGHTS];

    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void AddShader(GLenum shaderType, const char* shaderCode);
    std::string ReadFile(const char* fileLocation);
};

#endif /* shader_hpp */
