#pragma once
#include <string>
#include <vector>

namespace Models {
	class ModelLoader {
	protected:
		std::vector<float> vertexList;
		std::vector<float> texList;
		std::vector<float> normalList;
		bool isLoaded = false;
	public:
		bool genTextures = true;
		bool genNormals = true;
		//bool genTangents = false;
		/* Apply tiling to an already loaded set of texture coordinates. 
		   Does nothing if no texture coordinates have been loaded. */
		void applyTiling(float tiling);
		std::vector<float> getVertices() { return vertexList; }
		std::vector<float> getTexCoords() {	return texList; }
		std::vector<float> getNormals() { return normalList; }
		bool modelLoaded() { return isLoaded; }
		void clearVals();
	};

	class ObjLoader : public ModelLoader {
	private:
	public:
		ObjLoader() {}
		ObjLoader(std::string fileName) { loadObj(fileName); }
		bool loadObj(std::string fileName);
	};
}