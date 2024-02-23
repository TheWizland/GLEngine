#pragma once
#include <GLM/vec4.hpp>

namespace Lighting
{
	enum LightType
	{
		Positional = 0, //Position, Projection Matrix (+ Cubemap?)
		Directional = 1, //Direction, Ortho Matrix
		Spot = 2 //Position + Direction, Projection Matrix
	};
	//TODO: Add directional and spot lights.

	struct Attenuation
	{
		float constant = 1;
		float linear = 0.05f;
		float quadratic = 0.01f;
	};

	struct ADS
	{
		glm::vec4 ambient = glm::vec4(0.3f, 0.3f, 0.3f, 1.f);
		glm::vec4 diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.f);
		glm::vec4 specular = glm::vec4(0.8f, 0.8f, 0.8f, 1.f);
	};
}