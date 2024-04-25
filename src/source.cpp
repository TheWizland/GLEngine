#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Loaders/shaderLoader.h"
#include "Loaders/ModelGenerator.h"
#include "Loaders/textureLoader.h"
#include "Camera.h"
#include "ObjectData.h"
#include "InputHandler/InputHandler.h"
#include "VBOManager.h"
#include "LightData/Light.h"
#include "Renderers/StandardRenderer.h"
#include "Renderers/SkyboxRenderer.h"
#include "Scenes/SceneData.h"
#include "Scenes/TestScene.h"
#include "InputHandler/CameraMouse.h"
#include "Renderers/ShadowRenderer.h"
#include "Renderers/TessellationRenderer.h"
#include "Renderers/SpriteRenderer.h"

GLuint vao[1];
VBOManager vboGenerator; //Needs to be constructed
Renderers::StandardRenderer textureRenderer;
Renderers::SkyboxRenderer skyboxRenderer;
Renderers::ShadowRenderer shadowRenderer;
Renderers::TessellationRenderer tessRenderer;
Renderers::SpriteRenderer spriteRenderer;
int windowX = 1200;
int windowY = 900;
const char* windowTitle = "OpenGL Window";
InputManager inputManager;
Camera* camera;
ObjectData skybox;
Lighting::PositionalLight* positionalLight;
ObjectData* lightSourceModel;
ObjectData terrainMoon;
ObjectData tile;
TestScene defaultScene;
float deltaTime = 0;
glm::vec4 globalAmbient = glm::vec4();
CameraMouse cameraHandler;

void init() {
    vboGenerator.init(24);
    textureRenderer.init();
    skyboxRenderer.init();
    shadowRenderer.init();
    tessRenderer.init();
    spriteRenderer.init(vboGenerator);

    double t0 = glfwGetTime();
    defaultScene.genSkybox("milkyway", ".jpg", vboGenerator);
    defaultScene.init();
    //cube->matrices()->rotateY((float)M_PI / 4);

    tile.setTexture("sand.jpg");
    //tile.matrices()->rotateX(M_PI/2);
    //tile.matrices()->translate(0,-1,0);
    
    //applyTiling(tile, 5);
    /*ObjectData* terrain = defaultScene.genObject();
    terrain->setVBOs(vboGenerator.setupVBO(Models::loadObj("tile.obj")));
    terrain->setTexture("sand.jpg");
    Models::tilingMode = GL_CLAMP_TO_EDGE;
    terrain->setHeightMap("height.png"); //Shadow mapping currently does not account for height maps.
    Models::tilingMode = GL_REPEAT;
    terrain->matrices()->translate(0, -4, 0);
    terrain->matrices()->scale(10, 1, 10);
    terrain->material.shininess = 10;
    terrain->material.specular = glm::vec4(0.1, 0.1, 0.1, 1);*/

    lightSourceModel = defaultScene.genObject();
    lightSourceModel->setVBOs(vboGenerator.setupVBO(Models::genSphere(12)));
    lightSourceModel->setTexture("sunmap.jpg");
    lightSourceModel->matrices()->translate(0, 6, 0);
    lightSourceModel->matrices()->scale(0.1f);
    lightSourceModel->material.ads.specular = glm::vec4(0, 0, 0, 1);
    lightSourceModel->flags.internallyLit = true;
    lightSourceModel->flags.hasShadows = false;

    Models::Model patch = Models::genPatch(10);
    terrainMoon.setVBOs(vboGenerator.setupVBO(patch));
    terrainMoon.setTexture("squareMoonMap.jpg");
    terrainMoon.setHeightMap("squareMoonBump.jpg");
    terrainMoon.setNormalMap("squareMoonNormal.jpg");
    terrainMoon.matrices()->translate(0, -4, 0);
    

    Lighting::globalAmbient = glm::vec4(0.1f, 0.1f, 0.1f, 1);
    positionalLight = defaultScene.newLight();
    positionalLight->setPosition(0, 6, 0);
    positionalLight->ads.diffuse = glm::vec4(1, 1, 1, 1);
    positionalLight->ads.specular = glm::vec4(1, 1, 1, 1);
    

    double t1 = glfwGetTime();
    std::cout << "Object Load Time: " << t1 - t0 << std::endl;
    
    camera = defaultScene.newCamera(90.f, float(windowX) / windowY);
    camera->moveForward(-8);

    //sphere->matrices()->translate(4.f, 0.f, 0.f);
    //printf("%d\n", sphere->matrices)
}

void updateTransform(float deltaTime) {
    glm::mat4 translation = glm::translate(glm::mat4(1.f), positionalLight->getPosition());
    lightSourceModel->matrices()->setLocalTranslation(translation);
}

void display(GLFWwindow* window, double deltaTime) {
    shadowRenderer.enableBindings();
    shadowRenderer.render(defaultScene);
    shadowRenderer.renderTessellated(terrainMoon);
    shadowRenderer.clearBindings();

    skyboxRenderer.render(*defaultScene.getSkybox(), *camera);
    textureRenderer.bindShadow(shadowRenderer.getDepthMapTexture());
    textureRenderer.render(defaultScene);
    tessRenderer.bindShadow(shadowRenderer.getDepthMapTexture());
    tessRenderer.render(terrainMoon, *camera, *positionalLight);

    spriteRenderer.uniformCamera(*camera);
    spriteRenderer.render(*tile.matrices(), tile.textureID);
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
    
    //Light Controls
    inputManager.addAction(GLFW_KEY_EQUAL, [](float deltaTime) -> void { Lighting::globalAmbient += deltaTime; });
    inputManager.addAction(GLFW_KEY_MINUS, [](float deltaTime) -> void { Lighting::globalAmbient -= deltaTime; });
    inputManager.addAction(GLFW_KEY_I, [](float deltaTime) -> void { positionalLight->translate(0, 0, deltaTime); });
    inputManager.addAction(GLFW_KEY_K, [](float deltaTime) -> void { positionalLight->translate(0, 0, -deltaTime); });
    inputManager.addAction(GLFW_KEY_J, [](float deltaTime) -> void { positionalLight->translate(deltaTime, 0, 0); });
    inputManager.addAction(GLFW_KEY_L, [](float deltaTime) -> void { positionalLight->translate(-deltaTime, 0, 0); });
    inputManager.addAction(GLFW_KEY_U, [](float deltaTime) -> void { positionalLight->translate(0, deltaTime, 0); });
    inputManager.addAction(GLFW_KEY_O, [](float deltaTime) -> void { positionalLight->translate(0, -deltaTime, 0); });

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
    //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);  
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

    float alpha = 0.1f;
    double averageFPS = 60;
    float frameTime = 0;
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        deltaTime = curTime - prevTime;
        prevTime = curTime;
        curTime = (float)glfwGetTime();

        if(deltaTime != 0)
            frameTime = (1.f / deltaTime);
        averageFPS = (alpha * frameTime) + (1.0 - alpha) * averageFPS;
        std::cout << " FPS: " << averageFPS << '\r';
  
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