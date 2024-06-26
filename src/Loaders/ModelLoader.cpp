#include "../AssetPaths.h"
#include "Model.h"
#include "ModelGenerator.h"
#include <algorithm>
#include <fstream>
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <sstream>
#include <string>
#include <vector>

namespace Models {
	Model loadObj(std::string fileName, bool genTextures, bool genNormals)
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> textures;
		std::vector<glm::vec3> normals;
		std::vector<float> triangles;

		std::vector<float> vertexList;
		std::vector<float> texList;
		std::vector<float> normalList;

		std::ifstream objFile(modelPath + fileName);
		
		std::string buffer;
		int face[3][3];
		char discard;
		glm::vec3 point(0, 0, 0);
		glm::vec2 tex(0, 0);

		assert(objFile.is_open());

		while (std::getline(objFile, buffer)) {
			std::stringstream iss(buffer);
			std::string type;
			iss >> type;
			if (type == "#")
				continue;
			if (type == "v") {
				iss >> point.x >> point.y >> point.z;
				vertices.push_back(point);
			}
			else if (type == "vt") {
				iss >> tex.x >> tex.y;
				textures.push_back(tex);
			}
			else if (type == "vn") {
				iss >> point.x >> point.y >> point.z;
				normals.push_back(point);
			}
			else if (type == "f") {
				iss >> face[0][0] >> discard >> face[0][1] >> discard >> face[0][2]
					>> face[1][0] >> discard >> face[1][1] >> discard >> face[1][2]
					>> face[2][0] >> discard >> face[2][1] >> discard >> face[2][2];

				for (int i = 0; i < 3; ++i) {
					point = vertices[face[i][0] - 1];
					vertexList.push_back(point.x);
					vertexList.push_back(point.y);
					vertexList.push_back(point.z);

					if(genTextures) {
						tex = textures[face[i][1] - 1];
						texList.push_back(tex.x);
						texList.push_back(tex.y);
					}

					if (genNormals) {
						point = normals[face[i][2] - 1];
						normalList.push_back(point.x);
						normalList.push_back(point.y);
						normalList.push_back(point.z);
					}
				}
			}
		}

		return Model(vertexList, texList, normalList);
	}
}

