#include "ObjectData.h"
#include <iostream>
#include <vector>
#include "Loaders/textureLoader.h"
#include "./AssetPaths.h"

void ObjectData::loadModel(Models::ModelLoader modelData, VBOManager* vboHandler)
{
	assert(modelData.modelLoaded());
	
	std::vector<float> vertexList = modelData.getVertices();
	vertexCount = vertexList.size();
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
