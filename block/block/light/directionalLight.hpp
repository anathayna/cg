//
//  directionalLight.hpp
//  block
//
//  Created by ana thayna on 26/11/20.
//

#ifndef directionalLight_hpp
#define directionalLight_hpp

#pragma once
#include <stdio.h>
#include "light.hpp"

class directionalLight : public light {
public:
    directionalLight();
    directionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
        GLfloat xDir, GLfloat yDir, GLfloat zDir);
    void useLigth(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
                  GLuint diffuseIntensityLocation, GLuint directionLocation);
    ~directionalLight();
private:
    glm::vec3 direction;
};

#endif /* directionalLight_hpp */
