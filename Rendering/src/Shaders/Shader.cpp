#include "pch.h"

#include "Tools/Initializers.h"
#include "Shaders/Shader.h"

#include <stdexcept>

#include "Context/Device/Device.h"
#include "Log/Logger.h"
#include "Tools/File.h"

namespace NIX::Rendering
{
	Shader::~Shader()
	{
		vkDestroyShaderModule(Device::GetSingleton()->GetVkLogicalDevice(), m_shader, nullptr);
	}

	Shader::Shader(const std::string& path, EShaderType shaderType)
	{
		m_shaderType = shaderType;

		std::vector<char> code = File::ReadFile(path);

		VkShaderModuleCreateInfo createInfo = Initializers::ShaderModuleCreateInfo(code.size());

		if (code.empty())
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "Shader code is empty : %s", path.c_str());
		}
		
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(Device::GetSingleton()->GetVkLogicalDevice(), &createInfo, nullptr, &m_shader) != VK_SUCCESS)
		{
			ASSERT(SOURCE_INFO, "Failed to create shader module !");
		}
	}
	VkShaderModule Shader::GetVkShader() const
	{
		return m_shader;
	}
}
