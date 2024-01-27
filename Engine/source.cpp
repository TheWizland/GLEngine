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
#include "Loaders/textureLoader.h"
#include "Camera.h"
#include "ObjectData.h"
#include "InputHandler/InputHandler.h"
#include "VBOManager.h"
#include "Light.h"
#include "Renderers/StandardRenderer.h"
#include "Renderers/SkyboxRenderer.h"
#include "SceneData.h"

#include <iostream>
#include "InputHandler/CameraMouse.h"


GLuint vao[1];
VBOManager vboGenerator; //Needs to be constructed
Renderers::StandardRenderer textureRenderer;
Renderers::SkyboxRenderer skyboxRenderer;
int windowX = 1200;
int windowY = 900;
const char* windowTitle = "Hello World";
InputManager inputManager;
Camera* camera;
ObjectData skybox;
Light positionalLight;
SceneData defaultScene;
float deltaTime = 0;
glm::vec4 globalAmbient = glm::vec4();
CameraMouse cameraHandler;
ObjectData* cube;

void init() {
    vboGenerator.init(15);
    textureRenderer.init();
    skyboxRenderer.init("milkyway", ".jpg", vboGenerator);
    defaultScene.init();

    ObjectData* cube = defaultScene.genObject();
    FileLoader::ObjLoader objLoader("cube.obj");
    cube->loadModel(objLoader, &vboGenerator);
    cube->setTexture("sand.jpg");
    cube->matrices.translate(0, 3, 0);
    cube->matrices.rotateY(M_PI / 4);

    ObjectData* terrain = defaultScene.genObject();
    objLoader.applyTiling(5);
    terrain->loadModel(objLoader, &vboGenerator);
    terrain->setTexture("sand.jpg");
    terrain->matrices.translate(0, -4, 0);
    terrain->matrices.scale(10, 1, 10);
    terrain->material.shininess = 10;
    terrain->material.specular = glm::vec4(0.1, 0.1, 0.1, 1);
    
    ObjectData* sphere = defaultScene.genObject();
    sphere->loadModel(ModelGenerator::SphereGenerator(24), &vboGenerator);
    sphere->setTexture("rock.jpg");
    sphere->matrices.setParent(&cube->matrices);
    sphere->matrices.translate(4.f, 0.f, 0.f);

    ObjectData* dolphin = defaultScene.genObject();
    dolphin->loadModel(FileLoader::ObjLoader("dolphinHighPoly.obj"), &vboGenerator);
    dolphin->setTexture("Dolphin_HighPolyUV.png");
    dolphin->matrices.setParent(&cube->matrices);
    dolphin->matrices.translate(6.f, 0.f, 0.f);
    dolphin->matrices.scale(1.75f);
    dolphin->matrices.setApplyParentRotationToPosition(false);

    defaultScene.deleteObject(cube);
    dolphin->matrices.translate(0.f, -1.f, 0.f);

    ObjectData* lightSourceModel = defaultScene.genObject();
    lightSourceModel->copyVBO(*sphere);
    lightSourceModel->setTexture("sunmap.jpg");
    lightSourceModel->matrices.translate(0, 2, 0);
    lightSourceModel->matrices.scale(0.1f);

    Light::globalAmbient = glm::vec4(0.3f, 0.3f, 0.3f, 1);
    positionalLight.position = glm::vec3(0, 2, 0);
    
    
    camera = defaultScene.newCamera(90.f, float(windowX) / windowY);
    camera->moveForward(-8);
}

void updateTransform(float deltaTime) {
}

void display(GLFWwindow* window, double deltaTime) {
    skyboxRenderer.render(*camera);
    defaultScene.render(positionalLight);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    inputManager.key_callback(window, key, scancode, action, mods);
}

void initCallbacks(GLFWwindow* window) {
    inputManager.addAction(GLFW_KEY_ESCAPE, [window](float deltaTime) -> void { glfwSetWindowShouldClose(window, 1); });

    float camSpeed = 2.5;
    inputManager.addAction(GLFW_KEY_A, [camSpeed](float deltaTime) -> void { camera->moveRight(-deltaTime * camSpeed); });
    inputManager.addAction(GLFW_KEY_D, [camSpeed](float deltaTime) -> void { camera->moveRight(deltaTime * camSpeed); });
    inputManager.addAction(GLFW_KEY_W, [camSpeed](float deltaTime) -> void { camera->moveForward(deltaTime * camSpeed); });
    inputManager.addAction(GLFW_KEY_S, [camSpeed](float deltaTime) -> void { camera->moveForward(-deltaTime * camSpeed); });
    inputManager.addAction(GLFW_KEY_Q, [camSpeed](float deltaTime) -> void { camera->moveUp(deltaTime * camSpeed); });
    inputManager.addAction(GLFW_KEY_E, [camSpeed](float deltaTime) -> void { camera->moveUp(-deltaTime * camSpeed); });
    inputManager.addAction(GLFW_KEY_RIGHT, [](float deltaTime) -> void { camera->globalYaw(deltaTime); });
    inputManager.addAction(GLFW_KEY_LEFT, [](float deltaTime) -> void { camera->globalYaw(-deltaTime); });
    inputManager.addAction(GLFW_KEY_UP, [](float deltaTime) -> void { camera->pitch(deltaTime); });
    inputManager.addAction(GLFW_KEY_DOWN, [](float deltaTime) -> void { camera->pitch(-deltaTime); });
    inputManager.addAction(GLFW_KEY_Z, [](float deltaTime) -> void { camera->roll(-deltaTime); });
    inputManager.addAction(GLFW_KEY_C, [](float deltaTime) -> void { camera->roll(deltaTime); });
    inputManager.addAction(GLFW_KEY_EQUAL, [](float deltaTime) -> void { Light::globalAmbient += deltaTime; });
    inputManager.addAction(GLFW_KEY_MINUS, [](float deltaTime) -> void { Light::globalAmbient -= deltaTime; });
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
    cameraHandler.init(window, camera);
    
    float prevTime = glfwGetTime();
    float curTime = prevTime;
    deltaTime = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        deltaTime = curTime - prevTime;
        prevTime = curTime;
        curTime = glfwGetTime();
  
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        updateTransform(deltaTime);
        cameraHandler.checkMouse(window);
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