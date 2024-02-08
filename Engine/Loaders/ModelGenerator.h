#pragma once
#include <vector>
#include <string>
#include "Model.h"

namespace Models {
    extern bool genTextures;
    extern bool genNormals;
    extern bool genTangents;
    Model genCube();
    Model genSphere(int precision);
    Model genTile();
    Model loadObj(std::string fileName);
}