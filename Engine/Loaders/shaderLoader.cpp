#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include "../AssetPaths.h"

namespace Shaders {
    //Most of this file is from Computer Graphics Programming in OpenGL with C++ by V. Scott Gordon and John Clevenger
    std::string readFile(const char* filePath) {
        std::string content;
        std::ifstream fileStream(filePath, std::ios::in);
        std::string line = "";
        if(fileStream.fail()) {
            std::cout << "Shaders::readFile: " << filePath << " not found." << std::endl;
            abort();
        }
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

    GLuint compileShader(const char* shaderName, GLuint shaderType)
    {
        std::string fullPath = shaderPath;
        fullPath += shaderName;
        std::string shaderSource = readFile(fullPath.c_str());
        const char* shaderArr = shaderSource.c_str();
        GLuint shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shaderArr, NULL);
        glCompileShader(shader);

        checkOpenGLError();
        GLint compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (compiled != 1) {
            std::cout << fullPath << " compilation failed" << std::endl;
            printShaderLog(shader);
        }

        return shader;
    }

    GLuint createShaderProgram(const char* vShaderPath, const char* fShaderPath) 
    {
        GLint linked;

        GLuint vShader = compileShader(vShaderPath, GL_VERTEX_SHADER);
        GLuint fShader = compileShader(fShaderPath, GL_FRAGMENT_SHADER);

        GLuint program = glCreateProgram();
        glAttachShader(program, vShader);
        glAttachShader(program, fShader);
        glLinkProgram(program);
        checkOpenGLError();
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (linked != 1) {
            std::cout << "linking failed" << std::endl;
            printProgramLog(program);
        }
        return program;
    }

    GLuint createShaderProgram(const char* vShaderPath, const char* tcsShaderPath, const char* tesShaderPath, const char* fShaderPath)
    {
        GLint linked;

        GLuint vShader = compileShader(vShaderPath, GL_VERTEX_SHADER);
        GLuint tcsShader = compileShader(tcsShaderPath, GL_TESS_CONTROL_SHADER);
        GLuint tesShader = compileShader(tesShaderPath, GL_TESS_EVALUATION_SHADER);
        GLuint fShader = compileShader(fShaderPath, GL_FRAGMENT_SHADER);

        GLuint program = glCreateProgram();
        glAttachShader(program, vShader);
        glAttachShader(program, tcsShader);
        glAttachShader(program, tesShader);
        glAttachShader(program, fShader);
        glLinkProgram(program);
        checkOpenGLError();
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (linked != 1) {
            std::cout << "linking failed" << std::endl;
            printProgramLog(program);
        }
        return program;
    }
}