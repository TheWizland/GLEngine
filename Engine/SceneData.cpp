#include "SceneData.h"

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

void SceneData::render(Light light)
{
	standardRenderer.uniformCamera(*camera.get());
	standardRenderer.uniformLight(light);
	for (auto const& object : renderList) {
		standardRenderer.render(*object);
	}
	//standardRenderer.render(renderList);
}
