//
//  spotLight.cpp
//  block
//
//  Created by ana thayna on 26/11/20.
//

#include "spotLight.hpp"

spotLight::spotLight() : pointLight() {
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
    edge = 0.0f;
    procEdge = cosf(glm::radians(edge));
}

spotLight::spotLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
                     GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat xDir, GLfloat yDir, GLfloat zDir,
                     GLfloat con, GLfloat lin, GLfloat exp, GLfloat edg) :
                     pointLight (red, green, blue, aIntensity, dIntensity, xPos, yPos, zPos, con, lin, exp) {
    direction = glm::vec3(xDir, yDir, zDir);
    edge = edg;
    procEdge = cosf(glm::radians(edge));
}

spotLight::~spotLight() {

}

void spotLight::useLigth(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
                         GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionalLocation,
                         GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, GLuint edgeLocation) {
        
        glUniform1f(ambientIntensityLocation, ambientIntensity);
        glUniform3f(ambientColorLocation, color.x, color.y, color.z);


        glUniform1f(diffuseIntensityLocation, diffuseIntensity);
        glUniform3f(positionLocation, position.x, position.y, position.z);
        
        glUniform3f(directionalLocation, direction.x, direction.y, direction.z);

        glUniform1f(constantLocation, constant);
        glUniform1f(linearLocation, linear);
        glUniform1f(exponentLocation, exponent);

        glUniform1f(edgeLocation, procEdge); 
}

void spotLight::SetFlash(glm::vec3 pos, glm::vec3 dir) {
    position = pos;
    direction = dir;
}
