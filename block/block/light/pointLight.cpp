//
//  pointLight.cpp
//  block
//
//  Created by ana thayna on 26/11/20.
//

#include "pointLight.hpp"

pointLight::pointLight() : light() {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    constant = 1.0f;
    linear = 0.0f;
    exponent = 0.0f;
}

pointLight::pointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
    GLfloat xPos, GLfloat yPos, GLfloat zPos,
    GLfloat con, GLfloat lin, GLfloat exp) : light(red, green, blue, aIntensity, dIntensity) {

    position = glm::vec3(xPos, yPos, zPos);
    constant = con;
    linear = lin;
    exponent = exp;
}

pointLight::~pointLight() {

}

void pointLight::useLigth(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
    GLuint diffuseIntensityLocation, GLuint positionLocation,
    GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation) {

    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform3f(ambientColorLocation, color.x, color.y, color.z);
    
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
    glUniform3f(positionLocation, position.x, position.y, position.z); 

    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(exponentLocation, exponent);
}
