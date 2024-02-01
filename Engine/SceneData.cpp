#include "AssetPaths.h"
#include "SceneData.h"
#include "Loaders/ModelGenerator.h"
#include "Loaders/textureLoader.h"
#include <algorithm>

void SceneData::init()
{
	standardRenderer.init();
}

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
	renderList.emplace_back(std::make_unique<ObjectData>());
	return renderList.back().get();
}

void SceneData::deleteObject(ObjectData* object)
{
	renderList.erase(std::remove_if(renderList.begin(), renderList.end(), 
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

std::vector<std::unique_ptr<ObjectData>> const& SceneData::getObjectList() const
{
	return renderList;
}