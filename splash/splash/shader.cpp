//
//  shader.cpp
//  splash
//
//  Created by Ana Thayna Franca on 20/09/20.
//  Copyright © 2020 Ana Thayna Franca. All rights reserved.
//

#include "shader.hpp"

shader::shader() {
    shaderID = 0;
    uniformModel = 0;
    uniformProjection = 0;
}

shader::~shader() {
    if (shaderID != 0) {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }
    uniformModel = 0;
    uniformProjection = 0;
}

void shader::UseProgram() {
    glUseProgram(shaderID);
}

void shader::CreateFromString(const char* vertexCode, const char* fragmentCode) {
    CompileShader(vertexCode, fragmentCode);
}

void shader::CreateFromFile(const char* vertexLocation, const char* fragmentLocation) {
    std::string vertexString = ReadFile(vertexLocation); //recebe os valores do arquivo
    std::string fragmentString = ReadFile(fragmentLocation); //recebe os valores do arquivo
    
    const char* vertexCode = vertexString.c_str(); //converte string para char
    const char* fragmentCode = fragmentString.c_str(); //convert string para char

    CompileShader(vertexCode, fragmentCode); //executa a compilação do código
}

std::string shader::ReadFile(const char* fileLocation) {
    std::string content = ""; //variável de retorno
    std::ifstream fileStrem(fileLocation, std::ios::in); //abre o arquivo

    if (!fileStrem.is_open()) { //se o arquivo não estiver aberto
        printf(" erro ao tentar abrir o arquivo %s", fileLocation);
        return "";
    }

    std::string line; //define uma linha
    while (!fileStrem.eof()) { //roda até o final do arquivo (End of File)
        std::getline(fileStrem, line); //adiciona a linha na variável
        content.append(line + '\n'); //adiciona a varável no retorno
    }
    fileStrem.close(); //fecha o arquivo
    return content; //retorna o conteúdo
}


void shader::CompileShader(const char* vertexCode, const char* fragmentCode) {
    //1. criar um programa
    shaderID = glCreateProgram(); //inicia o programa
    if (!shaderID) {
        printf(" erro ao criar o programa");
        return;
    }

    //2. compila o vertex shader
    AddShader(GL_VERTEX_SHADER, vertexCode);
    //3. compila o fragment shader
    AddShader(GL_FRAGMENT_SHADER, fragmentCode);

    //4. criar o link do programa
    glLinkProgram(shaderID); //Programa

    //5. Validação de link do programa
    GLint result = 0;
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result); //Colocar o valor do o status da compilação na variável result
    if (!result) {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(shaderID, sizeof(log), NULL, log); //Busca o texto caso de algum erro na compilação
        printf(" erro ao linkar o programa: '%s'\n", log);
        return;
    }
    
    //6. Validação do programa
//    glValidateProgram(shaderID);
//    result = 0;
//    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result); //colocar o valor do o status da compilação na variável result
//    if (!result) {
//        GLchar log[1024] = { 0 };
//        glGetProgramInfoLog(shaderID, sizeof(log), NULL, log); //busca o texto caso de algum erro na compilação
//        printf(" erro ao validar o programa: '%s'\n", log);
//        return;
//    }

    uniformProjection = glGetUniformLocation(shaderID, "projection"); //Procura a entrada chamada projection
    uniformModel = glGetUniformLocation(shaderID, "model"); //Procura a entrada chamada model
    uniformView = glGetUniformLocation(shaderID, "view"); //Procura a entrada chamada view
    //Luz Ambiente
    uniformDirectionalLight.uniformAmbientColor = glGetUniformLocation(shaderID, "directionalLight.base.color"); //Procura a entrada chamada directionalLight.color
    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity"); //Procura a entrada chamada directionalLight.ambientIntensity
    //Luz Diffuse
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction"); //Procura a entrada no shader
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity"); //Procura a entrada no shader
    //Luz Specular
    uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity"); //Procura a entrada no shader
    uniformShininess = glGetUniformLocation(shaderID, "material.shininess"); //Procura a entrada no shader

    //Point Light
    uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");
    for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
        char locBuff[100] = { "\0" };
        
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
        uniformPointLight[i].uniformAmbientColor = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
        uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
        uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
        uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
        uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
        uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
        uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
    }

    //Spot Light
    uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");
    for (int i = 0; i < MAX_SPOT_LIGHTS; i++) {
        char locBuff[100] = { "\0" };

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].point.base.color", i);
        uniformSpotLight[i].uniformAmbientColor = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].point.base.ambientIntensity", i);
        uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].point.base.diffuseIntensity", i);
        uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].point.position", i);
        uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i); //direção
        uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].point.constant", i);
        uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].point.linear", i);
        uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].point.exponent", i);
        uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i); //edge
        uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
    }
}

void shader::AddShader(GLenum shaderType, const char* shaderCode) {
    //começamos a compilar cada shader
    //1. criar um shader
    GLuint shader = glCreateShader(shaderType);

    //2. atribui o código do GLSL para o shader
    //2.1 guarda e converte a variável char em GLchar
    const GLchar* code[1];
    code[0] = shaderCode;

    //2.2 anexa o código ao shader
    glShaderSource(shader, 1, code, NULL); //1: número de códigos que estão sendo enviados | NULL: final da string, termina o código

    //3. compila o shader
    glCompileShader(shader); //compila o shader

    //4. tratamento de erros de compilação
    GLint result = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result); //Colocar o valor do o status da compilação na variável result
    if (!result) {
        GLchar log[1024] = { 0 };
        glGetShaderInfoLog(shader, sizeof(log), NULL, log); //Busca o texto caso de algum erro na compilação
        printf(" erro ao compilar o %d shader '%s'\n", shaderType, log);
        return;
    }

    //5. anexa o shader ao programa
    glAttachShader(shaderID, shader); //anexa shader ao programa "variável global"
}

void shader::setDirectionalLight(directionalLight* dLight) {
    dLight->useLigth(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformAmbientColor,
                     uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);

}

void shader::setPointLight(pointLight* pLight, unsigned int lightCount) {
    if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;
    glUniform1i(uniformPointLightCount, lightCount);
    for (int i = 0; i < lightCount; i++) {
        pLight[i].useLigth(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformAmbientColor,
                           uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
                           uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
    }
}

void shader::setSpotLight(spotLight* sLight, unsigned int lightCount) {
    if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;
    glUniform1i(uniformSpotLightCount, lightCount);
    for (int i = 0; i < lightCount; i++) {
        sLight[i].useLigth(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformAmbientColor,
                           uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection,
                           uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent,
                           uniformSpotLight[i].uniformEdge);
    }
}
