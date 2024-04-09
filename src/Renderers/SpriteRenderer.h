#pragma once
#include "../ObjectData.h"
#include <GL/glew.h>
#include "../Camera.h"
#include "../VBOManager.h"
#include <glm/vec3.hpp>

namespace Renderers {
    //Renderer for 2D sprites that always face the camera.
    //Does not support lighting.
    //Intended as a basis for a particle system.
	class SpriteRenderer
	{
    private:
        GLuint program;
        glm::vec3 cameraUp, cameraForward, cameraRight;
        VBOData tileVBO;
    public:
        void init(VBOManager& vboGen);
        void uniformCamera(Camera& camera, bool verticalRotation = true);
        //Renders a vertical tile.
        void render(MatrixCollection& matrix, GLuint sprite);
    };
}