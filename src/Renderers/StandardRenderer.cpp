#include "../Loaders/shaderLoader.h"
#include "StandardRenderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <iterator>

namespace Renderers {
    void StandardRenderer::uniformObject(ObjectData& object)
    {
        GLuint mMatLoc = glGetUniformLocation(program, "m_matrix");
        glUniformMatrix4fv(mMatLoc, 1, GL_FALSE, glm::value_ptr(object.matrices()->getModel()));
        
        GLuint nLoc = glGetUniformLocation(program, "norm_matrix");
        glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(object.matrices()->getInverseTranspose()));

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
    }

    void StandardRenderer::uniformRenderFlags(RenderFlags& flags)
    {
        GLuint internalLitLoc = glGetUniformLocation(program, "internallyLit");
        int normMult = flags.internallyLit ? -1 : 1;
        glUniform1i(internalLitLoc, normMult);

        GLuint heightMapLoc = glGetUniformLocation(program, "heightMapped");
        glUniform1i(heightMapLoc, (int)flags.heightMapped);

        GLuint shadowFlagLoc = glGetUniformLocation(program, "hasShadows");
        int shadowVal = !flags.hasShadows;
        glUniform1i(shadowFlagLoc, shadowVal);

        GLuint mappedLoc = glGetUniformLocation(program, "normalMapped");
        glUniform1i(mappedLoc, (int)flags.normalMapped);
    }

    void StandardRenderer::uniformCamera(Camera camera) {
        GLuint vMatLoc = glGetUniformLocation(program, "v_matrix");
        glProgramUniformMatrix4fv(program, vMatLoc, 1, GL_FALSE, glm::value_ptr(camera.getView()));

        GLuint pMatLoc = glGetUniformLocation(program, "proj_matrix");
        glProgramUniformMatrix4fv(program, pMatLoc, 1, GL_FALSE, glm::value_ptr(camera.getPerspective()));
    }

    void StandardRenderer::uniformLight(Lighting::Light const& light) {
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

        float aspectRatio = 1200.f / 900;
        GLuint sVPLoc = glGetUniformLocation(program, "vp_shadow");
        glm::mat4 shadowVP = camToTexSpace * light.getPerspective() * light.getView();
        glProgramUniformMatrix4fv(program, sVPLoc, 1, GL_FALSE, glm::value_ptr(shadowVP));
    }

    void StandardRenderer::bindShadow(GLuint shadowTex)
    {
        glUseProgram(program);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, shadowTex);
    }

    void StandardRenderer::bindBuffers(ObjectData& object)
    {
        //VBO
        glBindBuffer(GL_ARRAY_BUFFER, object.vbo.vertex);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, object.vbo.texture);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, object.vbo.normal);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(2);

        //Binding VBO or textures that are -1 causes OpenGL error 1282 on Linux
        //But not on Windows
        //It renders fine regardless, this is just to stop error vomit.
        if(object.vbo.tangent != -1) 
        {
            glBindBuffer(GL_ARRAY_BUFFER, object.vbo.tangent);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(3);
        }

        //Textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, object.textureID);

        if(object.heightMapID != -1)
        {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, object.heightMapID);
        }

        if(object.normalMapID != -1)
        {
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, object.normalMapID);
        }
    }

    void StandardRenderer::init()
    {
        program = Shaders::createShaderProgram(
            "textureV.glsl", 
            "textureF.glsl");
        camToTexSpace = glm::mat4(
            0.5f, 0, 0, 0,
            0, 0.5f, 0, 0,
            0, 0, 0.5f, 0,
            0.5f, 0.5f, 0.5f, 1.0f);
    }

    void StandardRenderer::render(ObjectData& object)
    {
        if (!object.flags.visible)
            return;

        glUseProgram(program);

        uniformObject(object);
        uniformRenderFlags(object.flags);
        bindBuffers(object);

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDrawArrays(GL_TRIANGLES, 0, object.vbo.vertexCount);
        
        GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			printf("Standard Renderer Error: %d\n", err);
		}
    }

    void StandardRenderer::render(SceneData& scene)
    {
        uniformCamera(*scene.getCamera());
        uniformLight(*scene.getLight());
        for (auto it = scene.objectBegin(); it != scene.objectEnd(); ++it) {
            render(*it);
        }
    }
}