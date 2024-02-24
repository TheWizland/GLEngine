#include "Light.h"

namespace Lighting
{

	glm::vec4 globalAmbient = glm::vec4(0.6f, 0.6f, 0.6f, 1.f);

	void Light::setPosition(float x, float y, float z)
	{
		position = glm::vec3(x, y, z);
	}

	void Light::setPosition(glm::vec3 xyz)
	{
		position = xyz;
	}

	void Light::translate(float x, float y, float z)
	{
		position += glm::vec3(x, y, z);
	}

	void Light::translate(glm::vec3 xyz)
	{
		position += xyz;
	}

	glm::mat4 Light::getView() const
	{
		//TODO: up-vector is not world up. Needs to be adjusted based on position and target
		glm::mat4 lightView = glm::lookAt(position, position + direction, glm::vec3(1, 0, 0));
		return lightView;
	}
}


