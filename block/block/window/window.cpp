//
//  window.cpp
//  block
//
//  Created by ana thayna on 26/11/20.
//

#include "window.hpp"

Window::Window() {
    width = 800;
    height = 600;
    for (int i = 0; i < 1024; i++) {
        keys[i] = 0;
    }
    xChange = 0.0f;
    yChange = 0.0f;
}

Window::Window(GLint windowWidth, GLint windowHeight) {
    width = windowWidth;
    height = windowHeight;

    for (int i = 0; i < 1024; i++) {
        keys[i] = 0;
    }
    xChange = 0.0f;
    yChange = 0.0f;
}

Window::~Window() {
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}

int Window::initialize() {
    if (!glfwInit()) {
        printf("GLFW nao foi inicializado");
        glfwTerminate();
        return 1;
    };

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mainWindow = glfwCreateWindow(width, height, "block", NULL, NULL);
    if (!mainWindow) {
        printf("GLFW nao criou a janela");
        glfwTerminate();
        return 1;
    }

    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    glfwMakeContextCurrent(mainWindow);
    createCallbacks();

    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Glew nao foi iniciado");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    };

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, bufferWidth, bufferHeight);
    glfwSetWindowUserPointer(mainWindow, this);

    return 0;
}

void Window::swapBuffers() {
    glfwSwapBuffers(mainWindow);
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
        return;
    }

    if (key >= 0 && key <= 1024) {
        if (action == GLFW_PRESS){
            theWindow->keys[key] = true;
            //printf("pressed: %d\n", key);
        } else if(action == GLFW_RELEASE){
            theWindow->keys[key] = false;
            //printf("released: %d\n", key);
        }
    }
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos) {
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (theWindow->mouseFirstMove) {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMove = false;
    }

    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos;

    theWindow->lastX = xPos;
    theWindow->lastY = yPos;

    //printf("x:%.6f y:%.6f\n", theWindow->xChange, theWindow->yChange);
}

void Window::createCallbacks() {
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat Window::getXChange() {
    GLfloat theChange = xChange;
    xChange = 0;
    return theChange;
}

GLfloat Window::getYChange() {
    GLfloat theChange = yChange;
    yChange = 0;
    return theChange;
}
