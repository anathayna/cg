//
//  directionalLight.hpp
//  splash
//
//  Created by ana thayna on 19/11/20.
//  Copyright © 2020 Ana Thayna Franca. All rights reserved.
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
