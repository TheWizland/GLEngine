#pragma once
#include <vector>
#include "ModelLoader.h"

namespace ModelGenerator {
    class CubeGenerator : public FileLoader::ModelLoader {
    public:
        CubeGenerator() { genCube(); }
        void genCube();
    };

    class SphereGenerator : public FileLoader::ModelLoader {
    public:
        SphereGenerator() {}
        SphereGenerator(int precision) { genSphere(precision); }
        void genSphere(int precision);
    };

    class TileGenerator : public FileLoader::ModelLoader {
    public:
        TileGenerator() { genTile(); }
        void genTile();
    };
}