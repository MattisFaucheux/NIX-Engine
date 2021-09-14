#pragma once

#include <vulkan/vulkan_core.h>

#include "EShaderType.h"

#include <string>

namespace NIX::Rendering
{
	class Shader
	{
	private:
		EShaderType m_shaderType = EShaderType::UNDEFINED;
		VkShaderModule m_shader = nullptr;
	public:
		Shader() = delete;
		Shader(const Shader& other) = delete;
		Shader(Shader&& other) noexcept = delete;
		Shader& operator=(const Shader& other) = delete;
		Shader& operator=(Shader&& other) noexcept = delete;
		~Shader();

		Shader(const std::string& path, EShaderType shaderType);

		[[nodiscard]] VkShaderModule GetVkShader() const;
	};
}


