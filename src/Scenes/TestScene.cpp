#include "TestScene.h"
#include "../Loaders/ModelGenerator.h"

void TestScene::init()
{
    vboGenerator.init(20);
    ObjectData* cube = genObject();
    cube->setVBOs(vboGenerator.setupVBO(Models::loadObj("cube.obj")));
    cube->setTexture("sand.jpg");
    cube->matrices()->translate(0, 3, 0);

    ObjectData* sphere = genObject();
    sphere->setVBOs(vboGenerator.setupVBO(Models::genSphere(24)));
    sphere->setTexture("moon.jpg");
    sphere->setNormalMap("moonN.jpg");
    MatrixCollection* cMat = cube->matrices();
    glm::mat4 cMod = cMat->getModel();
    sphere->matrices()->setParent(cube->matrices());
    sphere->matrices()->translate(0.f, 0.f, 0.f);

    ObjectData* sphere2 = genObject();
    sphere2->setVBOs(sphere->vbo);
    sphere2->setTexture(sphere->textureID);
    sphere2->matrices()->setParent(sphere->matrices());
    sphere2->matrices()->translate(0, -2, 0);
    sphere2->matrices()->scale(0.5f);

    ObjectData* dolphin = genObject();
    dolphin->setVBOs(vboGenerator.setupVBO(Models::loadObj("dolphinHighPoly.obj")));
    dolphin->setTexture("Dolphin_HighPolyUV.png");
    dolphin->matrices()->setParent(cube->matrices());
    dolphin->matrices()->translate(4.f, 0.f, 0.f);
    dolphin->matrices()->scale(1.75f);
    dolphin->matrices()->setApplyParentRotationToPosition(false);

    cube->matrices()->translate(-1.f, 0.f, 1.f);
    deleteObject(cube);
    dolphin->matrices()->translate(0.f, -1.f, 0.f);

    ObjectData* cube2 = genObject();
    Models::Model cubeModel = Models::loadObj("cube.obj");
    Models::genTangents(cubeModel);
    cube2->setVBOs(vboGenerator.setupVBO(cubeModel));
    cube2->setTexture("rock.jpg");
    cube2->setNormalMap("rockN.jpg");
    cube2->matrices()->translate(2, 3, 2);
}
