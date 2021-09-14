#include "pch.h"

#include "Light/Light.h"

namespace NIX::Rendering
{
	Light::Light(ELightType lightType)
		: lightType(lightType)
	{}

	bool Light::operator==(const Light& other)
	{
		if (this == &other)
		{
			return true;
		}

		return false;
	}
}
