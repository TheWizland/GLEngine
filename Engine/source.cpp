#include <GL\glew.h>
#include <GLFW\glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "shaderloader.h"

GLuint vao[1];
GLuint renderingProgram;
float rotationRadians = 0;
float translationRadians = 0;
float circleRadius = 0.75;
float scale = 1;
float offsetX = 0;
float offsetY = 0;

void updateTransform(double deltaTime) {
    translationRadians += deltaTime * 0.5;
    offsetX = circleRadius * cos(translationRadians);
    offsetY = circleRadius * sin(translationRadians);
    rotationRadians = translationRadians - 2*M_PI;
}

void updateUniform(GLuint program) {
    GLint rotationLoc = glGetUniformLocation(program, "rotationRadians");
    GLint offsetXLoc = glGetUniformLocation(program, "offsetX");
    GLint offsetYLoc = glGetUniformLocation(program, "offsetY");
    GLint scaleLoc = glGetUniformLocation(program, "scale");

    glProgramUniform1f(program, rotationLoc, rotationRadians);
    glProgramUniform1f(program, offsetXLoc, offsetX);
    glProgramUniform1f(program, offsetYLoc, offsetY);
    glProgramUniform1f(program, scaleLoc, scale);
}

void display(GLFWwindow* window, double currentTime) {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
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
    window = glfwCreateWindow(900, 900, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        exit(EXIT_FAILURE);

    glfwSwapInterval(1);
    
    //VAO
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);

    renderingProgram = createShaderProgram("shaders/vshader.glsl", "shaders/fshader.glsl");
    glUseProgram(renderingProgram);
    
    float prevTime = glfwGetTime();
    float curTime = prevTime;
    float deltaTime = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        deltaTime = curTime - prevTime;
        prevTime = curTime;
        curTime = glfwGetTime();

        glUseProgram(renderingProgram);
        updateTransform(deltaTime);
        updateUniform(renderingProgram);
        display(window, deltaTime);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}