#pragma once
#include <GLM/vec4.hpp>
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