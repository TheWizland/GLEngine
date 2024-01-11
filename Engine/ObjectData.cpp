#include "ObjectData.h"
#include <iostream>

void ObjectData::loadModel(std::vector<float> vertexList, GLuint vbo)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexList.size() * sizeof(float), &vertexList[0], GL_STATIC_DRAW);
	vboVertex = vbo;
}
