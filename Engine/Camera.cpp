#include <glm/mat4.hpp>
#include <GLM/vec3.hpp>
#include <glm/ext.hpp>
#include <GLM/gtx/rotate_vector.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace glm;

class Camera {
private:
	mat4 perspective;
	vec3 location, U, V, N;
	float fovMax = M_PI * 2 / 3;
	float fovMin = M_PI / 6;
	float fov;
	float aspectRatio;
public:
	Camera(float fieldOfView, float aspectRatio) {
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

	void addFoV(float deltaFoV) {
		fov += deltaFoV;
		if (fov > fovMax)
			fov = fovMax;
		else if (fov < fovMin)
			fov = fovMin;

		perspective = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.0f);
	}

	void yaw(float distance) {
		N = glm::rotate(N, -distance, V);
		U = glm::rotate(U, -distance, V);
	}

	void globalYaw(float distance) {
		N = glm::rotateY(N, -distance);
		U = glm::rotateY(U, -distance);
		V = glm::rotateY(V, -distance);
	}

	mat4 getView() {
		mat4 view = glm::mat4(1); //Identity Matrix
		mat4 viewR = mat4(
			U.x, V.x, -N.x, 0,
			U.y, V.y, -N.y, 0,
			U.z, V.z, -N.z, 0,
			0, 0, 0, 0);

		mat4 viewT = mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			-location.x, -location.y, -location.z, 1);

		return view * viewR * viewT;
	}

	mat4 getPerspective() {
		return mat4(perspective);
	}
};