#pragma once
#include "../Camera.h"
#include "../Light.h"
#include "../ObjectData.h"
#include <GL/glew.h>

namespace Renderers {
	class StandardRenderer
	{
	private:
		GLuint program;
		void uniformObject(ObjectData object);
		void bindBuffers(ObjectData object);
	public:
		void init();
		void uniformCamera(Camera camera);
		void uniformLight(Light light);
		void render(ObjectData renderList);
	};
}
