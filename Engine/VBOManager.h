#pragma once
#include <GL\glew.h>
#include <vector>

class VBOManager
{
private:
	int maxUnusedVBO = 0;
	int numVBO;
	GLuint vao[1];
	GLuint* vbo;
public:
	void init(int numVBO);
	int setupVBO(std::vector<float> vals);
};

