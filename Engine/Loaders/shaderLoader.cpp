#include <GL\glew.h>
#include <fstream>
#include <iostream>
#include <string>

namespace Shaders {
    //Most of this file is from Computer Graphics Programming in OpenGL with C++ by V. Scott Gordon and John Clevenger
    std::string readFile(const char* path) {
        std::string content;
        std::ifstream fileStream(path, std::ios::in);
        std::string line = "";
        while (!fileStream.eof()) {
            getline(fileStream, line);
            content.append(line + "\n");
        }
        fileStream.close();

        return content;
    }

    void printShaderLog(GLuint shader) {
        int len = 0;
        int chWrittn = 0;
        char* log;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        if (len > 0) {
            log = (char*)malloc(len);
            glGetShaderInfoLog(shader, len, &chWrittn, log);
            std::cout << "Shader Info Log: " << log << std::endl;
            free(log);
        }
    }

    void printProgramLog(int prog) {
        int len = 0;
        int chWrittn = 0;
        char* log;
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
        if (len > 0) {
            log = (char*)malloc(len);
            glGetProgramInfoLog(prog, len, &chWrittn, log);
            std::cout << "Program Info Log: " << log << std::endl;
            free(log);
        }
    }

    bool checkOpenGLError() {
        bool foundError = false;
        int glErr = glGetError();
        while (glErr != GL_NO_ERROR) {
            std::cout << "glError: " << glErr << std::endl;
            foundError = true;
            glErr = glGetError();
        }
        return foundError;
    }

    GLuint createShaderProgram(const char* vShaderPath, const char* fShaderPath) {
        GLint vertCompiled;
        GLint fragCompiled;
        GLint linked;

        std::string vShaderSource = readFile(vShaderPath);
        const char* vShaderArr = vShaderSource.c_str();
        std::string fShaderSource = readFile(fShaderPath);
        const char* fShaderArr = fShaderSource.c_str();

        GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(vShader, 1, &vShaderArr, NULL);
        glShaderSource(fShader, 1, &fShaderArr, NULL);

        glCompileShader(vShader);
        checkOpenGLError();
        glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
        if (vertCompiled != 1) {
            std::cout << "vertex compilation failed" << std::endl;
            printShaderLog(vShader);
        }

        glCompileShader(fShader);
        checkOpenGLError();
        glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
        if (fragCompiled != 1) {
            std::cout << "fragment compilation failed" << std::endl;
            printShaderLog(fShader);
        }

        GLuint vfProgram = glCreateProgram();
        glAttachShader(vfProgram, vShader);
        glAttachShader(vfProgram, fShader);
        glLinkProgram(vfProgram);
        checkOpenGLError();
        glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
        if (linked != 1) {
            std::cout << "linking failed" << std::endl;
            printProgramLog(vfProgram);
        }
        return vfProgram;
    }
}