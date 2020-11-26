//
//  spotLight.hpp
//  block
//
//  Created by ana thayna on 26/11/20.
//

#ifndef spotLight_hpp
#define spotLight_hpp

#pragma once
#include <stdio.h>
#include "pointLight.hpp"

class spotLight : pointLight {
public:
    spotLight();
    spotLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
        GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat xDir, GLfloat yDir, GLfloat zDir,
        GLfloat con, GLfloat lin, GLfloat exp, GLfloat edg);
    ~spotLight();
    void useLigth(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
                  GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionalLocation,
                  GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, GLuint edgeLocation);

    void SetFlash(glm::vec3 pos, glm::vec3 dir);

private:
    glm::vec3 direction;
    GLfloat edge, procEdge;
};

#endif /* spotLight_hpp */
