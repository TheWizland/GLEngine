#pragma once
#include <vector>
#include "ModelLoader.h"

namespace ModelGenerator {
    class CubeGenerator : public FileLoader::ModelLoader {
    public:
        void genCube();
    };
}