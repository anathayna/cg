//
//  main.cpp
//  splash
//
//  Created by Ana Thayna Franca on 20/08/20.
//  Copyright © 2020 Ana Thayna Franca. All rights reserved.
//

#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <chrono>

const GLint WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, pShader;

// variaveis globais
bool direction = true, sizeDirection = true, angleDirection = true; // se for true andar para a direita se for false andar para a esquerda
float triOffset = 0.0f, triOffsetMax = 0.7f, triIncrement = 0.005f;
float size = 0.4f, maxSize = 0.8f, minSize = 0.1f, sizeIncrement = 0.005f;
float angle = 0.0, maxAngle = 360.0f, minAngle = 0.0f, angleIncrement = 0.1f;

static const char* vShader = "                           \n\
#version 330                                             \n\
                                                         \n\
layout(location=0) in vec3 pos;                          \n\
                                                         \n\
out vec4 vCol;                                           \n\
                                                         \n\
uniform mat4 model;                                      \n\
                                                         \n\
void main(){                                             \n\
  gl_Position = model * vec4(pos, 1.0);                  \n\
  vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);             \n\
}                                                        \n\
";

static const char* fShader = "                   \n\
#version 330                                     \n\
                                                 \n\
uniform vec3 triangleColor;                      \n\
                                                 \n\
in vec4 vCol;                                    \n\
                                                 \n\
out vec4 color;                                  \n\
                                                 \n\
void main(){                                     \n\
  color =  vCol;                                 \n\
}                                                \n\
";

void CreateTriangle() {
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f, // vertice 1 (x,y,z)
        1.0f, -1.0f, 0.0f, // vertice 2 (x,y,z)
        0.0f, 1.0f, 0.0f   // vertice 3 (x,y,z)
    };

    // Vertex Array Object: VAO
    glGenVertexArrays(1, &VAO); // gera um VAO ID
    glBindVertexArray(VAO); // atribui o ID ao VAO

        // carrega os dados do vertice para a memoria da placa de video
        // Vertex Buffer Object: VBO
        glGenBuffers(1, &VBO); // gera um VBO ID
        glBindBuffer(GL_ARRAY_BUFFER, VBO); // atribui o ID ao VBO
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // atribuindo os valores do vertice para o VBO
                //GL_STATIC_DRAW: utilizado com desenhos que sao fixos
                //GL_DYNAMIC_DRAW: utilizado com desenhos que podem ter alteracoes durtante a apresentacao
                //GL_STREAM_DRAW: desenho utilizado somente 1 frame

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); //0:  shader location | 3: numero de valores de vertice (x,y,z)
                                                                   //GL_FLOAT: Tipo de dados | GL_FALSE: normalizado | 0: pular elemento (cor)
                                                                   //0: offset
            glEnableVertexAttribArray(0); //0: shader location

        glBindBuffer(GL_ARRAY_BUFFER, 0); // remove o VBO da memoria
    glBindVertexArray(0); // remove o VAO da memoria
}

void AddShader(GLenum shaderType, const char* shaderCode) {
    // comecando a compilar cada shader
    //1. criar um shader
    GLuint shader = glCreateShader(shaderType);
    
    //2. atribui o codigo do GLSL para o shader
    //2.1 guarda e converte a variavel char em GLchar
    const GLchar* code[1];
    code[0] = shaderCode;
    
    //2.2 anexa o codigo ao shader
    glShaderSource(shader, 1, code, NULL); //1: numero de codigos que estao sendo enviados | NULL: final da string, termina o codigo

    //3. compila o shader
    glCompileShader(shader); // compila o shader

    //4. tratamento de erros de compilacao
    GLint result = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result); // colocar o valor do o status da compilacao na variavel result
    if (!result) {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(shader, sizeof(log), NULL, log); // busca o texto caso de algum erro na compilacao
        printf("erro ao compilar o %d shader '%s'\n", shaderType, log);
        return;
    }

    //5. anexa o shader ao programa
    glAttachShader(pShader, shader); //anexa shader ao programa "variavel global"
}

