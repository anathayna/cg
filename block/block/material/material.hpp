//
//  material.hpp
//  block
//
//  Created by ana thayna on 26/11/20.
//

#ifndef material_hpp
#define material_hpp

#pragma once
#include <stdio.h>
#include <GL/glew.h>

class material {
public:
    material();
    material(GLfloat sIntensity, GLfloat shine);
    void useMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);
    ~material();

private:
    GLfloat specularIntensity;
    GLfloat shininess;
};

#endif /* material_hpp */
