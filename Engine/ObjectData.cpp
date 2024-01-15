#include "ObjectData.h"
#include <iostream>
#include <vector>
#include "Loaders/textureLoader.h"

void ObjectData::loadModel(FileLoader::ModelLoader modelData, VBOManager* vboHandler)
{
	std::vector<float> vertexList = modelData.getVertices();
	std::vector<float> texList = modelData.getTexCoords();

	vboVertex = vboHandler->setupVBO(vertexList);
	vboTex = vboHandler->setupVBO(texList);
	vertexCount = vertexList.size();
	//vboNormal = vboHandler.setupVBO(normalList);
}

void ObjectData::setTexture(std::string path)
{
	textureID = FileLoader::genTexture(path);
}
