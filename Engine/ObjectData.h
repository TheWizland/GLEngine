#pragma once
#include "Loaders/Model.h"
#include "Material.h"
#include "MatrixCollection.h"
#include "VBOManager.h"
#include <GL\glew.h>
#include <vector>
#include <string>

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
	Material material;

	void loadModel(Models::Model modelData, VBOManager* vboHandler);
	void setTexture(std::string path);
	void setTexture(GLuint textureID);
	void copyVBO(ObjectData object);
};

