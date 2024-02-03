#include "StandardRenderer.h"
#include <GLM/gtc/type_ptr.hpp>
#include "../Loaders/shaderLoader.h"

namespace Renderers {
    void StandardRenderer::uniformObject(ObjectData object)
    {
        GLuint mMatLoc = glGetUniformLocation(program, "m_matrix");
        glUniformMatrix4fv(mMatLoc, 1, GL_FALSE, glm::value_ptr(object.matrices.getModel()));

        GLuint nLoc = glGetUniformLocation(program, "norm_matrix");
        glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(object.matrices.getInverseTranspose()));

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

        //Flags
        {
            GLuint internalLitLoc;
            internalLitLoc = glGetUniformLocation(program, "internallyLit");
            int normMult = object.flags.getInternallyLit() ? -1 : 1;
            glUniform1i(internalLitLoc, normMult);
        }
    }

    void StandardRenderer::uniformCamera(Camera camera) {
        GLuint vMatLoc = glGetUniformLocation(program, "v_matrix");
        glProgramUniformMatrix4fv(program, vMatLoc, 1, GL_FALSE, glm::value_ptr(camera.getView()));

        GLuint pMatLoc = glGetUniformLocation(program, "proj_matrix");
        glProgramUniformMatrix4fv(program, pMatLoc, 1, GL_FALSE, glm::value_ptr(camera.getPerspective()));
    }

    void StandardRenderer::uniformLight(Light light) {
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

    void StandardRenderer::bindBuffers(ObjectData object)
    {
        glBindBuffer(GL_ARRAY_BUFFER, object.vboVertex);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, object.vboTex);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, object.vboNormal);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(2);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, object.textureID);
    }

    void StandardRenderer::init()
    {
        program = Shaders::createShaderProgram("shaders/textureV.glsl", "shaders/textureF.glsl");
    }

    void StandardRenderer::render(ObjectData object) 
    {
        glUseProgram(program);

        uniformObject(object);
        bindBuffers(object);

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDrawArrays(GL_TRIANGLES, 0, object.vertexCount);
    }
}