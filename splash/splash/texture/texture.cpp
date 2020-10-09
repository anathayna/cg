//
//  texture.cpp
//  splash
//
//  Created by Ana Thayna Franca on 08/10/20.
//  Copyright © 2020 Ana Thayna Franca. All rights reserved.
//

#include "texture.hpp"

Texture::Texture() {
    textureID = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = NULL;
}

Texture::Texture(char* fileLoc) {
    textureID = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = fileLoc;
}

void Texture::loadTexture() {
    unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
    if (!texData) {
        printf("fail to load image %s\n", fileLocation);
    }
    
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    //filtros de imagem
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_REPEAT p/ o eixo X
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //GL_REPEAT p/ o eixo Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //remover os pixels se estiver muito perto
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //remover os pixels se estiver longe
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData); //cria imagem placa de video
    glGenerateMipmap(GL_TEXTURE_2D); //cria mipmap
    
    glBindTexture(GL_TEXTURE_2D, 0); //limpa textura da memoria
    stbi_image_free(texData);
}

void Texture::clearTexture() {
    glDeleteTextures(1, &textureID);
    textureID = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = NULL;
}

Texture::~Texture() {
    clearTexture();
}
