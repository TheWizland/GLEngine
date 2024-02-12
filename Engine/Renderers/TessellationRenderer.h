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
	public:
		void init();
		void render(ObjectData& object, Camera& camera, Light& light);
	};
}


