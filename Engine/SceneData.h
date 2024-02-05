#pragma once
#include "Camera.h"
#include "Light.h"
#include "Renderers/SkyboxRenderer.h"
#include "Renderers/StandardRenderer.h"
#include "DereferenceIterator.h"
#include <memory>

class SceneData
{
private:
	std::vector<std::unique_ptr<ObjectData>> objectList;
	std::unique_ptr<Camera> camera;
	std::unique_ptr<ObjectData> skybox;
public:
	Light getLight();
	void setLight(Light light);

	/* Destroys the current camera, creates a new one, and returns a non-owned pointer to the camera. */
	Camera* newCamera(float fieldOfView, float aspectRatio);
	/* Returns a non-owned pointer to the camera. */
	Camera* getCamera();
	/* Adds an object to the scene and returns a non-owned pointer. */
	ObjectData* genObject();
	void deleteObject(ObjectData* object);
	ObjectData* genSkybox(std::string name, std::string extension, VBOManager vboGenerator);
	ObjectData* getSkybox();
	DereferenceIterator<std::vector<std::unique_ptr<ObjectData>>::iterator> objectBegin();
	DereferenceIterator<std::vector<std::unique_ptr<ObjectData>>::iterator> objectEnd();
};

