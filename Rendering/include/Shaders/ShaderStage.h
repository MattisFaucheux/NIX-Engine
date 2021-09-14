#pragma once
#include <vulkan/vulkan_core.h>

namespace NIX::Rendering
{
	namespace ShaderStage
	{
		// Need to stay static const to allow combining them
		static const auto VERTEX = VK_SHADER_STAGE_VERTEX_BIT;
		static const auto GEOMETRY= VK_SHADER_STAGE_GEOMETRY_BIT;
		static const auto FRAGMENT = VK_SHADER_STAGE_FRAGMENT_BIT;
	};
}
