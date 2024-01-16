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
		bool genTextures = true;
		bool genNormals = true;
		//bool genTangents = false;
		std::vector<float> getVertices() { return vertexList; }
		std::vector<float> getTexCoords() {	return texList; }
		std::vector<float> getNormals() { return normalList; }
		void clearVals();
	};

	class ObjLoader : public ModelLoader {
	private:
	public:
		ObjLoader() {}
		ObjLoader(std::string filePath) { loadObj(filePath); }
		bool loadObj(std::string filePath);
	};
}