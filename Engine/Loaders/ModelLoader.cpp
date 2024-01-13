#include <GL\glew.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "ModelLoader.h"
#include <GLM/vec3.hpp>
#include <GLM/vec2.hpp>

std::vector<std::string> splitLine(std::string line, char delim) {
	std::vector<std::string> vals;
	std::string subStr;
	int pos;
	pos = line.find(delim);
	while (pos != std::string::npos) {
		vals.push_back(line.substr(0, pos));
		line.erase(0, pos + 1);
		pos = line.find(delim);
	}
	vals.push_back(line);

	return vals;
}

namespace FileLoader {
	void ModelLoader::clearVals()
	{
		vertexList.clear();
		texList.clear();
		normalList.clear();
	}

	bool FileLoader::ObjLoader::loadObj(std::string filePath)
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> textures;
		std::vector<glm::vec3> normals;
		std::vector<float> triangles;
		std::ifstream objFile(filePath);

		std::string buffer;
		std::vector<std::string> temp;
		glm::vec3 point(0, 0, 0);
		glm::vec2 tex(0, 0);

		if (!objFile.is_open())
			throw("ifstream: " + filePath + " failed to load.");

		while (std::getline(objFile, buffer)) {
			if (buffer[0] == '#')
				continue;
			if (buffer.rfind("v ", 0) == 0) {
				temp = splitLine(buffer, ' ');
				point = glm::vec3(std::stof(temp[1]), std::stof(temp[2]), std::stof(temp[3]));
				vertices.push_back(point);
			}
			if (buffer.rfind("vt ", 0) == 0) {
				temp = splitLine(buffer, ' ');
				tex = glm::vec2(std::stof(temp[1]), std::stof(temp[2]));
				textures.push_back(tex);
			}
			if (buffer.rfind("vn ", 0) == 0) {
				temp = splitLine(buffer, ' ');
				point = glm::vec3(std::stof(temp[1]), std::stof(temp[2]), std::stof(temp[3]));
				normals.push_back(point);
			}
			if (buffer.rfind("f ", 0) == 0) {
				temp = splitLine(buffer, ' ');
				//Should split into 4.
				std::vector<std::string> triangles;
				for (int i = 0; i < 3; ++i) {
					triangles = splitLine(temp[i + 1], '/');
					int vertexID = std::stoi(triangles[0]);
					point = vertices[vertexID - 1];
					vertexList.push_back(point.x);
					vertexList.push_back(point.y);
					vertexList.push_back(point.z);

					int texID = std::stoi(triangles[1]);
					tex = textures[texID - 1];
					texList.push_back(tex.x);
					texList.push_back(tex.y);

					int normID = std::stoi(triangles[2]);
					point = normals[normID - 1];
					normalList.push_back(point.x);
					normalList.push_back(point.y);
					normalList.push_back(point.z);
				}
			}
		}

		return true;
	}
}

