#pragma once
#include <string>
#include "../VBOManager.h"
#include "../Camera.h"

namespace Renderers {
	class SkyboxRenderer
	{
	private:
		GLuint program, vboVertex, vboTexture, textureID;
		int vertexCount;
	public:
		void init(std::string skyboxName, std::string extension, VBOManager vboGenerator);
		void render(Camera camera);
	};
}

