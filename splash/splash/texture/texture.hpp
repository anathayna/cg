//
//  texture.hpp
//  splash
//
//  Created by Ana Thayna Franca on 08/10/20.
//  Copyright © 2020 Ana Thayna Franca. All rights reserved.
//

#ifndef texture_hpp
#define texture_hpp

#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include "stb_image.h"

class texture {
public:
    texture();
    texture(char* fileLoc);
    ~texture();

    void loadTexture();
    void useTexture();
    void clearTexture();

private:
    GLuint textureID;
    int width, height, bitDepth;
    char* fileLocation;
};

#endif /* texture_hpp */
