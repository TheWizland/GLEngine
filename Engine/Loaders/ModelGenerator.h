#pragma once
#include <vector>
#include "ModelLoader.h"

namespace Models {
    class CubeGenerator : public Models::ModelLoader {
    public:
        CubeGenerator() { genCube(); }
        void genCube();
    };

    class SphereGenerator : public Models::ModelLoader {
    public:
        SphereGenerator() {}
        SphereGenerator(int precision) { genSphere(precision); }
        void genSphere(int precision);
    };

    class TileGenerator : public Models::ModelLoader {
    public:
        TileGenerator() { genTile(); }
        void genTile();
    };
}