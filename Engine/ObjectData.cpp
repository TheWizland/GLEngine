#include "ObjectData.h"
#include <iostream>
#include <vector>
#include "Loaders/textureLoader.h"

void ObjectData::loadModel(FileLoader::ModelLoader modelData, VBOManager* vboHandler)
{
	std::vector<float> vertexList = modelData.getVertices();
	vertexCount = vertexList.size();
	vboVertex = vboHandler->setupVBO(vertexList);
	vboTex = vboHandler->setupVBO(modelData.getTexCoords());
	vboNormal = vboHandler->setupVBO(modelData.getNormals());
}

void ObjectData::setTexture(std::string path)
{
	textureID = FileLoader::genTexture(path);
}
