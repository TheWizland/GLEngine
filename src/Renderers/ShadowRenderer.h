#pragma once
#include "../ObjectData.h"
#include <GL/glew.h>
#include <memory>
#include "../Scenes/SceneData.h"

namespace Renderers {
	class ShadowRenderer
	{
	private:
		GLuint program;
		GLuint programTess;
		GLuint depthMapBuffer;
		GLuint depthMapTexture;
		int shadowWidth, shadowHeight;
		glm::mat4 lightV;
		glm::mat4 lightP;
		bool bindingsActive = false;
	public:
		void enableBindings();
		void clearBindings();
		void init();
		void init(int width, int height);
		GLuint getDepthMapTexture() { return depthMapTexture; }
		void uniformLight(Lighting::Light& light);
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
		void renderTessellated(ObjectData const& object);
	};
}