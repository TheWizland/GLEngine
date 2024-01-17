#include "SkyboxRenderer.h"
#include "../Loaders/shaderLoader.h"
#include "../Loaders/ModelGenerator.h"
#include "../Loaders/textureLoader.h"
#include <GLM/gtc/type_ptr.hpp>

namespace Renderers {
	void SkyboxRenderer::init(std::string skyboxPath, std::string imageExtension, VBOManager vboGenerator)
	{
		program = FileLoader::createShaderProgram("shaders/skyboxV.glsl", "shaders/skyboxF.glsl");
		ModelGenerator::CubeGenerator cubeGen;
		cubeGen.genCube();
        std::vector<float> cubeVertices = cubeGen.getVertices();
        vertexCount = cubeVertices.size();
		vboVertex = vboGenerator.setupVBO(cubeVertices);
		vboTexture = vboGenerator.setupVBO(cubeGen.getTexCoords());

		textureID = FileLoader::genCubeMap(skyboxPath, imageExtension);
	}
	void SkyboxRenderer::render(Camera camera)
	{
		glUseProgram(program);
        GLuint vMatLoc, pMatLoc;
        vMatLoc = glGetUniformLocation(program, "v_matrix");
        glUniformMatrix4fv(vMatLoc, 1, GL_FALSE, glm::value_ptr(camera.getView()));

        pMatLoc = glGetUniformLocation(program, "proj_matrix");
        glUniformMatrix4fv(pMatLoc, 1, GL_FALSE, glm::value_ptr(camera.getPerspective()));

        glBindBuffer(GL_ARRAY_BUFFER, vboVertex);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);
        glDisable(GL_DEPTH_TEST);

        glDrawArrays(GL_TRIANGLES, 0, vertexCount);

        glFrontFace(GL_CCW);
        glEnable(GL_DEPTH_TEST);
	}
}
