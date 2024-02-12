#include "TessellationRenderer.h"
#include "../Loaders/shaderLoader.h"
#include <GLM/gtc/type_ptr.hpp>
#include <iostream>

namespace Renderers
{
	void TessellationRenderer::init()
	{
		program = Shaders::createShaderProgram("shaders/tessellationV.glsl", 
			"shaders/tessellationTCS.glsl",
			"shaders/tessellationTES.glsl",
			"shaders/tessellationF.glsl");
	}

	void TessellationRenderer::render(ObjectData& object, Camera& camera, Light& light)
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
			glUniform4fv(matAmbientLoc, 1, glm::value_ptr(object.material.ambient));

			matDiffuseLoc = glGetUniformLocation(program, "material.diffuse");
			glUniform4fv(matDiffuseLoc, 1, glm::value_ptr(object.material.diffuse));

			matSpecularLoc = glGetUniformLocation(program, "material.specular");
			glUniform4fv(matSpecularLoc, 1, glm::value_ptr(object.material.specular));

			shininessLoc = glGetUniformLocation(program, "material.shininess");
			glUniform1f(shininessLoc, object.material.shininess);
		}
		
		//Uniforms
		{
			GLuint globalAmbientLoc, ambientLoc, diffuseLoc, specularLoc, positionLoc;

			globalAmbientLoc = glGetUniformLocation(program, "globalAmbient");
			glProgramUniform4fv(program, globalAmbientLoc, 1, glm::value_ptr(Light::globalAmbient));

			ambientLoc = glGetUniformLocation(program, "light.ambient");
			glProgramUniform4fv(program, ambientLoc, 1, glm::value_ptr(light.ambient));

			diffuseLoc = glGetUniformLocation(program, "light.diffuse");
			glProgramUniform4fv(program, diffuseLoc, 1, glm::value_ptr(light.diffuse));

			specularLoc = glGetUniformLocation(program, "light.specular");
			glProgramUniform4fv(program, specularLoc, 1, glm::value_ptr(light.specular));

			positionLoc = glGetUniformLocation(program, "light.position");
			glProgramUniform3fv(program, positionLoc, 1, glm::value_ptr(light.position));

			GLuint constAttLoc, linearAttLoc, quadAttLoc;
			constAttLoc = glGetUniformLocation(program, "light.constantAttenuation");
			glProgramUniform1f(program, constAttLoc, light.constantAttenuation);

			linearAttLoc = glGetUniformLocation(program, "light.linearAttenuation");
			glProgramUniform1f(program, linearAttLoc, light.linearAttenuation);

			quadAttLoc = glGetUniformLocation(program, "light.quadraticAttenuation");
			glProgramUniform1f(program, quadAttLoc, light.quadraticAttenuation);
		}


		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_CULL_FACE);
		glDrawArrays(GL_PATCHES, 0, 4);

		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::cout << "Tess Renderer Error: " << err << std::endl;
		}
	}
}