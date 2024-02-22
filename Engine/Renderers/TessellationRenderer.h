#pragma once
#include "../ObjectData.h"
#include "../Camera.h"
#include "../Light.h"

namespace Renderers
{
	class TessellationRenderer
	{
	private:
		GLuint program;
		glm::mat4 camToTexSpace;
	public:
		void init();
		void bindShadow(GLuint shadowTex);
		void render(ObjectData& object, Camera& camera, Light& light);
	};
}


