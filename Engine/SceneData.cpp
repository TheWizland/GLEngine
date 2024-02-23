#include "AssetPaths.h"
#include "SceneData.h"
#include "Loaders/ModelGenerator.h"
#include "Loaders/textureLoader.h"
#include <algorithm>

Lighting::PositionalLight* SceneData::newLight()
{
	light = std::make_unique<Lighting::PositionalLight>();
	return light.get();
}

Lighting::PositionalLight* SceneData::getLight()
{
	return light.get();
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

	skybox->vbo.vertexCount = (int)cube.vertexCount();
	skybox->vbo.vertex = vboGenerator.setupVBO(cube.getVertices());
	skybox->vbo.texture = vboGenerator.setupVBO(cube.getTexCoords());

	std::string skyboxDir = skyboxPath + skyboxName + "/";
	skybox->textureID = Models::genCubeMap(skyboxDir, extension);

	return skybox.get();
}

ObjectData* SceneData::getSkybox()
{
	return skybox.get();
}

DereferenceIterator<std::vector<std::unique_ptr<ObjectData>>::iterator> SceneData::objectBegin()
{
	return dereference_iterator(objectList.begin());
}

DereferenceIterator<std::vector<std::unique_ptr<ObjectData>>::iterator> SceneData::objectEnd()
{
	return dereference_iterator(objectList.end());
}