#include "./AssetPaths.h"
#include "Loaders/textureLoader.h"
#include "ObjectData.h"

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
