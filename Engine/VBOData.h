#pragma once
#include <GL/glew.h>

struct VBOData
{
	GLuint vertex = -1;
	GLuint texture = -1;
	GLuint normal = -1;
	GLuint tangent = -1;
	int vertexCount = -1;
};