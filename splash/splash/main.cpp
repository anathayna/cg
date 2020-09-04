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

//Variaveis globais
bool direction = true, sizeDirection = true, angleDirection = true; //se for true andar para a direita se for false andar para a esquerda
float triOffset = 0.0f, triOffsetMax = 0.7f, triIncrement = 0.005f;
float size = 0.4f, maxSize = 0.8f, minSize = 0.1f, sizeIncrement = 0.005f;
float angle = 0.0, maxAngle = 360.0f, minAngle = 0.0f, angleIncrement = 0.1f;

static const char* vShader = "                           \n\
#version 330                                             \n\
                                                         \n\
layout(location=0) in vec3 pos;                          \n\
                                                         \n\
uniform mat4 model;                                      \n\
                                                         \n\
void main(){                                             \n\
  gl_Position = model * vec4(pos, 1.0);                  \n\
}                                                        \n\
";

static const char* fShader = "                   \n\
#version 330                                     \n\
                                                 \n\
uniform vec3 triangleColor;                      \n\
                                                 \n\
out vec4 color;                                  \n\
                                                 \n\
void main(){                                     \n\
  color =  vec4(triangleColor, 1.0);             \n\
}                                                \n\
";

void CreateTriangle() {
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f, //VÈrtice 1 (x,y,z)
        1.0f, -1.0f, 0.0f, //VÈrtice 2 (x,y,z)
        0.0f, 1.0f, 0.0f   //VÈrtice 3 (x,y,z)
    };

    //Vertex Array Object: VAO
    glGenVertexArrays(1, &VAO); //Gera um VAO ID
    glBindVertexArray(VAO); //Atribui o ID ao VAO

        //Carrega os dados do vÈrtice para a memÛria da placa de vÌdeo
        //Vertex Buffer Object: VBO
        glGenBuffers(1, &VBO); //Gera um VBO ID
        glBindBuffer(GL_ARRAY_BUFFER, VBO); //Atribui o ID ao VBO
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Atribuindo os valores do vertice para o VBO
                //GL_STATIC_DRAW: Utilizado com desenhos que s„o fixos
                //GL_DYNAMIC_DRAW: Utilizado com desenhos que podem ter alteraÁıes durtante a apresentaÁ„o
                //GL_STREAM_DRAW: Desenho È utilizado somente 1 frame

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); //0:  shader location | 3: numero de valores de vertice (x,y,z)
                                                                   //GL_FLOAT: Tipo de dados | GL_FALSE: normalizado | 0: pular elemento (cor)
                                                                   //0: offset
            glEnableVertexAttribArray(0); //0: shader location

        glBindBuffer(GL_ARRAY_BUFFER, 0); //remove o VBO da memÛria
    glBindVertexArray(0); //remove o VAO da memÛria
}

void AddShader(GLenum shaderType, const char* shaderCode) {
    //ComeÁamos a compilar cada shader
    //1. Criar um shader
    GLuint shader = glCreateShader(shaderType);
    
    //2. Atribui o cÛdigo do GLSL para o shader
    //2.1 Guarda e converte a vari·vel char em GLchar
    const GLchar* code[1];
    code[0] = shaderCode;
    
    //2.2 Anexa o cÛdigo ao shader
    glShaderSource(shader, 1, code, NULL); //1: n˙mero de cÛdigos que est„o sendo enviados | NULL: final da string, termina o cÛdigo

    //3. Compila o shader
    glCompileShader(shader); //compila o shader

    //4. Tratamento de erros de compilaÁ„o
    GLint result = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result); //Colocar o valor do o status da compilaÁ„o na vari·vel result
    if (!result) {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(shader, sizeof(log), NULL, log); //Busca o texto caso de algum erro na compilaÁ„o
        printf("Erro ao compilar o %d shader '%s'\n", shaderType, log);
        return;
    }

    //5. Anexa o shader ao programa
    glAttachShader(pShader, shader); //Anexa shader ao programa "vari·vel global"
}

