//
//  light.hpp
//  block
//
//  Created by ana thayna on 26/11/20.
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
    glm::vec3 color;
    GLfloat ambientIntensity;
    GLfloat diffuseIntensity;
};

#endif /* light_hpp */
