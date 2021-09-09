#include "Shader.h"

Shader::Shader() {
    shaderId = 0;
    uniformModel = 0;
    uniformProjection = 0;
    uniformView = 0;

    pointLightCount = 0;
}

Shader::~Shader() {
    clearShader();
}

void Shader::createFromString(const char* vertexCode, const char* fragmentCode) {
    compileShader(vertexCode, fragmentCode);
}

void Shader::createFromFiles(const char* vertexLocation, const char* fragmentLocation) {
    std::string vertexString = readFile(vertexLocation);
    std::string fragmentString = readFile(fragmentLocation);
    const char* vertexCode = vertexString.c_str();
    const char* fragmentCode = fragmentString.c_str();

    compileShader(vertexCode, fragmentCode);
}

std::string Shader::readFile(const char* fileLocation) {
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if (!fileStream.is_open()) {
        printf("Failed to read %s! File doesn't exist!", fileLocation);
        return "";
    }

    std::string line = "";
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

GLuint Shader::getProjectionLocation() {
    return uniformProjection;
}

GLuint Shader::getModelLocation() {
    return uniformModel;
}

GLuint Shader::getViewLocation() {
    return uniformView;
}

GLuint Shader::getAmbientColorLocation() {
    return uniformDirctionalLight.uniformColor;
}

GLuint Shader::getAmbientIntesityLocation() {
    return uniformDirctionalLight.uniformAmbientIntensity;
}

GLuint Shader::getDiffuseIntensityLocation() {
    return uniformDirctionalLight.uniformDiffuseIntensity;
}
    
GLuint Shader::getDirectionLocation() {
    return uniformDirctionalLight.uniformDirection;
}

GLuint Shader::getSpecularIntensityLocation() {
    return uniformSpecularIntensity;
}

GLuint Shader::getShininessLocation() {
    return uniformShininess;
}

GLuint Shader::getEyePositionLocation() {
    return uniformEyePosition;
}

void Shader::setDirectionalLight(DirectionalLight* dLight) {
    dLight->useLight(uniformDirctionalLight.uniformAmbientIntensity, uniformDirctionalLight.uniformColor,
        uniformDirctionalLight.uniformDiffuseIntensity, uniformDirctionalLight.uniformDirection);
}

void Shader::setPointLights(PointLight* pLights, unsigned int lightsCount) {
    if (lightsCount > MAX_POINT_LIGHTS) {
        lightsCount = MAX_POINT_LIGHTS;
    }

    glUniform1i(uniformPointLightCount, lightsCount);
    for (size_t i = 0; i < lightsCount; i++) {
        pLights[i].useLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColor,
                            uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
                            uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
    }
}

void Shader::setSpotLights(SpotLight* spotLights, unsigned int lightsCount) {
    if (lightsCount > MAX_SPOT_LIGHTS) {
        lightsCount = MAX_SPOT_LIGHTS;
    }

    glUniform1i(uniformSpotLightCount, lightsCount);
    for (size_t i = 0; i < lightsCount; i++) {
        spotLights[i].useLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColor,
                            uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection,
                            uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent,
                            uniformSpotLight[i].uniformEyeSight);
    }
}


void Shader::useShader() {
    glUseProgram(shaderId);
}

void Shader::clearShader() {
    if (shaderId != 0) {
        glDeleteProgram(shaderId);
        shaderId = 0;
    }
    
    uniformModel = 0;
    uniformProjection = 0;
}

void Shader::compileShader(const char* vertexCode, const char* fragmentCode) {
    shaderId = glCreateProgram();

    if (!shaderId) {
        printf("Error creating shader program!\n");
        return;
    }

    addShader(shaderId, vertexCode, GL_VERTEX_SHADER);
    addShader(shaderId, fragmentCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderId);
    glGetProgramiv(shaderId, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(shaderId);
    glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }

    uniformModel = glGetUniformLocation(shaderId, "model");
    uniformProjection = glGetUniformLocation(shaderId, "projection");
    uniformView  = glGetUniformLocation(shaderId, "view");
    uniformDirctionalLight.uniformColor = glGetUniformLocation(shaderId, "directionalLight.base.color");
    uniformDirctionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderId, "directionalLight.base.ambientIntensity");
    uniformDirctionalLight.uniformDirection = glGetUniformLocation(shaderId, "directionalLight.direction");
    uniformDirctionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderId, "directionalLight.base.diffuseIntensity");
    uniformSpecularIntensity = glGetUniformLocation(shaderId, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(shaderId, "material.shininess");
    uniformEyePosition = glGetUniformLocation(shaderId, "eyePosition");

    uniformPointLightCount = glGetUniformLocation(shaderId, "pointLightsCount");

    for (size_t i = 0; i < MAX_POINT_LIGHTS; i++) {
        char locBuff[100] = {'\0'};
        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].base.color", i);
        uniformPointLight[i].uniformColor = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].base.ambientIntensity", i);
        uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].base.diffuseIntensity", i);
        uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].position", i);
        uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].constant", i);
        uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].linear", i);
        uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%zu].exponent", i);
        uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderId, locBuff);
    }

    uniformSpotLightCount = glGetUniformLocation(shaderId, "spotLightsCount");
    for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++) {
        char locBuff[100] = {'\0'};
        snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].pointLight.base.color", i);
        uniformSpotLight[i].uniformColor = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].pointLight.base.ambientIntensity", i);
        uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].pointLight.base.diffuseIntensity", i);
        uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].pointLight.position", i);
        uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].direction", i);
        uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].pointLight.constant", i);
        uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].pointLight.linear", i);
        uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].pointLight.exponent", i);
        uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%zu].eyeSight", i);
        uniformSpotLight[i].uniformEyeSight = glGetUniformLocation(shaderId, locBuff);
    }
}

void Shader::addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(theProgram, theShader);
}