void CompileShader() {
    //1. Criar um programa
    pShader = glCreateProgram(); //Inicia o programa
    if (!pShader) {
        printf("Erro ao criar o programa");
        return;
    }

    //2. Compila o Vertex Shader
    AddShader(GL_VERTEX_SHADER, vShader);
    //3. Compila o Fragment Shader
    AddShader(GL_FRAGMENT_SHADER, fShader);

    //4. Criar o link do programa
    glLinkProgram(pShader); //Programa

    //5. ValidaÁ„o de link do programa
    GLint result = 0;
    glGetProgramiv(pShader, GL_LINK_STATUS, &result); //Colocar o valor do o status da compilaÁ„o na vari·vel result
    if (!result) {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(pShader, sizeof(log), NULL, log); //Busca o texto caso de algum erro na compilaÁ„o
        printf("Erro ao linkar o programa: '%s'\n", log);
        return;
    }

    //6. ValidaÁ„o do programa
    glValidateProgram(pShader);
    result = 0;
    glGetProgramiv(pShader, GL_VALIDATE_STATUS, &result); //Colocar o valor do o status da compilaÁ„o na vari·vel result
    if (!result) {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(pShader, sizeof(log), NULL, log); //Busca o texto caso de algum erro na compilaÁ„o
        printf("Erro ao validar o programa: '%s'\n", log);
        return;
    }
}

int main() {
    auto t_start = std::chrono::high_resolution_clock::now();
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
    CreateTriangle(); //Coloca os dados na memÛria da placa de vÌdeo
    CompileShader(); //Compila os Shaders
    
    while (!glfwWindowShouldClose(mainWindow)) {
        //Ativa inputs e eventos
        glfwPollEvents();

        /********************************
        * Cor de fundo da tela
        *********************************/
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//Limpa a janela, cor
        glClear(GL_COLOR_BUFFER_BIT);

        /********************************
        * Triangulo
        *********************************/
        //Desenhar o triangulo
        glUseProgram(pShader); //Usar o programa
        glBindVertexArray(VAO); //Deixa na memÛria os links para serem utilizados

            /********************************
            * Cor do triangulo
            *********************************/
            auto t_now = std::chrono::high_resolution_clock::now();
            float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
            
            GLint uniColor = glGetUniformLocation(pShader, "triangleColor"); //Procura a entrada chamada triangleColor
            float r = (sin(time * 4.0f) + 1.0f) / 2.0f;
            float g = 0.0f;
            float b = 0.0f;
            glUniform3f(uniColor, r, g, b);

            /********************************
            * Movimenta triangulo
            *********************************/
            //Regra para movimentaÁ„o
            if (direction) //se a direÁ„o for true, andar para direita
                triOffset += triIncrement; //incrementa valor no offset para andar para direita
            else //se a direÁ„o for false, andar para esquerda
                triOffset -= triIncrement; //decrementa o valor no offset para andar para esquerda

            if (abs(triOffset) >= triOffsetMax) //se o valor for igual ou maior que o m·ximo, mudar a direÁ„o
                direction = !direction; //inverte a direÁ„o
            
            //Regra para alteraÁ„o de tamanho
            if (sizeDirection)
                size += sizeIncrement;
            else
                size -= sizeIncrement;

            if (size >= maxSize || size <= minSize)
                sizeDirection = !sizeDirection;
            
            //Rega para calcular o angulo
            if (angleDirection)
                angle += angleIncrement;
            else
                angle -= angleIncrement;

            if (angle >= maxAngle || angle <= minAngle)
                angleDirection = !angleDirection;

            GLint uniModel = glGetUniformLocation(pShader, "model"); //Procura a entrada chamada xMove
            glm::mat4 model(1.0f); //cria uma matriz 4x4 e coloca os valores 1.0f em todas as posiÁıes
            
            //Rotate
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

            //movimentaÁ„o
            model = glm::translate(model, glm::vec3(triOffset, 0.0f ,0.0f)); //traduz o modelo para movimentar a posiÁ„o (x,y,z)

            //Tamanho
            model = glm::scale(model, glm::vec3(size, size, 1.0f));

            glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

        /********************************
        * Finaliza o triangulo na tela
        *********************************/
        glDrawArrays(GL_TRIANGLES, 0, 3); //Desenha na tela GL_TRIANGLE | 0: Primeira  | 3: Quantidade de VÈrtices (-1-1, 1,-1, 0,1)
                
        glBindVertexArray(0); //Removo o VAO da memÛria
        glUseProgram(0); //Removo o Programa da memÛria

        //Atualiza a tela
        glfwSwapBuffers(mainWindow);
    }

    return 0;
}

