#pragma once
#include <string>
#include "../VBOManager.h"
#include "../Camera.h"
#include "../ObjectData.h"

namespace Renderers {
	class SkyboxRenderer
	{
	private:
		GLuint program;
	public:
		void init();
		void render(ObjectData const& skybox, Camera camera);
	};
}

