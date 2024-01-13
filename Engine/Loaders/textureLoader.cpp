#include <GL\glew.h>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include <.\stb_image.h>

namespace FileLoader {
	GLuint genTexture(std::string path)
	{
		int width, height, channels;
		unsigned char* img = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
		if (img == NULL) {
			throw("stbi_load: " + path + " failed to load.");
		}

		GLuint textureID;
		glGenTextures(1, &textureID);

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		return textureID;
	}
}