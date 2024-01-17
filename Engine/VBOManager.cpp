#include "VBOManager.h"
#include <cassert>

void VBOManager::init(int numVBO)
{
    this->numVBO = numVBO;
    vbo = new GLuint[numVBO];

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBO, vbo);
}

int VBOManager::setupVBO(std::vector<float> vals)
{
    assert(maxUnusedVBO < numVBO && "Insufficient VBO Available");
    glBindBuffer(GL_ARRAY_BUFFER, vbo[maxUnusedVBO]);
    glBufferData(GL_ARRAY_BUFFER, vals.size() * sizeof(float), &vals[0], GL_STATIC_DRAW);
    maxUnusedVBO++;
    return vbo[maxUnusedVBO - 1];
}
