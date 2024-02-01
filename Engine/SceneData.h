#pragma once
#include "Camera.h"
#include "Light.h"
#include "Renderers/SkyboxRenderer.h"
#include "Renderers/StandardRenderer.h"
#include <memory>

class SceneData
{
private:
	Renderers::StandardRenderer standardRenderer;
	std::vector<std::unique_ptr<ObjectData>> renderList;
	std::unique_ptr<Camera> camera;
	std::unique_ptr<ObjectData> skybox;
public:
	void init();
	
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
	std::vector<std::unique_ptr<ObjectData>> const& getObjectList() const;
};

