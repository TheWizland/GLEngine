#pragma once
#include <GLM/vec4.hpp>
#include <GLM/vec3.hpp>
#include <GLM/mat4x4.hpp>

enum LightType
{
	Positional, //Position, Projection Matrix (+ Cubemap?)
	Directional, //Direction, Ortho Matrix
	Spot //Position + Direction, Projection Matrix
};
//TODO: Add directional and spot lights.

class Light
{
private:
public:
	static glm::vec4 globalAmbient;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec3 position;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
	LightType type = LightType::Positional;
	glm::mat4 getView();
	glm::mat4 getPerspective(float aspectRatio);
	Light();
};

