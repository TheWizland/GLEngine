#pragma once
#include <glm/vec4.hpp>
#include "LightStruct.h"

namespace Lighting
{
	class Material
	{
	public:
		ADS ads;
		float shininess;
		Material();
	};
}