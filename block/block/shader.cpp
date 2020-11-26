//
//  shader.cpp
//  block
//
//  Created by ana thayna on 26/11/20.
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
    std::string vertexString = ReadFile(vertexLocation);
    std::string fragmentString = ReadFile(fragmentLocation);
    
    const char* vertexCode = vertexString.c_str();
    const char* fragmentCode = fragmentString.c_str();

    CompileShader(vertexCode, fragmentCode);
}

std::string shader::ReadFile(const char* fileLocation) {
    std::string content = "";
    std::ifstream fileStrem(fileLocation, std::ios::in);

    if (!fileStrem.is_open()) {
        printf(" erro ao tentar abrir o arquivo %s", fileLocation);
        return "";
    }

    std::string line;
    while (!fileStrem.eof()) {
        std::getline(fileStrem, line);
        content.append(line + '\n');
    }
    fileStrem.close();
    return content;
}


void shader::CompileShader(const char* vertexCode, const char* fragmentCode) {
    
    shaderID = glCreateProgram();
    if (!shaderID) {
        printf(" erro ao criar o programa");
        return;
    }

    AddShader(GL_VERTEX_SHADER, vertexCode);
    AddShader(GL_FRAGMENT_SHADER, fragmentCode);
    
    glLinkProgram(shaderID);

    GLint result = 0;
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result) {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(shaderID, sizeof(log), NULL, log);
        printf(" erro ao linkar o programa: '%s'\n", log);
        return;
    }

    uniformProjection = glGetUniformLocation(shaderID, "projection");
    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformView = glGetUniformLocation(shaderID, "view");
    
    uniformDirectionalLight.uniformAmbientColor = glGetUniformLocation(shaderID, "directionalLight.base.color");
    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
    
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
    
    uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(shaderID, "material.shininess");

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

    GLuint shader = glCreateShader(shaderType);

    const GLchar* code[1];
    code[0] = shaderCode;

    glShaderSource(shader, 1, code, NULL);
    glCompileShader(shader);

    GLint result = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result) {
        GLchar log[1024] = { 0 };
        glGetShaderInfoLog(shader, sizeof(log), NULL, log);
        printf(" erro ao compilar o %d shader '%s'\n", shaderType, log);
        return;
    }

    glAttachShader(shaderID, shader); 
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
