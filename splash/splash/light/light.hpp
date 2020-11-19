//
//  light.hpp
//  splash
//
//  Created by ana thayna on 19/11/20.
//  Copyright © 2020 Ana Thayna Franca. All rights reserved.
//

#ifndef light_hpp
#define light_hpp

#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

class light {
public:
    light();
    light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);
    ~light();

protected:
    //ambiente
    glm::vec3 color;
    GLfloat ambientIntensity;

    //diffuse
    GLfloat diffuseIntensity;
};

#endif /* light_hpp */