void CompileShader() {
    //1. criar um programa
    pShader = glCreateProgram(); //inicia o programa
    if (!pShader) {
        printf("erro ao criar o programa");
        return;
    }

    //2. compila o Vertex Shader
    AddShader(GL_VERTEX_SHADER, vShader);
    //3. compila o Fragment Shader
    AddShader(GL_FRAGMENT_SHADER, fShader);

    //4. criar o link do programa
    glLinkProgram(pShader); // programa

    //5. validacao de link do programa
    GLint result = 0;
    glGetProgramiv(pShader, GL_LINK_STATUS, &result); // colocar o valor do o status da compilacao na variavel result
    if (!result) {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(pShader, sizeof(log), NULL, log); // busca o texto caso de algum erro na compilacao
        printf("erro ao linkar o programa: '%s'\n", log);
        return;
    }

    //6. validacao do programa
    glValidateProgram(pShader);
    result = 0;
    glGetProgramiv(pShader, GL_VALIDATE_STATUS, &result); // colocar o valor do o status da compilacao na variavel result
    if (!result) {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(pShader, sizeof(log), NULL, log); // busca o texto caso de algum erro na compilacao
        printf("erro ao validar o programa: '%s'\n", log);
        return;
    }
}

int main() {
    auto t_start = std::chrono::high_resolution_clock::now();
    //INICIALIZAR O GLFW
    if (!glfwInit()) {
        printf("GLFW nao foi inicializado");
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

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "splash", NULL, NULL);
    if (!mainWindow) {
        printf("GLWF nao criou a janela");
        glfwTerminate();
        return 1;
    }

    // pegar o buffer size da largura e altura
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // configurando a janela principal
    glfwMakeContextCurrent(mainWindow);

    //GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("glew nao foi iniciado");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    };

    // configurando viewport
    glViewport(0, 0, bufferWidth, bufferHeight);

    // criar o Triangulo
    CreateTriangle(); // coloca os dados na memoria da placa de video
    CompileShader(); // compila os Shaders
    
    while (!glfwWindowShouldClose(mainWindow)) {
        //ativa inputs e eventos
        glfwPollEvents();

        /********************************
        * cor de fundo da tela
        *********************************/
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);// limpa a janela, cor
        glClear(GL_COLOR_BUFFER_BIT);

        /********************************
        * triangulo
        *********************************/
        //desenhar o triangulo
        glUseProgram(pShader); //usar o programa
        glBindVertexArray(VAO); //deixa na memoria os links para serem utilizados

            /********************************
            * cor do triangulo
            *********************************/
            auto t_now = std::chrono::high_resolution_clock::now();
            float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
            
            GLint uniColor = glGetUniformLocation(pShader, "triangleColor"); //procura a entrada chamada triangleColor
            float r = (sin(time * 4.0f) + 1.0f) / 2.0f;
            float g = 0.0f;
            float b = 0.0f;
            glUniform3f(uniColor, r, g, b);

            /********************************
            * movimenta triangulo
            *********************************/
            //regra para movimentacao
            if (direction) //se a direcao for true, andar para direita
                triOffset += triIncrement; //incrementa valor no offset para andar para direita
            else //se a direcao for false, andar para esquerda
                triOffset -= triIncrement; //decrementa o valor no offset para andar para esquerda

            if (abs(triOffset) >= triOffsetMax) //se o valor for igual ou maior que o maximo, mudar a direcao
                direction = !direction; //inverte a direcao
            
            //regra para alteracao de tamanho
            if (sizeDirection)
                size += sizeIncrement;
            else
                size -= sizeIncrement;

            if (size >= maxSize || size <= minSize)
                sizeDirection = !sizeDirection;
            
            //regra para calcular o angulo
            if (angleDirection)
                angle += angleIncrement;
            else
                angle -= angleIncrement;

            if (angle >= maxAngle || angle <= minAngle)
                angleDirection = !angleDirection;

            GLint uniModel = glGetUniformLocation(pShader, "model"); //procura a entrada chamada xMove
            glm::mat4 model(1.0f); //cria uma matriz 4x4 e coloca os valores 1.0f em todas as posicoes
            
            //rotate
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

            //movimentacao
            model = glm::translate(model, glm::vec3(triOffset, 0.0f ,0.0f)); //traduz o modelo para movimentar a posicao (x,y,z)

            //tamanho
            model = glm::scale(model, glm::vec3(size, size, 1.0f));

            glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

        /********************************
        * finaliza o triangulo na tela
        *********************************/
        glDrawArrays(GL_TRIANGLES, 0, 3); //desenha na tela GL_TRIANGLE | 0: primeira  | 3: quantidade de vertices (-1-1, 1,-1, 0,1)
                
        glBindVertexArray(0); //removo o VAO da memoria
        glUseProgram(0); //removo o programa da memoria

        //atualiza a tela
        glfwSwapBuffers(mainWindow);
    }

    return 0;
}

