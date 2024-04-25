#include "SpriteRenderer.h"
#include <glm/gtc/type_ptr.hpp>
#include "../Loaders/shaderLoader.h"
#include "../Loaders/textureLoader.h"
#include "../Loaders/Model.h"
#include "../AssetPaths.h"
#include "../Loaders/ModelGenerator.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

namespace Renderers 
{
    void SpriteRenderer::init(VBOManager& vboGen)
    {
        program = Shaders::createShaderProgram(
            "spriteV.glsl", 
            "spriteF.glsl");
        
        std::vector<float> billboard_vertices = {
            -.5f, -.5f, 0.f,
            .5f, .5f, 0.f,
            .5f, -.5f, 0.f,
            -.5f, -.5f, 0.f,
            -.5f, .5f, 0.f,
            .5f, .5f, 0.f
        };
        std::vector<float> billboard_tex = {
            0.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f
        };
        Models::Model billboard(billboard_vertices, billboard_tex);
        tileVBO = vboGen.setupVBO(billboard);
    }

    void SpriteRenderer::uniformCamera(Camera& camera, bool verticalRotation)
    {
        cameraUp = camera.getUp();
        cameraRight = camera.getRight();
        
        if(!verticalRotation) { //Object will only rotate on Y-axis.
            cameraUp = glm::vec3(0,1,0);
            cameraRight.y = 0;
            cameraRight = glm::normalize(cameraRight);
        }
        //glm::mat4 ortho = glm::ortho(0.f, 100.f, 100.f, 0.f, -1.f, 1.f);
        glm::mat4 vpMatrix = camera.getPerspective() * camera.getView();

        GLuint rightLoc = glGetUniformLocation(program, "camera_right");
        glProgramUniform3fv(program, rightLoc, 1, glm::value_ptr(cameraRight));

        GLuint upLoc = glGetUniformLocation(program, "camera_up");
        glProgramUniform3fv(program, upLoc, 1, glm::value_ptr(cameraUp));
        
        GLuint vpLoc = glGetUniformLocation(program, "vp_matrix");
        glProgramUniformMatrix4fv(program, vpLoc, 1, GL_FALSE, glm::value_ptr(vpMatrix));

        //Size can be scale in m_matrix? Safe to get rid of?
        glm::vec2 size(0.5f,0.5f);
        GLuint sizeLoc = glGetUniformLocation(program, "billboard_size");
        glProgramUniform2fv(program, sizeLoc, 1, glm::value_ptr(size));
    }

    void SpriteRenderer::render(MatrixCollection& matrix, GLuint sprite)
    {
        glUseProgram(program);
        GLuint mLoc = glGetUniformLocation(program, "m_matrix");
        glProgramUniformMatrix4fv(program, mLoc, 1, GL_FALSE, glm::value_ptr(matrix.getModel()));

        glBindBuffer(GL_ARRAY_BUFFER, tileVBO.vertex);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, tileVBO.texture);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sprite);
        
        glDepthFunc(GL_LEQUAL);
        glDrawArrays(GL_TRIANGLES, 0, tileVBO.vertexCount);
        //Object rotation is determined by camera values.
    }
}