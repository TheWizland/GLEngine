#include "Light.h"
#include <GLM/gtc/matrix_transform.hpp>

glm::vec4 Light::globalAmbient = glm::vec4(0.6f, 0.6f, 0.6f, 1.f);

Light::Light() {
	ambient = glm::vec4(0.3f, 0.3f, 0.3f, 1.f);
	diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.f);
	specular = glm::vec4(0.8f, 0.8f, 0.8f, 1.f);
	position = glm::vec3(0.f, 0.f, 0.f);
	constantAttenuation = 1;
	linearAttenuation = 0.05f;
	quadraticAttenuation = 0.01f;
}

glm::mat4 Light::getMatVP(float aspectRatio)
{
	glm::mat4 lightPerspective = glm::perspective(glm::radians(90.f), aspectRatio, 0.1f, 100.f);
	glm::mat4 lightView = glm::lookAt(position, glm::vec3(0, 0, 0), glm::vec3(1, 0, 0)); 
	//up-vector is not world up. Needs to be adjusted based on position and target
	return lightPerspective * lightView;
	//glm::ortho(-10.f, 10.f, -10.f, 10.f, 0.1f, 100.f);
	//lightPerspective should be projection for a positional/spot light source, orthographic for a directional light source.
}
