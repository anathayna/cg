//
//  light.cpp
//  block
//
//  Created by ana thayna on 26/11/20.
//

#include "light.hpp"

light::light() {
    //ambiente
    color = glm::vec3(1.0f, 1.0f, 1.0f);
    ambientIntensity = 1.0f;

    //difuse
    diffuseIntensity = 0.0f;
}

light::light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity) {
    //ambiente
    color = glm::vec3(red, green, blue);
    ambientIntensity = aIntensity;

    //difuse
    diffuseIntensity = dIntensity;
}

light::~light() {}
