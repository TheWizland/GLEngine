#pragma once
#include <GLM/vec4.hpp>
#include <GLM/vec3.hpp>

class Light
{
private:
public:
	static glm::vec4 globalAmbient;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec3 position;
	Light();
};

