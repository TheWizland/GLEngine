#include "Light.h"

glm::vec4 Light::globalAmbient = glm::vec4(0.6f, 0.6f, 0.6f, 1.f);

Light::Light() {
	ambient = glm::vec4(0.3f, 0.3f, 0.3f, 1.f);
	diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.f);
	specular = glm::vec4(0.8f, 0.8f, 0.8f, 1.f);
	position = glm::vec3(0.f, 0.f, 0.f);
}