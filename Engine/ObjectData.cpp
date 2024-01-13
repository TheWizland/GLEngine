#include "ObjectData.h"
#include <iostream>
#include <vector>
#include "Loaders/textureLoader.h"

void ObjectData::loadModel(FileLoader::ModelLoader modelData, GLuint vboVertex, GLuint vboTex)
{
	std::vector<float> vertexList = modelData.getVertices();
	std::vector<float> texList = modelData.getTexCoords();

	glBindBuffer(GL_ARRAY_BUFFER, vboVertex);
	glBufferData(GL_ARRAY_BUFFER, vertexList.size() * sizeof(float), &vertexList[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboTex);
	glBufferData(GL_ARRAY_BUFFER, texList.size() * sizeof(float), &texList[0], GL_STATIC_DRAW);
	
	this->vboVertex = vboVertex;
	this->vboTex = vboTex;
}

void ObjectData::setTexture(std::string path)
{
	textureID = FileLoader::genTexture(path);
}
