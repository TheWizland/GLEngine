#include "Material.h"

Material::Material()
{
	ambient = glm::vec4(0.3f, 0.3f, 0.3f, 1.f);
	diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.f);
	specular = glm::vec4(0.8f, 0.8f, 0.8f, 1.f);
	shininess = 50.0f;
}
