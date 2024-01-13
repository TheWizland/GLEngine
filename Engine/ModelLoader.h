#pragma once
#include <string>
#include <vector>

namespace FileLoader {
	class ObjLoader {
	private:
		std::vector<float> vertexList;
		std::vector<float> texList;
		std::vector<float> normalList;
	public:
		bool loadObj(std::string filePath);
		std::vector<float> getVertices();
		std::vector<float> getTexCoords();
		std::vector<float> getNormals();
		void clearVals();
	};
}