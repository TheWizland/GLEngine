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

#include "shaderloader.h"
#include "Camera.h"
#include <iostream>

GLuint vao[1];
const int numVBO = 2;
GLuint vbo[numVBO];
GLuint renderingProgram;
int windowX = 900;
int windowY = 900;
Camera camera = Camera(90.f, float(windowX)/windowY);
const char* windowTitle = "Hello World";
glm::mat4 model;


float cube[] = { -1.0f,  1.0f, -1.0f,        -1.0f, -1.0f, -1.0f,        1.0f, -1.0f, -1.0f, //Back-Bottom
            1.0f, -1.0f, -1.0f,         1.0f,  1.0f, -1.0f,         -1.0f,  1.0f, -1.0f, //Back-Top
            1.0f, -1.0f, -1.0f,         1.0f, -1.0f,  1.0f,         1.0f,  1.0f, -1.0f, //Right-Bottom
            1.0f, -1.0f,  1.0f,         1.0f,  1.0f,  1.0f,         1.0f,  1.0f, -1.0f, //Right-Top
            1.0f, -1.0f,  1.0f,         -1.0f, -1.0f,  1.0f,        1.0f,  1.0f,  1.0f, //Front-Bottom
            -1.0f, -1.0f,  1.0f,        -1.0f,  1.0f,  1.0f,        1.0f,  1.0f,  1.0f, //Front-Top
            -1.0f, -1.0f,  1.0f,        -1.0f, -1.0f, -1.0f,        -1.0f,  1.0f,  1.0f, //Left-Bottom
            -1.0f, -1.0f, -1.0f,        -1.0f,  1.0f, -1.0f,        -1.0f,  1.0f,  1.0f, //Left-Top
            -1.0f, -1.0f,  1.0f,        1.0f, -1.0f,  1.0f,         1.0f, -1.0f, -1.0f, //Bottom
            1.0f, -1.0f, -1.0f,         -1.0f, -1.0f, -1.0f,        -1.0f, -1.0f,  1.0f, //Bottom
            -1.0f,  1.0f, -1.0f,        1.0f,  1.0f, -1.0f,         1.0f,  1.0f,  1.0f, //Top
            1.0f,  1.0f,  1.0f,         -1.0f,  1.0f,  1.0f,        -1.0f,  1.0f, -1.0f //Top
};

void init() {
    model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(0.f, -2.f, 0.f));

    camera.moveForward(-8);
}

void updateTransform(float deltaTime) {
}

void updateUniform(GLuint program) {
    GLuint mMatLoc, vMatLoc, pMatLoc;
    mMatLoc = glGetUniformLocation(program, "m_matrix");
    glUniformMatrix4fv(mMatLoc, 1, GL_FALSE, glm::value_ptr(model));

    vMatLoc = glGetUniformLocation(program, "v_matrix");
    glUniformMatrix4fv(vMatLoc, 1, GL_FALSE, glm::value_ptr(camera.getView()));

    pMatLoc = glGetUniformLocation(program, "proj_matrix");
    glUniformMatrix4fv(pMatLoc, 1, GL_FALSE, glm::value_ptr(camera.getPerspective()));
    
    //glProgramUniform4fv(program, vMatLoc, 1, glm::value_ptr(camera.getView()));
    //glProgramUniform4fv(program, pMatLoc, 1, glm::value_ptr(camera.getPerspective()));
}

void display(GLFWwindow* window, double deltaTime) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, 1);
    }
}

void initCallbacks(GLFWwindow* window) {
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

    renderingProgram = createShaderProgram("shaders/vshader.glsl", "shaders/fshader.glsl");
    
    //VAO
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBO, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

    init();
    
    float prevTime = glfwGetTime();
    float curTime = prevTime;
    float deltaTime = 0;

    //std::cout << glm::to_string(camera.getPerspective()) << std::endl;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        deltaTime = curTime - prevTime;
        prevTime = curTime;
        curTime = glfwGetTime();

        glUseProgram(renderingProgram);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        updateTransform(deltaTime);
        updateUniform(renderingProgram);
        display(window, deltaTime);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}