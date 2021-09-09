# OpenGL training project

This project is a training project inspired by Udemy ["Computer Graphics with Modern OpenGL and C++"](https://www.udemy.com/course/graphics-with-modern-opengl/) course for learning better computer graphics implementation for Computer Graphics course of University of Tehran.

Here is a sample of output:

<img src="./files/preview.gif" alt="Preview" width="700"/>

The lag is from video conversion to gif.

### Structure of the code

1. `main.cpp` is the main file! (Obviously)
2. `Window` class is reponsible for creating window and handling keyboard and mouse events. I used [GLFW](https://www.glfw.org/) for this purpose.
3. `Shader` class is handling vertex and fragment shaders.
4. `Mesh` class is created for handcraft mesh objects in the code (such as two pyramids).
5. `Texture` class is handling texture appearance and loading files (Using [stb_image](https://github.com/nothings/stb) library in the lib folder)
6. `Material` is important for lighting. It will affect diffuse and specular lightings.
7. `Model` is responsible for loading models and create corresponding Mesh and Texture instances. (Using [assimp](https://www.assimp.org) library)
8. `Camera` is camera of-course :D. This class create view matrix and handle changes in position and direction.
9. `Light` is the abstract class for having a light. All other lights have been inherited from this one.
10. `DirectionalLight` is a class handling directional light such as sun, just with direction.
11. `PointLight` is a simple point light which has position instead of direction.
12. `SpotLight` has both position and direction. Also it has an eyesight attribute to handle the angle of spot light.
13. `ShadowMap` is under proccess. It will create a texture from any light to help simulating shadows.

Additional used libraries are [GLEW](http://glew.sourceforge.net/) and [GLM](https://github.com/g-truc/glm).

To run my project, just try to have dependencies installed and run `make` command (Makefile has the details of compilation).
