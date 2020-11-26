//
//  pointLight.hpp
//  block
//
//  Created by ana thayna on 26/11/20.
//

#ifndef pointLight_hpp
#define pointLight_hpp

#pragma once
#include <stdio.h>
#include "light.hpp"

class pointLight : public light {
public:
    pointLight();
    pointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
               GLfloat xPos, GLfloat yPos, GLfloat zPos,
               GLfloat con, GLfloat lin, GLfloat exp);
    ~pointLight();
    void useLigth(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
        GLuint diffuseIntensityLocation, GLuint positionLocation,
        GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

protected:
    glm::vec3 position;
    GLfloat constant, linear, exponent;
    //L/(ax^2 + bx + c)
};

#endif /* pointLight_hpp */
