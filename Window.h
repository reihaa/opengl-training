#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:
    Window();
    Window(GLint windowWidth, GLint windowHeight);
    ~Window();

    int initialise();

    GLfloat getBufferWidth () { return bufferWidth;  }
    GLfloat getBufferHeight() { return bufferHeight; }

    bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

    bool* getsKeys() { return keys; }
    GLfloat getXChange();
    GLfloat getYChange();

    void swapBuffers() { glfwSwapBuffers(mainWindow); }

private:
    GLFWwindow* mainWindow;

    GLint width, height;
    GLint bufferWidth, bufferHeight;

    bool keys[1024];

    GLfloat lastX, lastY, xChange, yChange;
    bool mouseFirstMoved, mouseButtonDown = false;

    void createCallbacks();

    static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
    static void handleMouse(GLFWwindow* window, double xPos, double yPos);
    static void handleMouseButton(GLFWwindow* window, int button, int action, int mode);
};
