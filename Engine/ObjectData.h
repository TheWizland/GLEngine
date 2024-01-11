#pragma once
#include "MatrixCollection.h"
#include <GL\glew.h>
#include <vector>

//Stores data about an individual object.
class ObjectData
{
public:
	MatrixCollection matrices;
	GLuint vboVertex;

	void loadModel(std::vector<float> vertexList, GLuint vbo);
};

