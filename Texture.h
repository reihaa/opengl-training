#pragma once

#include <GL/glew.h>
#include "lib/stb_image.h"

class Texture {
public:
    Texture();
    Texture(char* fileLoc);
    ~Texture();

    bool loadTexture();
    bool loadTextureA();
    
    void useTexture();
    void clearTexture();

private:
    GLuint textureId;
    int width, height, bitDepth;

    char* fileLocation;
};