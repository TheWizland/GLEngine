#include "Model.h"
#include <algorithm>

Models::Model::Model(std::vector<float> vertexList)
{
	this->vertexList = vertexList;
	vertexLoaded = true;
	isLoaded = true;
}

Models::Model::Model(std::vector<float> vertexList, std::vector<float> textureList)
{
	this->vertexList = vertexList;
	this->textureList = textureList;
	vertexLoaded = true;
	textureLoaded = true;
	isLoaded = true;
}

Models::Model::Model(std::vector<float> vertexList, std::vector<float> textureList, std::vector<float> normalList)
{
	this->vertexList = vertexList;
	this->textureList = textureList;
	this->normalList = normalList;
	vertexLoaded = true;
	textureLoaded = true;
	normalLoaded = true;
	isLoaded = true;
}

void Models::Model::applyTiling(float tiling)
{
	std::for_each(textureList.begin(), textureList.end(), [tiling](float& coord) -> void { coord *= tiling; });
}
