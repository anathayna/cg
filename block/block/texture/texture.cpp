//
//  texture.cpp
//  block
//
//  Created by ana thayna on 26/11/20.
//

#include "texture.hpp"

texture::texture() {
    textureID = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = NULL;
}

texture::texture(char* fileLoc) {
    textureID = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = fileLoc;
}

void texture::loadTexture() {
    unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0); //carrega o arquivo na memória
    if (!texData) {
        printf("fail to load image %s\n", fileLocation);
    }

    glGenTextures(1, &textureID); //gera uma textura e retorna o ID
    glBindTexture(GL_TEXTURE_2D, textureID); //bind da textura na memória

        //filtros da imagem
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_REPEAT para o eixo X
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //GL_REPEAT para o eixo Y
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //remover os pixels se estiver muito perto
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //remover os pixels se estiver longe

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData); //cria a imagem na placa de vídeo
        glGenerateMipmap(GL_TEXTURE_2D); //cria os MipMap

    glBindTexture(GL_TEXTURE_2D, 0); //limpa a textura da memória
    stbi_image_free(texData); //limpa o arquivo lido da memória
}

void texture::useTexture() {
    glActiveTexture(GL_TEXTURE0); //ativa a textura no nível 0
    glBindTexture(GL_TEXTURE_2D, textureID); //bind da textura na memória
}

void texture::clearTexture() {
    glDeleteTextures(1, &textureID);
    textureID = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = NULL;
}

texture::~texture() {
    clearTexture();
}
