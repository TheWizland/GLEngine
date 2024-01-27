#include "SceneData.h"
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

void SceneData::render(Light light)
{
	standardRenderer.uniformCamera(*camera.get());
	standardRenderer.uniformLight(light);
	for (auto const& object : renderList) {
		standardRenderer.render(*object);
	}
}
