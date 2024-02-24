#pragma once
#include "../Camera.h"
#include "../LightData/Light.h"
#include "../ObjectData.h"
#include "../SceneData.h"
#include <GL/glew.h>
#include <glm/mat4x4.hpp>


namespace Renderers {
	class StandardRenderer
	{
	private:
		GLuint program;
		GLuint programShadow;
		glm::mat4 camToTexSpace; //Used to convert camera space to texture space.
		void uniformObject(ObjectData& object);
		void uniformRenderFlags(RenderFlags& flags);
		void bindBuffers(ObjectData& object);
	public:
		void init();
		void uniformCamera(Camera camera);
		void uniformLight(Lighting::Light const& light);
		void bindShadow(GLuint shadowTex);
		void render(ObjectData& object);

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
