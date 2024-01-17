#include <GL\glew.h>
#include <string>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include <.\stb_image.h>
#include "textureLoader.h"

namespace FileLoader {
	GLuint genTexture(std::string path)
	{
		int width, height, channels;
		unsigned char* img = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
		std::string fail = path + " failed to load.";
		assert(("Image failed to load.", img != NULL));

		GLuint textureID;
		glGenTextures(1, &textureID);

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		return textureID;
	}

	GLuint FileLoader::genCubeMap(std::string path, std::string extension)
	{
		std::vector<std::string> files;
		files.push_back(path + "xp" + extension); //right
		files.push_back(path + "xn" + extension); //left
		files.push_back(path + "yp" + extension); //top
		files.push_back(path + "yn" + extension); //bottom
		files.push_back(path + "zp" + extension); //front
		files.push_back(path + "zn" + extension); //back

		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		for (int i = 0; i < files.size(); ++i) {
			int width, height, channels;
			unsigned char* img = stbi_load(files[i].c_str(), &width, &height, &channels, STBI_rgb);
			assert(img != NULL && "Image failed to load.");
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		
		return textureID;
	}
}