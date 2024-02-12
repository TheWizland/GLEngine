//Functions for getting model vertices.
//Basic models that can be defined by hand.
#include <vector>
#include <iostream>
#include "ModelGenerator.h"
#include <GLM/vec3.hpp>
#include <GLM/vec2.hpp>
#include <GLM/geometric.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>


namespace Models {
    bool genTextures = true;
    bool genNormals = true;

    Model genCube()
    {
        std::vector<float> vertexList = {
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
        std::vector<float> texList = {
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

        return Model(vertexList, texList);
    }

    Model genSphere(int precision)
    {
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> texCoords;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec3> tangents;

        std::vector<float> vertexList;
        std::vector<float> texList;
        std::vector<float> normalList;
        std::vector<float> tangentList;

        double toRadians = M_PI / 180;
        // calculate triangle vertices
        for (int i = 0; i <= precision; i++) {
            for (int j = 0; j <= precision; j++) {
                int index = i * (precision + 1) + j;
                double y = cos(toRadians * (180.0 - i * 180.0 / precision));
                double x = -cos(toRadians * (j * 360.0 / precision)) * abs(cos(asin(y)));
                double z = sin(toRadians * (j * 360.0 / precision)) * abs(cos(asin(y)));
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
                indices[index] = i * (precision + 1) + j; //Buffer overrun? VS might be lying.
                indices[index + 1] = i * (precision + 1) + j + 1;
                indices[index + 2] = (i + 1) * (precision + 1) + j;
                indices[index + 3] = i * (precision + 1) + j + 1;
                indices[index + 4] = (i + 1) * (precision + 1) + j + 1;
                indices[index + 5] = (i + 1) * (precision + 1) + j;
            }
        }

        for (int i = 0; i < numIndices; i++) {
            vertexList.push_back(vertices.at(indices[i]).x);
            vertexList.push_back(vertices.at(indices[i]).y);
            vertexList.push_back(vertices.at(indices[i]).z);
            texList.push_back(texCoords.at(indices[i]).x);
            texList.push_back(texCoords.at(indices[i]).y);
            normalList.push_back(normals.at(indices[i]).x);
            normalList.push_back(normals.at(indices[i]).y);
            normalList.push_back(normals.at(indices[i]).z);
            tangentList.push_back(tangents.at(indices[i]).x);
            tangentList.push_back(tangents.at(indices[i]).y);
            tangentList.push_back(tangents.at(indices[i]).z);
        }

        return Model(vertexList, texList, normalList, tangentList);
    }

    Model genTile()
    {
        std::vector<float> vertexList = {
            -1.0f, 0.0f, -1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, -1.0f,
            -1.0f, 0.0f, -1.0f,
            -1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f
        };
        std::vector<float> texList = {
            0.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f
        };
        std::vector<float> normalList = {
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f
        };
        return Model(vertexList, texList, normalList);
    }

    Model genPatch(float size)
    {
        //Patch is a quad.
        std::vector<float> vertexList = {
            -size, 0, -size,
            size, 0, -size,
            -size, 0, size,
            size, 0, size
        };

        std::vector<float> texList = {
            0.0f, 0.0f,
            1.f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f
        };

        //Normals are generated in the shader.
        return Model(vertexList, texList);
    }

    void applyTiling(Model& model, float tiling)
    {
        assert(model.hasTexture() && " Can't apply tiling with no texture coordinates.");
        std::vector<float> textureList = model.getTexCoords();
        std::for_each(textureList.begin(), textureList.end(), [tiling](float& coord) -> void { coord *= tiling; });
        model.setTexCoords(textureList);
    }

    /* Tangent generation algorithm based on code from opengl-tutorial.org
    License: CC-BY-NC-ND
    http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
    http://www.opengl-tutorial.org/uncategorized/2014/02/18/license-changes/
    */
    void genTangents(Model& model)
    {
        std::vector<float> tangentList;
        std::vector<float> vertexList = model.getVertices();
        std::vector<float> textureList = model.getTexCoords();
        glm::vec3 tangent;
        glm::vec3 vertex0, vertex1, vertex2;
        glm::vec2 uv0, uv1, uv2;
        glm::vec3 deltaPos1, deltaPos2;
        glm::vec2 deltaUV1, deltaUV2;

        assert(model.hasVertex() && "genTangents requires vertices to be loaded.");
        assert(model.hasVertex() && model.hasTexture() && model.hasNormal() && "genTangents requires textures and normals to be loaded.");


        for (int i = 0; i * 3 < vertexList.size(); i += 3)
        {
            vertex0 = glm::vec3(vertexList[i * 3 + 0], vertexList[i * 3 + 1], vertexList[i * 3 + 2]);
            vertex1 = glm::vec3(vertexList[i * 3 + 3], vertexList[i * 3 + 4], vertexList[i * 3 + 5]);
            vertex2 = glm::vec3(vertexList[i * 3 + 6], vertexList[i * 3 + 7], vertexList[i * 3 + 8]);

            uv0 = glm::vec2(textureList[i * 2 + 0], textureList[i * 2 + 1]);
            uv1 = glm::vec2(textureList[i * 2 + 2], textureList[i * 2 + 3]);
            uv2 = glm::vec2(textureList[i * 2 + 4], textureList[i * 2 + 5]);

            deltaPos1 = vertex1 - vertex0;
            deltaPos2 = vertex2 - vertex0;
            deltaUV1 = uv1 - uv0;
            deltaUV2 = uv2 - uv0;

            float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
            tangent = r * (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y);
            //bitangent = r * (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x);
            for (int j = 0; j < 3; ++j) //Same tangent for all 3 vertices of a triangle.
            {
                tangentList.push_back(tangent.x);
                tangentList.push_back(tangent.y);
                tangentList.push_back(tangent.z);
            }
        }
        model.setTangents(tangentList);
    }
}