#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_CTOR_INIT // for default value of glm::mat4
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"

const float toRadians = 3.14159265f / 180.0f;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
        uniformSpecularIntensity = 0, uniformShininess = 0;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

Material shinyMaterial;
Material dullMaterial;

Model xwing;
Model blackhawk;
Model A10;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

unsigned int pointLightsCount, spotLightsCount;

GLfloat blackhawkAngle = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";

void calculateAverageNormals(unsigned int* indices, unsigned int indicesCount, GLfloat* vertices, unsigned int verticesCount, 
        unsigned int vLength, unsigned int normalOffset) {
    for (size_t i = 0; i < indicesCount; i += 3) {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;

        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in1] - vertices[in2], vertices[in1 + 1] - vertices[in2 + 1], vertices[in1 + 2] - vertices[in2 + 2]);
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

        in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
        vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
        vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
        vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
    }

    for (size_t i = 0; i < verticesCount / vLength; i += 1) {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;   
    }
}

void CreateObjects() {
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2,
    };

    GLfloat vertices[] = {
    //  X      Y      Z     U     V      Nx     Ny     Nz
        -1.0f, -1.0f,-0.6f, 0.0f, 0.0f,  0.0f,  0.0f,  0.0f,
         0.0f, -1.0f, 1.0f, 0.5f, 0.0f,  0.0f,  0.0f,  0.0f,
         1.0f, -1.0f,-0.6f, 1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
         0.0f,  1.0f, 0.0f, 0.5f, 1.0f,  0.0f,  0.0f,  0.0f,
    };

    unsigned int floorIndices[] = {
        0, 2, 1,
        1, 2, 3,
    };

    GLfloat floorVertices[] = {
        -10.0f,  0.0f, -10.0f,  0.0f,  0.0f,   0.0f, -1.0f, 0.0f,   
         10.0f,  0.0f, -10.0f,  10.0f, 0.0f,   0.0f, -1.0f, 0.0f,
        -10.0f,  0.0f,  10.0f,  0.0f,  10.0f,  0.0f, -1.0f, 0.0f,
         10.0f,  0.0f,  10.0f,  10.0f, 10.0f,  0.0f, -1.0f, 0.0f,
    };

    calculateAverageNormals(indices, 12, vertices, 32, 8, 5);

    Mesh* obj1 = new Mesh();
    obj1->createMesh(vertices, indices, 32, 12);
    meshList.push_back(obj1);

    Mesh* obj2 = new Mesh();
    obj2->createMesh(vertices, indices, 32, 12);
    meshList.push_back(obj2);

    Mesh* obj3 = new Mesh();
    obj3->createMesh(floorVertices, floorIndices, 32, 6);
    meshList.push_back(obj3);
}

void CreateShaders() {
    Shader* shader1 = new Shader();
    shader1->createFromFiles(vShader, fShader);
    shaderList.push_back(shader1);
}

void renderScene() {
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -2.5f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    brickTexture.useTexture();
    shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[0]->renderMesh();

    model = glm::mat4();
    model = glm::translate(model, glm::vec3(1.0f, 0.0f, -2.5f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    dirtTexture.useTexture();
    dullMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[1]->renderMesh();

    model = glm::mat4();
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    dirtTexture.useTexture();
    dullMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[2]->renderMesh();

    	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 10.0f));
	model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
	xwing.renderModel();

	blackhawkAngle += 0.3f;
	if (blackhawkAngle > 360.0f)
	{
		blackhawkAngle = 0.1f;
	}
	model = glm::mat4(1.0f);
	model = glm::rotate(model, -blackhawkAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-8.0f, 2.0f, 0.0f));
	model = glm::rotate(model, -20.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
	blackhawk.renderModel();
}

void renderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderList[0]->useShader();
    uniformModel = shaderList[0]->getModelLocation();
    uniformProjection = shaderList[0]->getProjectionLocation();
    uniformView = shaderList[0]->getViewLocation();
    uniformEyePosition = shaderList[0]->getEyePositionLocation();
    uniformSpecularIntensity = shaderList[0]->getSpecularIntensityLocation();
    uniformShininess = shaderList[0]->getShininessLocation();

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

    shaderList[0]->setDirectionalLight(&mainLight);
    shaderList[0]->setPointLights(pointLights, pointLightsCount);
    shaderList[0]->setSpotLights(spotLights, spotLightsCount);

    renderScene();
}

int main() {
    mainWindow = Window(1366, 768);
    mainWindow.initialise();

    CreateObjects();
    CreateShaders();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 5.0f, 0.1f);

    brickTexture = Texture((char*)"textures/brick.png");
    brickTexture.loadTextureA();

    dirtTexture = Texture((char*)"textures/dirt.png");
    dirtTexture.loadTextureA();

    plainTexture = Texture((char*)"textures/plain.png");
    plainTexture.loadTextureA();

    shinyMaterial = Material(5.0f, 256);
    dullMaterial = Material(0.3f, 4);

    xwing = Model();
	xwing.loadModel("models/x-wing.obj");

	blackhawk = Model();
	blackhawk.loadModel("models/uh60.obj");

	A10 = Model();
	A10.loadModel("models/A10.obj");


    mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 
                                0.5f, 0.1f, 
                                0.0f, -1.0f, 0.0f);
    
    pointLightsCount = 1;
    pointLights[0] = PointLight(0.0f, 0.0f, 1.0f, 
                                0.0f, 0.4f,
                                4.0f, 0.0f, 0.0,
                                0.3f, 0.2f, 0.1f);

    pointLights[1] = PointLight(0.0f, 1.0f, 0.0f, 
                                0.0f, 1.0f,
                                -4.0f, 0.0f, 0.0,
                                0.3f, 0.2f, 0.1f);

    spotLightsCount = 1;
    spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f, 
                                1.0f, 1.0f,
                                -3.0f, 2.0f, 0.0,
                                1.0f, -1.0f, 1.0f,
                                0.0f, 0.0f, 0.1f,
                                10); 

    glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

    while (!mainWindow.getShouldClose()) {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();

        camera.keyControl(mainWindow.getsKeys(), deltaTime);
        camera.mouseControl(-1.0f * mainWindow.getXChange(), -1.0f * mainWindow.getYChange()); // drag tor
        // glm::vec3 cameraPosition = camera.getCameraPosition();
        // spotLights[0].setPosition(cameraPosition.x, cameraPosition.y, cameraPosition.z);
        // glm::vec3 cameraDirection = camera.getCameraDirection();
        // spotLights[0].setDirection(cameraDirection.x, cameraDirection.y, cameraDirection.z);
        // ************************************************************************************

		renderPass(projection, camera.calculateViewMatrix());

        glUseProgram(0);

        mainWindow.swapBuffers();
    }
    glfwTerminate();
    return 0;
}
