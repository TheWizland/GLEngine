#include "Camera.h"
#include <glm/mat4x4.hpp>
#include <GLM/vec3.hpp>
#include <glm/ext.hpp>
#include <GLM/gtx/rotate_vector.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace glm;

Camera::Camera() 
{
	Camera::Camera(90.f, 16.f / 9.f);
}

Camera::Camera(float fieldOfView, float aspectRatio) 
{
	fov = fieldOfView;
	if (fov > fovMax)
		fov = fovMax;
	else if (fov < fovMin)
		fov = fovMin;

	perspective = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.0f);
	location = vec3(0, 0, 0);
	U = vec3(1, 0, 0);
	V = vec3(0, 1, 0);
	N = vec3(0, 0, -1);

	this->aspectRatio = aspectRatio;
}

void Camera::addFoV(float deltaFoV) 
{
	fov += deltaFoV;
	if (fov > fovMax)
		fov = fovMax;
	else if (fov < fovMin)
		fov = fovMin;

	perspective = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.0f);
}

void Camera::yaw(float distance) 
{
	N = glm::rotate(N, -distance, V);
	U = glm::rotate(U, -distance, V);
}

void Camera::globalYaw(float distance) 
{
	N = glm::rotateY(N, -distance);
	U = glm::rotateY(U, -distance);
	V = glm::rotateY(V, -distance);
}

void Camera::pitch(float distance)
{
	if (curPitch + distance > M_PI / 2) {
		distance = (float)M_PI / 2 - curPitch;
	}
	else if (curPitch + distance < -M_PI / 2) {
		distance = (float)(-curPitch - M_PI / 2);
	}
	curPitch += distance;
	N = glm::rotate(N, distance, U);
	V = glm::rotate(V, distance, U);
}

void Camera::roll(float distance)
{
	U = glm::rotate(U, distance, N);
	V = glm::rotate(V, distance, N);
}

void Camera::moveForward(float distance)
{
	location += N * distance;
}

void Camera::moveRight(float distance)
{
	location += U * distance;
}

void Camera::moveUp(float distance)
{
	location += V * distance;
}

void Camera::translate(glm::vec3 translation)
{
	location += translation;
}

void Camera::translate(float x, float y, float z)
{
	location.x += x;
	location.y += y;
	location.z += z;
}

mat4 Camera::getView() 
{
	mat4 view = glm::mat4(1); //Identity Matrix
	mat4 viewR = mat4(
		U.x, V.x, -N.x, 0,
		U.y, V.y, -N.y, 0,
		U.z, V.z, -N.z, 0,
		0, 0, 0, 1);

	mat4 viewT = mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-location.x, -location.y, -location.z, 1);

	return viewR*viewT*view;
}

mat4 Camera::getPerspective() 
{
	return mat4(perspective);
}