#include "AssetPaths.h"
#include "SceneData.h"
#include "Loaders/ModelGenerator.h"
#include "Loaders/textureLoader.h"
#include <algorithm>

Camera* SceneData::newCamera(float fieldOfView, float aspectRatio)
{
	camera = std::make_unique<Camera>(fieldOfView, aspectRatio);
	return camera.get();
}

Camera* SceneData::getCamera()
{
	return camera.get();
}

ObjectData* SceneData::genObject()
{
	objectList.emplace_back(std::make_unique<ObjectData>());
	return objectList.back().get();
}

void SceneData::deleteObject(ObjectData* object)
{
	objectList.erase(std::remove_if(objectList.begin(), objectList.end(), 
		[object](std::unique_ptr<ObjectData> const& objectPtr) { return objectPtr.get() == object; }));
}

ObjectData* SceneData::genSkybox(std::string skyboxName, std::string extension, VBOManager vboGenerator)
{
	skybox = std::make_unique<ObjectData>();

	Models::Model cube = Models::genCube();

	skybox->vertexCount = (int)cube.vertexCount();
	skybox->vboVertex = vboGenerator.setupVBO(cube.getVertices());
	skybox->vboTex = vboGenerator.setupVBO(cube.getTexCoords());

	std::string skyboxDir = skyboxPath + skyboxName + "/";
	skybox->textureID = Models::genCubeMap(skyboxDir, extension);

	return skybox.get();
}

ObjectData* SceneData::getSkybox()
{
	return skybox.get();
}

std::vector<std::unique_ptr<ObjectData>>::const_iterator SceneData::objectBegin()
{
	return objectList.begin();
}

std::vector<std::unique_ptr<ObjectData>>::const_iterator SceneData::objectEnd()
{
	return objectList.end();
}