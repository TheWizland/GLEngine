#pragma once
#include "./SceneData.h"
#include "../VBOManager.h"
#include "../VBOData.h"

class TestScene : public SceneData 
{
private:
    VBOManager vboGenerator;
public:
    void init();

};