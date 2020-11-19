//
//  material.cpp
//  splash
//
//  Created by ana thayna on 19/11/20.
//  Copyright © 2020 Ana Thayna Franca. All rights reserved.
//

#include <stdio.h>
#include "material.hpp"

material::material() {
    specularIntensity = 0.0f;
    shininess = 0.0f;
};

material::material(GLfloat sIntensity, GLfloat shine) {
    specularIntensity = sIntensity;
    shininess = shine;
};

void material::useMaterial(GLuint specularIntensityLocation, GLuint shininessLocation) {
    glUniform1f(specularIntensityLocation, specularIntensity);
    glUniform1f(shininessLocation, shininess);
};

material::~material() {
    specularIntensity = 0.0f;
    shininess = 0.0f;
}
