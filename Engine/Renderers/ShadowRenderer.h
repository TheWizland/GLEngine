#pragma once
#include "../ObjectData.h"
#include <GL/glew.h>
#include <memory>
#include "../SceneData.h"

namespace Renderers {
	class ShadowRenderer
	{
	private:
		GLuint program;
		GLuint depthMapBuffer;
		GLuint depthMapTexture;
		int shadowWidth, shadowHeight;
		glm::mat4 lightVP;
		void bindings();
		void clearBindings();
	public:
		void init();
		void init(int width, int height);
		GLuint getDepthMapTexture() { return depthMapTexture; }
		void uniformLight(Light& light);
		void render(ObjectData const& object);

		template<typename ObjectIterator>
		void render(ObjectIterator const objStart, ObjectIterator const objEnd)
		{
			for (auto it = objStart; it != objEnd; ++it)
			{
				render(*it);
			}
		}

		void render(SceneData& scene);
	};
}