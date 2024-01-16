#pragma once
#include <GLM/vec4.hpp>

class Material
{
public:
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	float shininess;
	Material();
};

