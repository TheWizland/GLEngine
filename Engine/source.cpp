#include <GL\glew.h>
#include <GLFW\glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>
#include <GLM/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Loaders/shaderLoader.h"
#include "Loaders/ModelGenerator.h"
#include "Loaders/ModelLoader.h"
#include "Camera.h"
#include "ObjectData.h"
#include "InputHandler/InputHandler.h"
#include "VBOManager.h"

#include <iostream>

GLuint vao[1];
VBOManager vboGenerator; //Needs to be constructed
GLuint textureProgram;
int windowX = 1200;
int windowY = 900;
const char* windowTitle = "Hello World";
InputManager inputManager;
Camera camera = Camera(90.f, float(windowX) / windowY);
ObjectData cube, sphere;
float deltaTime = 0;

void init() {
    textureProgram = FileLoader::createShaderProgram("shaders/vshader.glsl", "shaders/fshader.glsl");
    vboGenerator.init(8);

    FileLoader::ObjLoader objLoader;
    objLoader.loadObj("./assets/cube.obj");
    cube.loadModel(objLoader, &vboGenerator);
    cube.setTexture("./assets/Textures/sand.jpg");
    cube.matrices.setLocalTranslation(glm::translate(cube.matrices.getLocalTranslation(), glm::vec3(0.f, -2.f, 0.f)));
    
    ModelGenerator::SphereGenerator sphereGen;
    sphereGen.genSphere(9);
    sphere.loadModel(sphereGen, &vboGenerator);
    sphere.setTexture("./assets/Textures/sand.jpg");
    sphere.matrices.setLocalTranslation(glm::translate(sphere.matrices.getLocalTranslation(), glm::vec3(2.f, -2.f, 0.f)));
    
    camera.moveForward(-8);
}

void updateTransform(float deltaTime) {
}

void updateUniform(GLuint program, ObjectData object) {
    GLuint mMatLoc, vMatLoc, pMatLoc;
    mMatLoc = glGetUniformLocation(program, "m_matrix");
    glUniformMatrix4fv(mMatLoc, 1, GL_FALSE, glm::value_ptr(object.matrices.getModel()));

    vMatLoc = glGetUniformLocation(program, "v_matrix");
    glUniformMatrix4fv(vMatLoc, 1, GL_FALSE, glm::value_ptr(camera.getView()));

    pMatLoc = glGetUniformLocation(program, "proj_matrix");
    glUniformMatrix4fv(pMatLoc, 1, GL_FALSE, glm::value_ptr(camera.getPerspective()));
}

void renderObject(GLuint program, ObjectData object) {
    updateUniform(program, object);

    glBindBuffer(GL_ARRAY_BUFFER, object.vboVertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, object.vboTex);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, object.textureID);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, object.vertexCount);
}

void display(GLFWwindow* window, double deltaTime) {
    renderObject(textureProgram, cube);
    renderObject(textureProgram, sphere);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    inputManager.key_callback(window, key, scancode, action, mods);
}

void initCallbacks(GLFWwindow* window) {
    inputManager.addAction(GLFW_KEY_ESCAPE, [window](float deltaTime) -> void { glfwSetWindowShouldClose(window, 1); });

    float camSpeed = 1.5;
    inputManager.addAction(GLFW_KEY_A, [camSpeed](float deltaTime) -> void { camera.moveRight(-deltaTime * camSpeed); });
    inputManager.addAction(GLFW_KEY_D, [camSpeed](float deltaTime) -> void { camera.moveRight(deltaTime * camSpeed); });
    inputManager.addAction(GLFW_KEY_W, [camSpeed](float deltaTime) -> void { camera.moveForward(deltaTime * camSpeed); });
    inputManager.addAction(GLFW_KEY_S, [camSpeed](float deltaTime) -> void { camera.moveForward(-deltaTime * camSpeed); });
    inputManager.addAction(GLFW_KEY_Q, [camSpeed](float deltaTime) -> void { camera.moveUp(deltaTime * camSpeed); });
    inputManager.addAction(GLFW_KEY_E, [camSpeed](float deltaTime) -> void { camera.moveUp(-deltaTime * camSpeed); });
    inputManager.addAction(GLFW_KEY_RIGHT, [](float deltaTime) -> void { camera.globalYaw(deltaTime); });
    inputManager.addAction(GLFW_KEY_LEFT, [](float deltaTime) -> void { camera.globalYaw(-deltaTime); });
    inputManager.addAction(GLFW_KEY_UP, [](float deltaTime) -> void { camera.pitch(deltaTime); });
    inputManager.addAction(GLFW_KEY_DOWN, [](float deltaTime) -> void { camera.pitch(-deltaTime); });
    inputManager.addAction(GLFW_KEY_Z, [](float deltaTime) -> void { camera.roll(-deltaTime); });
    inputManager.addAction(GLFW_KEY_C, [](float deltaTime) -> void { camera.roll(deltaTime); });
    glfwSetKeyCallback(window, key_callback);
}

int main(void)
{
    GLFWwindow* window;
    

    /* Initialize the library */
    if (!glfwInit())
        exit(EXIT_FAILURE);
    
    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window = glfwCreateWindow(windowX, windowY, windowTitle, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    initCallbacks(window);
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        exit(EXIT_FAILURE);

    glfwSwapInterval(1);
    
    init();
    
    float prevTime = glfwGetTime();
    float curTime = prevTime;
    deltaTime = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        deltaTime = curTime - prevTime;
        prevTime = curTime;
        curTime = glfwGetTime();

        glUseProgram(textureProgram);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        updateTransform(deltaTime);
        display(window, deltaTime);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        inputManager.checkInputs(deltaTime);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}