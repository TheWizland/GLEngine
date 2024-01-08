#pragma once
#include <GLM/mat4x4.hpp>
#include <GLM/vec3.hpp>
namespace Matrix {
	class Camera {
	private:
		glm::mat4 perspective;
		glm::vec3 location, U, V, N;
		float fovMax;
		float fovMin;
		float fov;
		float aspectRatio;
	public:
		Matrix::Camera();
		Matrix::Camera(float fieldOfView, float aspectRatio);
		void addFoV(float deltaFoV);
		void yaw(float distance);
		void globalYaw(float distance);
		glm::mat4 getView();
		glm::mat4 getPerspective();
	};
}