#pragma once
#include "../Camera.h"
#include "../LightData/Light.h"
#include "../VBOManager.h"
#include "../ObjectData.h"
#include "../DereferenceIterator.h"
#include <memory>

class SceneData
{
private:
	std::vector<std::unique_ptr<ObjectData>> objectList;
	//TODO: Maybe instead of vector<unique_ptr>, simply add an isDeleted field to ObjectData
	//Then, whenever access to objectList is requested, remove any objects with isDeleted.
	//Setting isDeleted should also delete unique_ptr<MatrixCollection> in ObjectData?
	//Downside: Could cause stale data.
	std::unique_ptr<Camera> camera;
	std::unique_ptr<ObjectData> skybox;
	std::unique_ptr<Lighting::PositionalLight> light;
public:
	virtual void init() {}
	Lighting::PositionalLight* newLight();
	Lighting::PositionalLight* getLight();
	/* Destroys the current camera, creates a new one, and returns a non-owned pointer to the camera. */
	Camera* newCamera(float fieldOfView, float aspectRatio);
	/* Returns a non-owned pointer to the camera. */
	Camera* getCamera();
	/* Adds an object to the scene and returns a non-owned pointer. */
	ObjectData* genObject();
	void deleteObject(ObjectData* object);
	ObjectData* genSkybox(std::string name, std::string extension, VBOManager& vboGenerator);
	ObjectData* getSkybox();
	DereferenceIterator<std::vector<std::unique_ptr<ObjectData>>::iterator> objectBegin();
	DereferenceIterator<std::vector<std::unique_ptr<ObjectData>>::iterator> objectEnd();
};

