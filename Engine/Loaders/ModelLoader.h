#pragma once
#include <string>
#include <vector>

namespace FileLoader {
	class ModelLoader {
	protected:
		std::vector<float> vertexList;
		std::vector<float> texList;
		std::vector<float> normalList;
	public:
		std::vector<float> getVertices() { return vertexList; }
		std::vector<float> getTexCoords() {	return texList; }
		std::vector<float> getNormals() { return normalList; }
		void clearVals();
	};

	class ObjLoader : public ModelLoader {
	private:
	public:
		bool loadObj(std::string filePath);
	};
}