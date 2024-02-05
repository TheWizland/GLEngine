#pragma once
#include "Loaders/Model.h"
#include "Material.h"
#include "MatrixCollection.h"
#include "VBOManager.h"
#include <GL\glew.h>
#include <vector>
#include <string>
#include "RenderFlags.h"
#include "VBOData.h"
#include <memory>

//Stores data about an individual object.
class ObjectData
{
public:
	std::unique_ptr<MatrixCollection> matrix;
	VBOData vbo;
	GLuint textureID = -1;
	GLuint heightMapID = -1;
	GLuint normalID = -1;
	Material material;
	RenderFlags flags;

	ObjectData() { matrix = std::make_unique<MatrixCollection>(); }
	void loadModel(Models::Model modelData, VBOManager* vboHandler);
	void setTexture(std::string imageName);
	void setTexture(GLuint textureID);
	void setHeightMap(std::string imageName);
	void setHeightMap(GLuint heightMapID);
	void setVBOs(VBOData vbo);
	MatrixCollection* matrices() { return matrix.get(); }
};

