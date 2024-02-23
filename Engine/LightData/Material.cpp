#include "Material.h"

namespace Lighting
{
	Material::Material()
	{
		ads.ambient = glm::vec4(0.3f, 0.3f, 0.3f, 1.f);
		ads.diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.f);
		ads.specular = glm::vec4(0.8f, 0.8f, 0.8f, 1.f);
		shininess = 50.0f;
	}
}