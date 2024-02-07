#include "ShadowRenderer.h"
#include "../Loaders/shaderLoader.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

namespace Renderers {
	void ShadowRenderer::init()
	{
		init(1200, 900);
	}

	void ShadowRenderer::init(int width, int height)
	{
		shadowWidth = width;
		shadowHeight = height;

		program = Shaders::createShaderProgram("shaders/shadowV.glsl", "shaders/shadowF.glsl");
		glGenFramebuffers(1, &depthMapBuffer);
		glGenTextures(1, &depthMapTexture);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
			width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	void ShadowRenderer::bindings()
	{
		glUseProgram(program);

		glViewport(0, 0, shadowWidth, shadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapBuffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMapTexture, 0);

		glDrawBuffer(GL_NONE); //Don't want to draw on pass 1
		glReadBuffer(GL_NONE); //Why?

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_POLYGON_OFFSET_FILL); //Reduces shadow artifacts
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_POLYGON_OFFSET_FILL); //Artifact reduction
		glPolygonOffset(2.0f, 4.0f);

		//glBindFramebuffer(GL_FRAMEBUFFER, 0); Why?
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void ShadowRenderer::clearBindings()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glActiveTexture(GL_TEXTURE0);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDrawBuffer(GL_FRONT);
	}

	void ShadowRenderer::uniformLight(Light& light)
	{
		float aspectRatio = (float)(shadowWidth) / shadowHeight;
		lightVP = light.getMatVP(aspectRatio);
	}

	void ShadowRenderer::render(ObjectData const& object)
	{
		if (!object.flags.castsShadows
			|| !object.flags.visible)
			return;

		glm::mat4 shadowMVP = lightVP * object.matrix->getModel();

		int shadowLoc = glGetUniformLocation(program, "shadowMVP");
		glProgramUniformMatrix4fv(program, shadowLoc, 1, false, glm::value_ptr(shadowMVP));

		glBindBuffer(GL_ARRAY_BUFFER, object.vbo.vertex);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glDrawArrays(GL_TRIANGLES, 0, object.vbo.vertexCount);
	}

	void ShadowRenderer::render(SceneData& scene)
	{
		bindings();

		uniformLight(*scene.getLight());
		for (auto it = scene.objectBegin(); it != scene.objectEnd(); ++it) {
			render(*it);
		}

		clearBindings();
	}
}