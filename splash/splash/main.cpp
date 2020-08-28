//
//  main.cpp
//  splash
//
//  Created by Ana Thayna Franca on 20/08/20.
//  Copyright © 2020 Ana Thayna Franca. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, pShader;

static const char* vShader = "                   \n\
#version 330                                     \n\
                                                 \n\
layout(location=0) in vec2 pos;                  \n\
                                                 \n\
void main(){                                     \n\
  gl_Position =  vec4(pos.x, pos.y, 0.0, 1.0);   \n\
}                                                \n\
";

static const char* fShader = "                   \n\
#version 330                                     \n\
                                                 \n\
out vec4 color;                                  \n\
                                                 \n\
void main(){                                     \n\
  color =  vec4(1.0, 0.0, 0.0, 1.0);             \n\
}                                                \n\
";

void CreateTriangle() {
    GLfloat vertices[] = {
    //    X       Y
        -1.0f,  -1.0f,  //vértice 1 (x,y)
         1.0f,  -1.0f,  //vértice 1 (x,y)
         0.0f,   1.0f   //vértice 1 (x,y)
    };

    //VAO
    glGenVertexArrays(1, &VAO); //Gera um VAO ID
    glBindVertexArray(VAO); //Atribui o ID ao VAO

        //Carrega os dados do vértice para a memória da placa de vídeo
        //Vertex Buffer Object: VBO
        glGenBuffers(1, &VBO); //Gera um VBO ID
        glBindBuffer(GL_ARRAY_BUFFER, VBO); //Atribui o ID ao VBO
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Atribuindo os valores do vertice para o VBO
    
                    //GL_STATIC_DRAW: Utilizado com desenhos que são fixos
                    //GL_DINAMIC_DRAW: Utilizado com desenhos que podem ter alterações durante a apresentação
                    //GL_STREAM_DRAW: Desenho utilizado somente 1 frame

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); //0:  shader location | 3: numero de valores de vertice (x,y,z)
                                                                   //GL_FLOAT: Tipo de dados | GL_FALSE: normalizado | 0: pular elemento (cor)
                                                                   //0: offset
            glEnableVertexAttribArray(0); //0:  shader location

        glBindBuffer(GL_ARRAY_BUFFER, 0); //remove o VBO da memÛria
    glBindVertexArray(0); //remove o VAO da memÛria
}

void AddShader(GLenum shaderType, const char* shaderCode) {
    //começando a compilar cada shader
    //1. criar um shader
    GLuint shader = glCreateShader(shaderType);
    
    const GLchar* code[1];
    code[0] = shaderCode;

    //2. atribui o code do GLSL p/ o shader
    //2.1 guarda e converte a variável char em GLchar
    //2.2 anexa o code ao shader
    glShaderSource(shader, 1, code, NULL); //1. número de códigos que estão sendo enviados | NULL: final da string, termina code
    
    //3. compila o shader
    glCompileShader(shader);
    
    //4. tratamentos de erros de compilação
    GLint result = 0;
    glGetShaderiv(shader , GL_COMPILE_STATUS, &result); //coloca o valor do status da compilação na variável result
    if(!result) {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(shader, sizeof(log), NULL, log); //busca texto caso ocorra algum erro na compilação
        printf("erro ao compilar %d shader '%s'", shaderType, log);
        return;
    }
    
    //5. anexa o shader ao programa
    glAttachShader(pShader, shader); //variável global
}

void CompileShader() {
    //1. criar programa
    pShader = glCreateProgram(); //Inicia o programa
    if (!pShader) {
        printf("Erro ao criar o programa");
        return;
    }

    //2. compila o vertex shader
    AddShader(GL_VERTEX_SHADER, vShader);
    //3. compila o fragment shader
    AddShader(GL_FRAGMENT_SHADER, fShader);

    //4. cria o link do programa
    glLinkProgram(pShader); //Programa
    
    //5. validação do link do programa
    GLint result = 0;
    glGetProgramiv(pShader , GL_LINK_STATUS, &result); //coloca o valor do status da compilação na variável result
    if(!result) {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(pShader, sizeof(log), NULL, log); //busca texto caso ocorra algum erro na compilação
        printf("erro ao linkar o programa: '%s'\n", log);
        return;
    }
    
    //6. validação do programa
    glValidateProgram(pShader);
    glGetProgramiv(pShader , GL_VALIDATE_STATUS, &result); //coloca o valor do status da compilação na variável result
    if(!result) {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(pShader, sizeof(log), NULL, log); //busca texto caso ocorra algum erro na compilação
        printf("erro ao validar o programa: '%s'\n", log);
        return;
    }
}


int main() {

    //INICIALIZAR O GLFW
    if (!glfwInit()) {
        printf("GLFW Nao foi inicializado");
        glfwTerminate();
        return 1;
    };

    //GLFW OpenGL Version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //Core Profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //Forward Functions
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Nova Janela", NULL, NULL);
    if (!mainWindow) {
        printf("GLWF nao criou a janela");
        glfwTerminate();
        return 1;
    }

    //Pegar o buffer size da largura e altura
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    //Configurando a janela principal
    glfwMakeContextCurrent(mainWindow);

    //GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Glew nao foi iniciado");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    };

    //Configurando viewport
    glViewport(0, 0, bufferWidth, bufferHeight);

    //Criar o Triangulo
    CreateTriangle(); //coloca os dados na memória da placa de vídeo
    CompileShader(); //compila shaders

    while (!glfwWindowShouldClose(mainWindow)) {
        //Ativa inputs e eventos
        glfwPollEvents();

        //Limpa a janela, cor
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //Desenhar o triangulo
        glUseProgram(pShader); //usar o programa da memória
            glBindVertexArray(VAO); //deixa na memória os links p/ serem utilizados
                glDrawArrays(GL_TRIANGLES, 0, 3); //desenha na tela, GL_TRIANGLES | 0: primeira posição | 3: qtd de vértices (-1-1, 1-1, 0,1)
            glBindVertexArray(0); //removo VAO da memória
        glUseProgram(0); //removo o programa da memória

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}
