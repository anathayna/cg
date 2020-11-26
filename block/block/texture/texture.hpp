//
//  texture.hpp
//  block
//
//  Created by ana thayna on 26/11/20.
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
