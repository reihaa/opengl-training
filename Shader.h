#pragma once

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Shader {
public:
    Shader();
    ~Shader();

    void createFromString(const char* vertexCode, const char* fragmentCode);
    void createFromFiles(const char* vertexLocation, const char* fragmentLocation);

    std::string readFile(const char* fileLocation);

    GLuint getProjectionLocation();
    GLuint getModelLocation();
    GLuint getViewLocation();
    GLuint getAmbientIntesityLocation();
    GLuint getAmbientColorLocation();
    GLuint getDiffuseIntensityLocation();
    GLuint getDirectionLocation();
    GLuint getSpecularIntensityLocation();
    GLuint getShininessLocation();
    GLuint getEyePositionLocation();
    
    void setDirectionalLight(DirectionalLight* dLight);
    void setPointLights(PointLight* pLights, unsigned int lightsCount);
    void setSpotLights(SpotLight* spotLight, unsigned int lightsCount);

    void useShader();
    void clearShader();

private:
    int pointLightCount;
    GLuint uniformPointLightCount;

    int spotLightCount;
    GLuint uniformSpotLightCount;


    GLuint shaderId, uniformProjection, uniformModel, uniformView, uniformEyePosition,
        uniformSpecularIntensity, uniformShininess;

    struct {
        GLuint uniformColor;
        GLuint uniformAmbientIntensity; 
        GLuint uniformDiffuseIntensity; 

        GLuint uniformDirection;
    } uniformDirctionalLight;

    struct {
        GLuint uniformColor;
        GLuint uniformAmbientIntensity; 
        GLuint uniformDiffuseIntensity; 

        GLuint uniformPosition;
        GLuint uniformConstant;
        GLuint uniformLinear;
        GLuint uniformExponent;
    } uniformPointLight[MAX_POINT_LIGHTS];

    struct {
        GLuint uniformColor;
        GLuint uniformAmbientIntensity; 
        GLuint uniformDiffuseIntensity; 

        GLuint uniformPosition;
        GLuint uniformDirection;

        GLuint uniformConstant;
        GLuint uniformLinear;
        GLuint uniformExponent;

        GLuint uniformEyeSight;
    } uniformSpotLight[MAX_SPOT_LIGHTS];


    void compileShader(const char* vertexCode, const char* fragmentCode);
    void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};
