#pragma once
#include "Material.h"
#include "MatrixCollection.h"
#include "RenderFlags.h"
#include "VBOData.h"
#include <GL\glew.h>
#include <memory>
#include <string>

//Stores data about an individual object.
class ObjectData
{
public:
	std::unique_ptr<MatrixCollection> matrix;
	VBOData vbo;
	GLuint textureID = -1;
	GLuint heightMapID = -1;
	GLuint normalMapID = -1;
	Material material;
	RenderFlags flags;

	ObjectData() { matrix = std::make_unique<MatrixCollection>(); }
	void setTexture(std::string imageName);
	void setTexture(GLuint textureID);
	void setHeightMap(std::string imageName);
	void setHeightMap(GLuint heightMapID);
	void setNormalMap(std::string imageName);
	void setNormalMap(GLuint normalMapID);
	void setVBOs(VBOData vbo);
	MatrixCollection* matrices() { return matrix.get(); }
};

