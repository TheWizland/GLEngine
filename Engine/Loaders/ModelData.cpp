//Functions for getting model vertices.
//Basic models that can be defined by hand.
#include <vector>
#include <iostream>
#include "ModelData.h"
#include <GLM/vec3.hpp>
#include <GLM/vec2.hpp>
#include <GLM/geometric.hpp>
#define _USE_MATH_DEFINES
#include <math.h>


namespace ModelGenerator {
    void ModelGenerator::CubeGenerator::genCube()
    {
        int vertexCount = 108;
        vertexList = {
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
        texList = {
            0.0f, 1.0f,     0.0f, 0.0f,     1.0f, 0.0f,
            1.0f, 0.0f,     1.0f, 1.0f,     0.0f, 1.0f,
            1.0f, 0.0f,     0.0f, 0.0f,     1.0f, 1.0f,
            0.0f, 0.0f,     0.0f, 1.0f,     1.0f, 1.0f,
            1.0f, 0.0f,     0.0f, 0.0f,     1.0f, 1.0f,
            0.0f, 0.0f,     0.0f, 1.0f,     1.0f, 1.0f,
            1.0f, 0.0f,     0.0f, 0.0f,     1.0f, 1.0f,
            0.0f, 0.0f,     0.0f, 1.0f,     1.0f, 1.0f,
            0.0f, 1.0f,     0.0f, 0.0f,     1.0f, 0.0f,
            1.0f, 0.0f,     1.0f, 1.0f,     0.0f, 1.0f,
            0.0f, 1.0f,     0.0f, 0.0f,     1.0f, 0.0f,
            1.0f, 0.0f,     1.0f, 1.0f,     0.0f, 1.0f
        };
    }

    void SphereGenerator::genSphere(int precision)
    {
        int vertexCount = (precision + 1) * (precision + 1);
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> texCoords;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec3> tangents;

        double toRadians = M_PI / 180;
        // calculate triangle vertices
        for (int i = 0; i <= precision; i++) {
            for (int j = 0; j <= precision; j++) {
                int index = i * (precision + 1) + j;
                float y = cos(toRadians*(180.0 - i * 180.0 / precision));
                float x = -cos(toRadians*(j * 360.0 / precision)) * abs(cos(asin(y)));
                float z = sin(toRadians*(j * 360.0 / precision)) * abs(cos(asin(y)));
                vertices.push_back(glm::vec3(x, y, z));
                texCoords.push_back(glm::vec2((float)j / precision, (float)i / precision));
                normals.push_back(glm::vec3(x, y, z));

                // calculate tangent vector
                if (((x == 0) && (y == 1) && (z == 0)) || ((x == 0) && (y == -1) && (z == 0))) {
                    tangents.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
                }
                else {
                    tangents.push_back(glm::cross(glm::vec3(0, 1, 0), glm::vec3(x, y, z)));
                }
            }
        }

        int numIndices = precision * precision * 6;
        int* indices = new int[numIndices];
        for (int i = 0; i < precision; i++) {
            for (int j = 0; j < precision; j++) {
                int index = 6 * (i * precision + j);
                indices[index] = i * (precision + 1) + j;
                indices[index + 1] = i * (precision + 1) + j + 1;
                indices[index + 2] = (i + 1) * (precision + 1) + j;
                indices[index + 3] = i * (precision + 1) + j + 1;
                indices[index + 4] = (i + 1) * (precision + 1) + j + 1;
                indices[index + 5] = (i + 1) * (precision + 1) + j;
            }
        }

        vertexCount = numIndices * 3;
        for (int i = 0; i < numIndices; i++) {
            vertexList.push_back(vertices.at(indices[i]).x);
            vertexList.push_back(vertices.at(indices[i]).y);
            vertexList.push_back(vertices.at(indices[i]).z);
            texList.push_back(texCoords.at(indices[i]).x);
            texList.push_back(texCoords.at(indices[i]).y);
            normalList.push_back(normals.at(indices[i]).x);
            normalList.push_back(normals.at(indices[i]).y);
            normalList.push_back(normals.at(indices[i]).z);
            /*tangentList.push_back(tangents.at(indices[i]).x());
            tangentList.push_back(tangents.at(indices[i]).y());
            tangentList.push_back(tangents.at(indices[i]).z());*/
        }
    }
}