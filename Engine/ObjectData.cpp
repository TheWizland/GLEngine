#include "./AssetPaths.h"
#include "Loaders/Model.h"
#include "Loaders/textureLoader.h"
#include "ObjectData.h"
#include <iostream>
#include <vector>

void ObjectData::loadModel(Models::Model modelData, VBOManager* vboHandler)
{
	assert(modelData.modelLoaded());
	
	std::vector<float> vertexList = modelData.getVertices();
	vbo.vertexCount = (int)vertexList.size();
	vbo.vertex = vboHandler->setupVBO(vertexList);
	vbo.texture = vboHandler->setupVBO(modelData.getTexCoords());
	vbo.normal = vboHandler->setupVBO(modelData.getNormals());
}

void ObjectData::setTexture(std::string imageName)
{
	textureID = Models::genTexture(texturePath + imageName);
}

void ObjectData::setTexture(GLuint textureID)
{
	this->textureID = textureID;
}

void ObjectData::setVBOs(VBOData vbo)
{
	this->vbo = VBOData(vbo);
}

void ObjectData::setHeightMap(std::string imageName)
{
	heightMapID = Models::genTexture(texturePath + imageName);
	flags.heightMapped = true;
}

void ObjectData::setHeightMap(GLuint heightMapID)
{
	this->heightMapID = heightMapID;
	flags.heightMapped = true;
}
