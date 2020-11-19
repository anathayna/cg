//
//  pointLight.cpp
//  splash
//
//  Created by ana thayna on 19/11/20.
//  Copyright © 2020 Ana Thayna Franca. All rights reserved.
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

    //Ambiente
    glUniform1f(ambientIntensityLocation, ambientIntensity); //Passa via uniform o valor de Intensidade
    glUniform3f(ambientColorLocation, color.x, color.y, color.z); //Passa via uniform o valor de R/G/B

    //Diffuse
    glUniform1f(diffuseIntensityLocation, diffuseIntensity); //Passa via uniform o valor de Intensidade
    glUniform3f(positionLocation, position.x, position.y, position.z); //Passa via uniform o valor de x,y,z

    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(exponentLocation, exponent);
}
