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
		programTess = Shaders::createShaderProgram("shaders/tessellationV.glsl",
			"shaders/tessellationTCS.glsl",
			"shaders/tessellationTES.glsl",
			"shaders/shadowF.glsl");

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

	void ShadowRenderer::enableBindings()
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
		bindingsActive = true;
	}

	void ShadowRenderer::clearBindings()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glActiveTexture(GL_TEXTURE0);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDrawBuffer(GL_FRONT);
		bindingsActive = false;
	}

	void ShadowRenderer::uniformLight(Light& light)
	{
		float aspectRatio = (float)(shadowWidth) / shadowHeight;
		lightV = light.getView();
		lightP = light.getPerspective(aspectRatio);
	}

	void ShadowRenderer::render(ObjectData const& object)
	{
		assert(bindingsActive && "enableBindings() should be called before rendering. Call clearBindings() once ShadowRenderer is finished.");

		if (!object.flags.castsShadows
			|| !object.flags.visible)
			return;

		glUseProgram(program);
		glm::mat4 shadowMVP = lightP * lightV * object.matrix->getModel();

		int shadowLoc = glGetUniformLocation(program, "shadowMVP");
		glProgramUniformMatrix4fv(program, shadowLoc, 1, false, glm::value_ptr(shadowMVP));

		glBindBuffer(GL_ARRAY_BUFFER, object.vbo.vertex);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glDrawArrays(GL_TRIANGLES, 0, object.vbo.vertexCount);
	}

	void ShadowRenderer::render(SceneData& scene)
	{
		assert(bindingsActive && "enableBindings() should be called before rendering. Call clearBindings() once ShadowRenderer is finished.");

		glUseProgram(program);
		uniformLight(*scene.getLight());
		for (auto it = scene.objectBegin(); it != scene.objectEnd(); ++it) {
			render(*it);
		}
	}

	//Hacky solution for tessellated objects. Maybe make a separate ShadowRenderer class for tessellated objects?
	void ShadowRenderer::renderTessellated(ObjectData const& object)
	{
		assert(bindingsActive && "enableBindings() should be called before rendering. Call clearBindings() once ShadowRenderer is finished.");
		if (!object.flags.castsShadows
			|| !object.flags.visible)
			return;

		glUseProgram(programTess);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, object.heightMapID);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, object.normalMapID);

		int mLoc = glGetUniformLocation(programTess, "m_matrix");
		glProgramUniformMatrix4fv(programTess, mLoc, 1, false, glm::value_ptr(object.matrix->getModel()));
		int vLoc = glGetUniformLocation(programTess, "v_matrix");
		glProgramUniformMatrix4fv(programTess, vLoc, 1, false, glm::value_ptr(lightV));
		int pLoc = glGetUniformLocation(programTess, "p_matrix");
		glProgramUniformMatrix4fv(programTess, pLoc, 1, false, glm::value_ptr(lightP));

		glBindBuffer(GL_ARRAY_BUFFER, object.vbo.vertex);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, object.vbo.texture);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		//glDrawArrays(GL_TRIANGLES, 0, object.vbo.vertexCount);
		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_CULL_FACE);
		glDrawArrays(GL_PATCHES, 0, 4);
	}
}