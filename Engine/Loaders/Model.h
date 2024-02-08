#pragma once
#include <vector>

namespace Models {
	class Model
	{
	private:
		std::vector<float> vertexList;
		std::vector<float> textureList;
		std::vector<float> normalList;
		std::vector<float> tangentList;
		bool vertexLoaded = false;
		bool textureLoaded = false;
		bool normalLoaded = false;
		bool tangentLoaded = false;
		bool isLoaded = false;
	public:
		Model(std::vector<float> vertexList);
		Model(std::vector<float> vertexList, std::vector<float> textureList);
		Model(std::vector<float> vertexList, std::vector<float> textureList, std::vector<float> normalList);
		std::vector<float> getVertices() { return vertexList; }
		std::vector<float> getTexCoords() { return textureList; }
		std::vector<float> getNormals() { return normalList; }
		bool modelLoaded() { return isLoaded; }
		bool hasVertex() { return vertexLoaded; }
		bool hasTexture() { return textureLoaded; }
		bool hasNormal() { return normalLoaded; }
		size_t vertexCount() { return vertexList.size(); }
		/* Multiply all texture coordinates by tiling.
		   Does nothing if no texture coordinates have been loaded. */
		void applyTiling(float tiling);
		void genTangents();
	};
}
