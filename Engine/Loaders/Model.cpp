#include "Model.h"
#include <algorithm>
#include <cassert>
#include <GLM/vec3.hpp>
#include <GLM/vec2.hpp>

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

/* Tangent generation algorithm based on code from opengl-tutorial.org
License: CC-BY-NC-ND
http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
http://www.opengl-tutorial.org/uncategorized/2014/02/18/license-changes/ 
*/
void Models::Model::genTangents()
{
	tangentList.clear();
	glm::vec3 tangent;
	glm::vec3 vertex0, vertex1, vertex2;
	glm::vec2 uv0, uv1, uv2;
	glm::vec3 deltaPos1, deltaPos2;
	glm::vec2 deltaUV1, deltaUV2;

	assert(textureLoaded && normalLoaded && "genTangents requires textures and normals to be loaded.");

	for (int i = 0; i < vertexList.size(); i += 3)
	{
		vertex0 = glm::vec3(vertexList[i*3 + 0], vertexList[i*3 + 1], vertexList[i*3 + 2]);
		vertex1 = glm::vec3(vertexList[i*3 + 3], vertexList[i*3 + 4], vertexList[i*3 + 5]);
		vertex2 = glm::vec3(vertexList[i*3 + 6], vertexList[i*3 + 7], vertexList[i*3 + 8]);

		uv0 = glm::vec2(textureList[i*2 + 0], textureList[i*2 + 1]);
		uv1 = glm::vec2(textureList[i*2 + 2], textureList[i*2 + 3]);
		uv2 = glm::vec2(textureList[i*2 + 4], textureList[i*2 + 5]);

		deltaPos1 = vertex1 - vertex0;
		deltaPos2 = vertex2 - vertex0;
		deltaUV1 = uv1 - uv0;
		deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		tangent = r * (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y);
		//bitangent = r * (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x);
		for (int j = 0; j < 3; ++j) //Same tangent for all 3 vertices of a triangle.
		{
			tangentList.push_back(tangent.x);
			tangentList.push_back(tangent.y);
			tangentList.push_back(tangent.z);
		}
	}
	tangentLoaded = true;
}
