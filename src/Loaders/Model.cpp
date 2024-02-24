#include "Model.h"
#include <algorithm>
#include <cassert>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

Models::Model::Model(std::vector<float> vertexList)
{
	this->vertexList = vertexList;
	vertexLoaded = true;
}

Models::Model::Model(std::vector<float> vertexList, std::vector<float> textureList)
	: Model(vertexList)
{
	this->textureList = textureList;
	textureLoaded = true;
}

Models::Model::Model(std::vector<float> vertexList, std::vector<float> textureList, std::vector<float> normalList)
	: Model(vertexList, textureList)
{
	this->normalList = normalList;
	normalLoaded = true;
}

Models::Model::Model(std::vector<float> vertexList, std::vector<float> textureList, std::vector<float> normalList, std::vector<float> tangentList)
	: Model(vertexList, textureList, normalList)
{
	this->tangentList = tangentList;
	tangentLoaded = true;
}

void Models::Model::setTexCoords(std::vector<float> texCoords)
{
	this->textureList = texCoords;
	textureLoaded = true;
}

void Models::Model::setTangents(std::vector<float> tangents)
{
	this->tangentList = tangents;
	tangentLoaded = true;
}
