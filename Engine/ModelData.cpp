//Functions for getting model vertices.
//Basic models that can be defined by hand.
#include <vector>
#include <iostream>
namespace ModelData {
    std::vector<float> getCube() {
        int vertexCount = 108;
        std::vector<float> cubePtr = {
                -1.0f,  1.0f, -1.0f,        -1.0f, -1.0f, -1.0f,        1.0f, -1.0f, -1.0f, //Back-Bottom
                1.0f, -1.0f, -1.0f,         1.0f,  1.0f, -1.0f,         -1.0f,  1.0f, -1.0f, //Back-Top
                1.0f, -1.0f, -1.0f,         1.0f, -1.0f,  1.0f,         1.0f,  1.0f, -1.0f, //Right-Bottom
                1.0f, -1.0f,  1.0f,         1.0f,  1.0f,  1.0f,         1.0f,  1.0f, -1.0f, //Right-Top
                1.0f, -1.0f,  1.0f,         -1.0f, -1.0f,  1.0f,        1.0f,  1.0f,  1.0f, //Front-Bottom
                -1.0f, -1.0f,  1.0f,        -1.0f,  1.0f,  1.0f,        1.0f,  1.0f,  1.0f, //Front-Top
                -1.0f, -1.0f,  1.0f,        -1.0f, -1.0f, -1.0f,        -1.0f,  1.0f,  1.0f, //Left-Bottom
                -1.0f, -1.0f, -1.0f,        -1.0f,  1.0f, -1.0f,        -1.0f,  1.0f,  1.0f, //Left-Top
                -1.0f, -1.0f,  1.0f,        1.0f, -1.0f,  1.0f,         1.0f, -1.0f, -1.0f, //Bottom
                1.0f, -1.0f, -1.0f,         -1.0f, -1.0f, -1.0f,        -1.0f, -1.0f,  1.0f, //Bottom
                -1.0f,  1.0f, -1.0f,        1.0f,  1.0f, -1.0f,         1.0f,  1.0f,  1.0f, //Top
                1.0f,  1.0f,  1.0f,         -1.0f,  1.0f,  1.0f,        -1.0f,  1.0f, -1.0f //Top
        };
        return cubePtr;
    }
}