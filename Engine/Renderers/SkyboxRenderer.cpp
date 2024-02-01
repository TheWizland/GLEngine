#include "../Loaders/ModelGenerator.h"
#include "../Loaders/shaderLoader.h"
#include "../Loaders/textureLoader.h"
#include "SkyboxRenderer.h"
#include <GLM/gtc/type_ptr.hpp>

namespace Renderers {
	void SkyboxRenderer::init()
	{
		program = Shaders::createShaderProgram("shaders/skyboxV.glsl", "shaders/skyboxF.glsl");
	}

	void SkyboxRenderer::render(ObjectData skybox, Camera camera)
	{
		glUseProgram(program);
        GLuint vMatLoc, pMatLoc;
        vMatLoc = glGetUniformLocation(program, "v_matrix");
        glUniformMatrix4fv(vMatLoc, 1, GL_FALSE, glm::value_ptr(camera.getView()));

        pMatLoc = glGetUniformLocation(program, "proj_matrix");
        glUniformMatrix4fv(pMatLoc, 1, GL_FALSE, glm::value_ptr(camera.getPerspective()));

        glBindBuffer(GL_ARRAY_BUFFER, skybox.vboVertex);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textureID);

        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);
        glDisable(GL_DEPTH_TEST);

        glDrawArrays(GL_TRIANGLES, 0, skybox.vertexCount);

        glFrontFace(GL_CCW);
        glEnable(GL_DEPTH_TEST);
	}
}
