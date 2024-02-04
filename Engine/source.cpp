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
const char* windowTitle = "OpenGL Window";
InputManager inputManager;
Camera* camera;
ObjectData skybox;
Light positionalLight;
SceneData defaultScene;
float deltaTime = 0;
glm::vec4 globalAmbient = glm::vec4();
CameraMouse cameraHandler;

void init() {
    vboGenerator.init(15);
    textureRenderer.init();
    skyboxRenderer.init();

    defaultScene.genSkybox("milkyway", ".jpg", vboGenerator);

    ObjectData* cube = defaultScene.genObject();
    cube->loadModel(Models::loadObj("cube.obj"), &vboGenerator);
    cube->setTexture("sand.jpg");
    cube->matrices.translate(0, 3, 0);
    cube->matrices.rotateY((float)M_PI / 4);

    Models::Model tile = Models::genTile();
    //tile.applyTiling(5);
    ObjectData* terrain = defaultScene.genObject();
    terrain->loadModel(Models::loadObj("tile.obj"), &vboGenerator);
    terrain->setTexture("sand.jpg");
    Models::tilingMode = GL_CLAMP_TO_EDGE;
    terrain->setHeightMap("height.png");
    Models::tilingMode = GL_REPEAT;
    terrain->matrices.translate(0, -4, 0);
    terrain->matrices.scale(10, 1, 10);
    terrain->material.shininess = 10;
    terrain->material.specular = glm::vec4(0.1, 0.1, 0.1, 1);
    
    ObjectData* sphere = defaultScene.genObject();
    sphere->loadModel(Models::genSphere(24), &vboGenerator);
    sphere->setTexture("rock.jpg");
    sphere->matrices.setParent(&cube->matrices);
    sphere->matrices.translate(4.f, 0.f, 0.f);

    ObjectData* sphere2 = defaultScene.genObject();
    sphere2->copyVBO(*sphere);
    sphere2->setTexture(sphere->textureID);
    sphere2->matrices.setParent(&sphere->matrices);
    sphere2->matrices.translate(0, -2, 0);
    sphere2->matrices.scale(0.5f);
    
    ObjectData* dolphin = defaultScene.genObject();
    dolphin->loadModel(Models::loadObj("dolphinHighPoly.obj"), &vboGenerator);
    dolphin->setTexture("Dolphin_HighPolyUV.png");
    dolphin->matrices.setParent(&cube->matrices);
    dolphin->matrices.translate(6.f, 0.f, 0.f);
    dolphin->matrices.scale(1.75f);
    dolphin->matrices.setApplyParentRotationToPosition(false);

    defaultScene.deleteObject(cube);
    dolphin->matrices.translate(0.f, -1.f, 0.f);

    ObjectData* lightSourceModel = defaultScene.genObject();
    lightSourceModel->copyVBO(*sphere2);
    lightSourceModel->setTexture("sunmap.jpg");
    lightSourceModel->matrices.translate(0, 2, 0);
    lightSourceModel->matrices.scale(0.1f);
    lightSourceModel->material.specular = glm::vec4(0, 0, 0, 1);
    lightSourceModel->flags.internallyLit = true;

    Light::globalAmbient = glm::vec4(0.1f, 0.1f, 0.1f, 1);
    positionalLight.position = glm::vec3(0, 2, 0);
    
    
    camera = defaultScene.newCamera(90.f, float(windowX) / windowY);
    camera->moveForward(-8);

    sphere->matrices.translate(4.f, 0.f, 0.f);
    //printf("%d\n", sphere->matrices)
}

void updateTransform(float deltaTime) {
}

void display(GLFWwindow* window, double deltaTime) {
    skyboxRenderer.render(*defaultScene.getSkybox(), *camera);

    textureRenderer.uniformCamera(*defaultScene.getCamera());
    textureRenderer.uniformLight(positionalLight);
    for (auto const& object : defaultScene.getObjectList()) {
        textureRenderer.render(*object);
    }
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
    
    float prevTime = (float)glfwGetTime();
    float curTime = prevTime;
    deltaTime = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        deltaTime = curTime - prevTime;
        prevTime = curTime;
        curTime = (float)glfwGetTime();
  
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