#pragma once
#include "Loaders/Model.h"
#include "Material.h"
#include "MatrixCollection.h"
#include "VBOManager.h"
#include <GL\glew.h>
#include <vector>
#include <string>
#include "RenderFlags.h"

//Stores data about an individual object.
class ObjectData
{
public:
	MatrixCollection matrices;
	GLuint vboVertex = -1;
	GLuint vboTex = -1;
	GLuint vboNormal = -1;
	GLuint textureID = -1;
	GLuint heightMapID = -1;
	GLuint normalID = -1;
	int vertexCount = -1;
	Material material;
	RenderFlags flags;

	void loadModel(Models::Model modelData, VBOManager* vboHandler);
	void setTexture(std::string imageName);
	void setTexture(GLuint textureID);
	void setHeightMap(std::string imageName);
	void setHeightMap(GLuint heightMapID);
	void copyVBO(ObjectData object);
};

