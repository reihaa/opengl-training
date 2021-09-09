#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "PointLight.h"


class SpotLight: public PointLight {
public:
    SpotLight();

    SpotLight(GLfloat red, GLfloat green, GLfloat blue, 
                GLfloat aIntensity, GLfloat dIntensity,
                GLfloat xPos, GLfloat yPos, GLfloat zPos,
                GLfloat xDir, GLfloat yDir, GLfloat zDir,
                GLfloat con, GLfloat lin, GLfloat exp,
                GLfloat eyeSight);

    void useLight(GLuint ambientIntensityLocation, GLuint colorLocation,
                    GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionPosition,
                    GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
                    GLuint eyeSightLocation);

    void setPosition(GLfloat xPos, GLfloat yPos, GLfloat zPos);
    void setDirection(GLfloat xDir, GLfloat yDir, GLfloat zDir);

    ~SpotLight();


private:
    glm::vec3 direction;
    
    GLfloat eyeSightAngle;
    GLfloat eyeSightAngleCos;
};
