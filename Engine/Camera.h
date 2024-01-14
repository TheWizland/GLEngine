#pragma once
#include <GLM/mat4x4.hpp>
#include <GLM/vec3.hpp>
class Camera {
private:
	glm::mat4 perspective;
	glm::vec3 location, U, V, N;
	float fovMax = 120;
	float fovMin = 30;
	float fov;
	float aspectRatio;
	float curPitch = 0;
public:
	Camera();
	Camera(float fieldOfView, float aspectRatio);
	void addFoV(float deltaFoV);
	void yaw(float distance);
	void globalYaw(float distance);
	void pitch(float distance);
	void roll(float distance);
	void moveForward(float distance);
	void moveRight(float distance);
	void moveUp(float distance);
	void translate(glm::vec3 translation);
	void translate(float x, float y, float z);
	glm::mat4 getView();
	glm::mat4 getPerspective();
};