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
	vertexCount = (int)vertexList.size();
	vboVertex = vboHandler->setupVBO(vertexList);
	vboTex = vboHandler->setupVBO(modelData.getTexCoords());
	vboNormal = vboHandler->setupVBO(modelData.getNormals());
}

void ObjectData::setTexture(std::string imageName)
{
	textureID = Models::genTexture(texturePath + imageName);
}

void ObjectData::setTexture(GLuint textureID)
{
	this->textureID = textureID;
}

void ObjectData::copyVBO(ObjectData object)
{
	vboVertex = object.vboVertex;
	vboTex = object.vboTex;
	vboNormal = object.vboNormal;
	vertexCount = object.vertexCount;
}
