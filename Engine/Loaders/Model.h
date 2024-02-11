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
	public:
		Model(std::vector<float> vertexList);
		Model(std::vector<float> vertexList, std::vector<float> textureList);
		Model(std::vector<float> vertexList, std::vector<float> textureList, std::vector<float> normalList);
		Model(std::vector<float> vertexList, std::vector<float> textureList, std::vector<float> normalList, std::vector<float> tangentList);
		std::vector<float> const getVertices() { return vertexList; }
		std::vector<float> const getTexCoords() { return textureList; }
		std::vector<float> const getNormals() { return normalList; }
		std::vector<float> const getTangents() { return tangentList; }
		bool const hasVertex() { return vertexLoaded; }
		bool const hasTexture() { return textureLoaded; }
		bool const hasNormal() { return normalLoaded; }
		bool const hasTangent() { return tangentLoaded; }
		size_t const vertexCount() { return vertexList.size(); }
		
		void setTexCoords(std::vector<float> texCoords);
		void setTangents(std::vector<float> tangents);
	};
}
