#pragma once
#include <string>
std::string readFile(const char* path);
void printShaderLog(GLuint shader);
void printProgramLog(int prog);
bool checkOpenGLError();
GLuint createShaderProgram(const char* vShaderPath, const char* fShaderPath);