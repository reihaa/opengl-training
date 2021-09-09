#include "SpotLight.h"

SpotLight::SpotLight() : PointLight() {
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
    eyeSightAngle = 0.0f;
    eyeSightAngleCos = glm::cos(glm::radians(eyeSightAngle));
}

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue, 
                GLfloat aIntensity, GLfloat dIntensity,
                GLfloat xPos, GLfloat yPos, GLfloat zPos,
                GLfloat xDir, GLfloat yDir, GLfloat zDir,
                GLfloat con, GLfloat lin, GLfloat exp,
                GLfloat eyeSight): 
                PointLight(red, green, blue, aIntensity, dIntensity, xPos, yPos, zPos, con, lin, exp) {
    
    direction = glm::vec3(xDir, yDir, zDir);
    eyeSightAngle = eyeSight;
    eyeSightAngleCos = glm::cos(glm::radians(eyeSightAngle));
}

void SpotLight::useLight(GLuint ambientIntensityLocation, GLuint colorLocation,
                    GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionPosition,
                    GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
                    GLuint eyeSightLocation) {
    
    glUniform3f(colorLocation, color.x, color.y, color.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);

    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform3f(directionPosition, direction.x, direction.y, direction.z);
    
    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(exponentLocation, exponent);

    glUniform1f(eyeSightLocation, eyeSightAngleCos);
}

void SpotLight::setPosition(GLfloat xPos, GLfloat yPos, GLfloat zPos) {
    position = glm::vec3(xPos, yPos, zPos);
}

void SpotLight::setDirection(GLfloat xDir, GLfloat yDir, GLfloat zDir) {
    direction = glm::vec3(xDir, yDir, zDir);
}



SpotLight::~SpotLight() {}

