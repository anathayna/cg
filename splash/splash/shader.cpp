//
//  shader.cpp
//  splash
//
//  Created by Ana Thayna Franca on 20/09/20.
//  Copyright © 2020 Ana Thayna Franca. All rights reserved.
//

#include "shader.hpp"

Shader::Shader() {
    shaderID = 0;
    uniformModel = 0;
    uniformProjection = 0;
}

Shader::~Shader() {
    if (shaderID != 0) {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }
    uniformModel = 0;
    uniformProjection = 0;
}

void Shader::UseProgram() {
    glUseProgram(shaderID);
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode) {
    CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFile(const char* vertexLocation, const char* fragmentLocation) {
    std::string vertexString = ReadFile(vertexLocation); //recebe os valores do arquivo
    std::string fragmentString = ReadFile(fragmentLocation); //recebe os valores do arquivo
    
    const char* vertexCode = vertexString.c_str(); //converte string para char
    const char* fragmentCode = fragmentString.c_str(); //convert string para char

    CompileShader(vertexCode, fragmentCode); //executa a compilação do código
}

std::string Shader::ReadFile(const char* fileLocation) {
    std::string content = ""; //variável de retorno
    std::ifstream fileStrem(fileLocation, std::ios::in); //abre o arquivo

    if (!fileStrem.is_open()) { //se o arquivo não estiver aberto
        printf("erro ao tentar abrir o arquivo %s", fileLocation);
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


void Shader::CompileShader(const char* vertexCode, const char* fragmentCode) {
    //1. criar um programa
    shaderID = glCreateProgram(); //inicia o programa
    if (!shaderID) {
        printf("erro ao criar o programa");
        return;
    }

    //2. compila o Vertex Shader
    AddShader(GL_VERTEX_SHADER, vertexCode);
    //3. compila o Fragment Shader
    AddShader(GL_FRAGMENT_SHADER, fragmentCode);

    //4. criar o link do programa
    glLinkProgram(shaderID); //programa

    //5. validação de link do programa
    GLint result = 0;
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result); //colocar o valor do o status da compilação na variável result
    if (!result) {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(shaderID, sizeof(log), NULL, log); //busca o texto caso de algum erro na compilação
        printf("erro ao linkar o programa: '%s'\n", log);
        return;
    }

    //6. validação do programa
    glValidateProgram(shaderID);
    result = 0;
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result); //colocar o valor do o status da compilação na variável result
    if (!result) {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(shaderID, sizeof(log), NULL, log); //busca o texto caso de algum erro na compilação
        printf("erro ao validar o programa: '%s'\n", log);
        return;
    }

    uniformProjection = glGetUniformLocation(shaderID, "projection"); //procura a entrada chamada projection
    uniformModel = glGetUniformLocation(shaderID, "model"); //procura a entrada chamada model
}

void Shader::AddShader(GLenum shaderType, const char* shaderCode) {
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
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result); //colocar o valor do o status da compilação na variável result
    if (!result) {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(shader, sizeof(log), NULL, log); //busca o texto caso de algum erro na compilação
        printf("erro ao compilar o %d shader '%s'\n", shaderType, log);
        return;
    }

    //5. anexa o shader ao programa
    glAttachShader(shaderID, shader); //anexa shader ao programa "variável global"
}
