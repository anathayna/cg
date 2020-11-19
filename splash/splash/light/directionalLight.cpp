//
//  directionalLight.cpp
//  splash
//
//  Created by ana thayna on 19/11/20.
//  Copyright © 2020 Ana Thayna Franca. All rights reserved.
//

#include "directionalLight.hpp"

directionalLight::directionalLight() : light() {
    direction = glm::vec3(0.0f, 0.0f, 0.0f);
}

directionalLight::directionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
    GLfloat xDir, GLfloat yDir, GLfloat zDir) : light(red, green, blue, aIntensity, dIntensity) {
    direction = glm::vec3(xDir, yDir, zDir);
}

void directionalLight::useLigth(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
    GLuint diffuseIntensityLocation, GLuint directionLocation) {
    //ambiente
    glUniform1f(ambientIntensityLocation, ambientIntensity); //passa via uniform o valor de Intensidade
    glUniform3f(ambientColorLocation, color.x, color.y, color.z); //passa via uniform o valor de R/G/B

    //diffuse
    glUniform1f(diffuseIntensityLocation, diffuseIntensity); //passa via uniform o valor de Intensidade
    glUniform3f(directionLocation, direction.x, direction.y, direction.z); //passa via uniform o valor de x,y,z
}

directionalLight::~directionalLight() {

}
