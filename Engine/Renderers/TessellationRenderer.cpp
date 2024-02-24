#include "TessellationRenderer.h"
#include "../Loaders/shaderLoader.h"
#include <glm/gtc/type_ptr.hpp>

namespace Renderers
{
	void TessellationRenderer::init()
	{
		program = Shaders::createShaderProgram(
			"tessellationV.glsl", 
			"tessellationTCS.glsl",
			"tessellationTES.glsl",
			"tessellationF.glsl");

		camToTexSpace = glm::mat4(
			0.5f, 0, 0, 0,
			0, 0.5f, 0, 0,
			0, 0, 0.5f, 0,
			0.5f, 0.5f, 0.5f, 1.0f);
	}

	void TessellationRenderer::bindShadow(GLuint shadowTex)
	{
		glUseProgram(program);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, shadowTex);
	}

	void TessellationRenderer::render(ObjectData& object, Camera& camera, Lighting::Light& light)
	{
		glUseProgram(program);

		//Bindings
		glBindBuffer(GL_ARRAY_BUFFER, object.vbo.vertex);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, object.vbo.texture);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		//Textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, object.textureID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, object.heightMapID);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, object.normalMapID);


		GLuint mMatLoc = glGetUniformLocation(program, "m_matrix");
		glUniformMatrix4fv(mMatLoc, 1, GL_FALSE, glm::value_ptr(object.matrices()->getModel()));

		GLuint nLoc = glGetUniformLocation(program, "norm_matrix");
		glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(object.matrices()->getInverseTranspose()));

		GLuint vMatLoc = glGetUniformLocation(program, "v_matrix");
		glProgramUniformMatrix4fv(program, vMatLoc, 1, GL_FALSE, glm::value_ptr(camera.getView()));

		GLuint pMatLoc = glGetUniformLocation(program, "p_matrix");
		glProgramUniformMatrix4fv(program, pMatLoc, 1, GL_FALSE, glm::value_ptr(camera.getPerspective()));

		GLuint patchCountLoc = glGetUniformLocation(program, "patchCount");
		glUniform1i(patchCountLoc, 1);

		//Material
		{
			GLuint matAmbientLoc, matDiffuseLoc, matSpecularLoc, shininessLoc;
			matAmbientLoc = glGetUniformLocation(program, "material.ambient");
			glUniform4fv(matAmbientLoc, 1, glm::value_ptr(object.material.ads.ambient));

			matDiffuseLoc = glGetUniformLocation(program, "material.diffuse");
			glUniform4fv(matDiffuseLoc, 1, glm::value_ptr(object.material.ads.diffuse));

			matSpecularLoc = glGetUniformLocation(program, "material.specular");
			glUniform4fv(matSpecularLoc, 1, glm::value_ptr(object.material.ads.specular));

			shininessLoc = glGetUniformLocation(program, "material.shininess");
			glUniform1f(shininessLoc, object.material.shininess);
		}
		
		//Uniforms
		{
			GLuint globalAmbientLoc, ambientLoc, diffuseLoc, specularLoc, positionLoc;

			globalAmbientLoc = glGetUniformLocation(program, "globalAmbient");
			glProgramUniform4fv(program, globalAmbientLoc, 1, glm::value_ptr(Lighting::globalAmbient));

			ambientLoc = glGetUniformLocation(program, "light.ambient");
			glProgramUniform4fv(program, ambientLoc, 1, glm::value_ptr(light.ads.ambient));

			diffuseLoc = glGetUniformLocation(program, "light.diffuse");
			glProgramUniform4fv(program, diffuseLoc, 1, glm::value_ptr(light.ads.diffuse));

			specularLoc = glGetUniformLocation(program, "light.specular");
			glProgramUniform4fv(program, specularLoc, 1, glm::value_ptr(light.ads.specular));

			positionLoc = glGetUniformLocation(program, "light.position");
			glProgramUniform3fv(program, positionLoc, 1, glm::value_ptr(light.getPosition()));

			GLuint constAttLoc, linearAttLoc, quadAttLoc;
			constAttLoc = glGetUniformLocation(program, "light.constantAttenuation");
			glProgramUniform1f(program, constAttLoc, light.attenuation.constant);

			linearAttLoc = glGetUniformLocation(program, "light.linearAttenuation");
			glProgramUniform1f(program, linearAttLoc, light.attenuation.linear);

			quadAttLoc = glGetUniformLocation(program, "light.quadraticAttenuation");
			glProgramUniform1f(program, quadAttLoc, light.attenuation.quadratic);
		}

		float aspectRatio = 1200.f / 900;
		GLuint sVPLoc = glGetUniformLocation(program, "vp_shadow");
		glm::mat4 shadowVP = camToTexSpace * light.getPerspective() * light.getView();
		glProgramUniformMatrix4fv(program, sVPLoc, 1, GL_FALSE, glm::value_ptr(shadowVP));

		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_CULL_FACE);
		glDrawArrays(GL_PATCHES, 0, 4);

		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			printf("Tess Renderer Error: %d\n", err);
		}
	}
}