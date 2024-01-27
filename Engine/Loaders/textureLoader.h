#pragma once
#include <GL\glew.h>
#include <string>

namespace FileLoader {
	extern GLuint tilingMode;
	GLuint genTexture(std::string path);
	GLuint genCubeMap(std::string path, std::string extension);
}