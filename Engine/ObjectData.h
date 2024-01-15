#pragma once
#include "MatrixCollection.h"
#include "Loaders/ModelLoader.h"
#include <GL\glew.h>
#include <vector>
#include "VBOManager.h"

//Stores data about an individual object.
class ObjectData
{
public:
	MatrixCollection matrices;
	GLuint vboVertex;
	GLuint vboTex;
	GLuint vboNormal;
	GLuint textureID;
	int vertexCount;

	void loadModel(FileLoader::ModelLoader modelData, VBOManager* vboHandler);
	void setTexture(std::string path);
};

