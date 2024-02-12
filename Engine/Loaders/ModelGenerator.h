#pragma once
#include <vector>
#include <string>
#include "Model.h"

namespace Models {
    extern bool genTextures;
    extern bool genNormals;
    Model genCube();
    Model genSphere(int precision);
    Model genTile();
    Model genPatch(float size = 0.5f);
    Model loadObj(std::string fileName);
    /* Multiply all texture coordinates by tiling value. */
    void applyTiling(Model model, float tiling);
    /* Generate tangents from existing vertices and texture coordinates.
       Doesn't require normals, but it makes no sense to invoke without normal vectors. */
    void genTangents(Model& model);
}