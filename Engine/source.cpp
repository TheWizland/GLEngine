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

#include <iostream>


GLuint vao[1];
VBOManager vboGenerator; //Needs to be constructed
Renderers::StandardRenderer textureRenderer;
Renderers::SkyboxRenderer skyboxRenderer;
int windowX = 1200;
int windowY = 900;
const char* windowTitle = "Hello World";
InputManager inputManager;
Camera camera = Camera(90.f, float(windowX) / windowY);
ObjectData skybox;
std::vector<ObjectData> objectList;
Light positionalLight;
float deltaTime = 0;
glm::vec4 globalAmbient = glm::vec4();

void init() {
    vboGenerator.init(12);
    textureRenderer.init();
    skyboxRenderer.init("./assets/Skybox/milkyway/", vboGenerator);

    FileLoader::ObjLoader objLoader("./assets/Models/cube.obj");
    ObjectData cube;
    cube.loadModel(objLoader, &vboGenerator);
    cube.setTexture("./assets/Textures/sand.jpg");
    cube.matrices.rotateY(M_PI / 4);
    objectList.push_back(cube);
    
    
    ModelGenerator::SphereGenerator sphereGen;
    ObjectData sphere;
    sphereGen.genSphere(24);
    sphere.loadModel(sphereGen, &vboGenerator);
    sphere.setTexture("./assets/Textures/rock.jpg");
    sphere.matrices.setParent(&cube.matrices);
    sphere.matrices.translate(4.f, 0.f, 0.f);
    objectList.push_back(sphere);

    ObjectData dolphin;
    dolphin.copyVBO(sphere);
    dolphin.loadModel(FileLoader::ObjLoader("./assets/Models/dolphinHighPoly.obj"), &vboGenerator);
    dolphin.setTexture("./assets/Textures/Dolphin_HighPolyUV.png");
    dolphin.matrices.setParent(&cube.matrices);
    dolphin.matrices.translate(6.f, 0.f, 0.f);
    dolphin.matrices.scale(1.75f);
    dolphin.matrices.setApplyParentRotationToPosition(false);
    objectList.push_back(dolphin);

    ObjectData lightSourceModel;
    lightSourceModel.copyVBO(sphere);
    lightSourceModel.setTexture("./assets/Textures/sunmap.jpg");
    lightSourceModel.matrices.translate(0, 2, 0);
    lightSourceModel.matrices.scale(0.1f);
    objectList.push_back(lightSourceModel);

    Light::globalAmbient = glm::vec4(0.3f, 0.3f, 0.3f, 1);
    positionalLight.position = glm::vec3(0, 2, 0);

    camera.moveForward(-8);
}

void updateTransform(float deltaTime) {
}

void display(GLFWwindow* window, double deltaTime) {
    skyboxRenderer.render(camera);

    textureRenderer.uniformCamera(camera);
    textureRenderer.uniformLight(positionalLight);
    for (ObjectData object : objectList) {
        textureRenderer.render(object);
    }
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