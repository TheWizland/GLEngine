#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
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
	Camera(float fieldOfView = 90.f, float aspectRatio = 4.f/3.f);
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