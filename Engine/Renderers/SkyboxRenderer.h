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
		void init(std::string skyboxPath, VBOManager vboGenerator);
		void render(Camera camera);
	};
}

