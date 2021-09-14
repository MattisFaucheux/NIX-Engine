#pragma once

#include <Vector/Vector4.h>
#include "Light/ELightType.h"

namespace NIX::Rendering
{
	struct Light
	{
		LibMath::Vector4 ambient = {1.0f, 1.0f , 1.0f, 1.0f};
		LibMath::Vector4 diffuse = { 1.0f, 1.0f , 1.0f, 1.0f };
		LibMath::Vector4 specular = { 1.0f, 1.0f , 1.0f, 1.0f };

		LibMath::Vector4 position = { 0.0f, 0.0f, 0.0f, 0.0f };
		LibMath::Vector4 direction = { 0.0f, 0.0f, 0.0f, 0.0f};

		LibMath::Vector4 attenuation = {0.0f, 0.0f, 0.0f, 0.0f};

		ELightType lightType = ELightType::UNDEFINED;
		float innerCone = 0.90f;
		float outerCone = 0.99f;
		bool isActive = true;
		
		explicit Light(ELightType lightType);

		bool operator==(const Light& other);
		
		Light() = delete;
		Light(const Light& other) = default;
		Light(Light&& other) noexcept = default;
		Light& operator=(const Light& other) = default;
		Light& operator=(Light&& other) noexcept = default;
		~Light() = default;

	};
}


