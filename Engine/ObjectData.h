#pragma once
#include "MatrixCollection.h"
#include "Loaders/ModelLoader.h"
#include <GL\glew.h>
#include <vector>

//Stores data about an individual object.
class ObjectData
{
public:
	MatrixCollection matrices;
	GLuint vboVertex;
	GLuint vboTex;
	GLuint textureID;

	void loadModel(FileLoader::ModelLoader modelData, GLuint vboVertex, GLuint vboTex);
	void setTexture(std::string path);
};

